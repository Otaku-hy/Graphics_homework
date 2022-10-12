#version 330 core

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

out vec4 fColor;

void main()
{
	if (isShadow == 1) {
		fColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else {

		// 将顶点坐标、光源坐标和法向量转换到相机坐标系
		//vec3 norm = (vec4(normal, 0.0)).xyz;

		// ===========@TODO: 计算四个归一化的向量 N,V,L,R(或半程向量H)===========
		vec3 light_dir = normalize(light.position - position);
		vec3 look_dir = normalize(eye_position - position);
		vec3 half_vec = normalize(look_dir + light_dir);

		vec3 ambient_color = (light.ambient * material.ambient).xyz;
		vec3 diffuse_color = (max(0,dot(light_dir,normal)) * light.diffuse * material.diffuse).xyz;
		vec3 specular_color = (pow(max(0,dot(half_vec,normal)),material.shininess) * light.specular * material.specular).xyz;

		// ==============@TODO: Task2 注意如果光源在背面则去除高光，修正I_s==============
		if( dot(light_dir, normal) < 0.0 ) {
			specular_color = vec3(0);
		} 

		// ==============@TODO: Task2 合并三个分量的颜色，修正透明度=============
		fColor = vec4(ambient_color + diffuse_color + specular_color,1);
		//fColor.a = 
		
	}
}

