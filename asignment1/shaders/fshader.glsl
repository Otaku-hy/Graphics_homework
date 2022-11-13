#version 430 core

// 给光源数据一个结构体
struct Light{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	vec3 position;
};

// 给物体材质数据一个结构体
struct Material{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float shininess;
};

in vec3 position;
in vec3 normal;

// 相机坐标
uniform vec3 eye_position;

// 光源
uniform Light light;
// 物体材质
uniform Material material;

uniform int isShadow;

layout(binding=0) uniform sampler2D shTex;
uniform mat4 shadowVP;

out vec4 fColor;

void main()
{
	vec3 light_dir = normalize(light.position - position);
	vec3 look_dir = normalize(eye_position - position);
	vec3 half_vec = normalize(look_dir + light_dir);

	float bias = max(0.03 * (1.0 - dot(normal, light_dir)), 0.003);
	vec4 shadowSpaceCoord = (shadowVP * vec4(position,1.0));
	vec3 shadowCoord = shadowSpaceCoord.xyz / shadowSpaceCoord.w;
	shadowCoord = shadowCoord * 0.5 + 0.5;
	float depth = texture(shTex,shadowCoord.xy).r;
	if(shadowCoord.z - bias> depth)
	{
		fColor = (light.ambient * material.ambient);
	}
	else 
	{
		vec3 ambient_color = (light.ambient * material.ambient).xyz;
		vec3 diffuse_color = (max(0,dot(light_dir,normal)) * light.diffuse * material.diffuse).xyz;
		vec3 specular_color = (pow(max(0,dot(half_vec,normal)),material.shininess) * light.specular * material.specular).xyz;

		// ==============@TODO: Task2 注意如果光源在背面则去除高光，修正I_s==============
		if( dot(light_dir, normal) < 0.0 ) {
			specular_color = vec3(0);
		} 

		// ==============@TODO: Task2 合并三个分量的颜色，修正透明度=============
		fColor = vec4(ambient_color + diffuse_color + specular_color,1);
		//fColor = vec4(1.f);
		
	}
}

