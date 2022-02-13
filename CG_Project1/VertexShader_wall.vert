#version 330 core                                 

layout(location = 0) in vec3 aPos;  
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 transform;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform float scale_x;
uniform float scale_y;
uniform float scale_z;
uniform mat4 lightSpaceMatrix;


//out vec3 vertexColor;
//out vec2 TexCoord;
out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;
out vec4 fragPosLightSpace;

void main() 
{
    gl_Position = projMat * viewMat * modelMat * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	//vertexColor = aColor;  
	//TexCoord = aTexCoord;
	fragPos = vec3(modelMat * transform * vec4(aPos, 1.0f));
	fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0f);
	//Normal = aNormal;
	normal = mat3(transpose(inverse(modelMat * transform))) * aNormal;


	// Seting texCoord for the 6 sufaces of a cube
	if (dot(aNormal, vec3(0.0f, 0.0f, 1.0f)) < -0.5 || dot(aNormal, vec3(0.0f, 0.0f, 1.0f)) > 0.5)
	{
		texCoord = vec2(aTexCoord.x * scale_y, aTexCoord.y * scale_x);
	}
	else if(dot(aNormal, vec3(1.0f, 0.0f, 0.0f)) < -0.5 || dot(aNormal, vec3(1.0f, 0.0f, 0.0f)) > 0.5)
	{
		texCoord = vec2(aTexCoord.x * scale_y, aTexCoord.y * scale_z);
	}
	else if(dot(aNormal, vec3(0.0f, 1.0f, 0.0f)) < -0.5 || dot(aNormal, vec3(0.0f, 1.0f, 0.0f)) > 0.5)
	{
		texCoord = vec2(aTexCoord.x * scale_x, aTexCoord.y * scale_z);
	}
	else
	{
		texCoord = aTexCoord;
	}
	



	
}

