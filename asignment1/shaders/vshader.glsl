#version 430 core

// 顶点着色器
in vec3 vPosition;
in vec3 vColor;
in vec3 vNormal;

// 传给片元着色器的变量
out vec3 position;
out vec3 normal;

// 模型变换矩阵、相机观察矩阵、投影矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(binding=0) uniform sampler2D shTex;

void main() 
{
	//在TriMesh.cpp中的computeVertexNormals()已经求好了顶点坐标
	//只需要在phong shading中计算好新的每一个点对应position对应的normal坐标即可
	
	// ===========@TODO: 利用model矩阵计算不同位置的normal===========
	vec4 v1 = model * vec4(vPosition,1);//利用model矩阵进行一次顶点位置变换
	vec4 normW = normalize(model * vec4(vNormal,0));

	// 由于model矩阵有可能为阴影矩阵，为了得到正确位置，我们需要做一次透视除法

	// 考虑相机和投影矩阵
	vec4 v3 = projection* view * v1;
	
	gl_Position = v3;

    position = v1.xyz;        // 这行需要修改新的position
    normal = normW.xyz;			 // 这行需要修改新的normal

}

