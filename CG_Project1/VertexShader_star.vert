#version 330 core                                 

layout(location = 0) in vec3 aPos;  
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 UItransform;
uniform mat4 UImodelMat;
uniform mat4 transform;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;



//out vec3 vertexColor;
//out vec2 TexCoord;
out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;

void main() 
{
	//gl_Position = projMat * viewMat * modelMat * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	gl_Position = UImodelMat * UItransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	fragPos = vec3(modelMat * transform * vec4(aPos, 1.0f));
	//Normal = aNormal;
	normal = mat3(transpose(inverse(modelMat * transform))) * aNormal;
	texCoord = aTexCoord;
}

