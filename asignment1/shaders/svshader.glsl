#version 430 core

in vec3 vPosition;
in vec3 vColor;
in vec3 vNormal;

out vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{

	vec4 v3 = projection* view * model * vec4(vPosition,1.0);
	
	gl_Position = v3;

    position = v3.xyz;        // 这行需要修改新的position

}

