#version 430 core

in vec3 position;

out vec4 color;

void main()
{	
	color = vec4(position.r,position.r,position.r,1);
}

