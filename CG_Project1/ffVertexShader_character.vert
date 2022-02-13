#version 330 core                                 

layout(location = 0) in vec3 aPos;  
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 inBoneWeights;
layout(location = 4) in ivec4 inBoneIds;


uniform mat4 transform;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 lightSpaceMatrix;

const int MAX_BONES = 250;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 _finalBoneMatrices[MAX_BONES];

//out vec3 vertexColor;
//out vec2 TexCoord;
out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;
out vec4 fragPosLightSpace;

void main() 
{
	vec4 _totalPos = vec4(0.0f);

	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		if (inBoneIds[i] < 0)
		{
			continue;
		}
		if(inBoneIds[i] >= MAX_BONES)
		{
			_totalPos = vec4(aPos, 1.0);
			break;
		}

		_totalPos += inBoneWeights[i] * _finalBoneMatrices[inBoneIds[i]] *	vec4(aPos, 1.0);
	}


    gl_Position = projMat * viewMat * modelMat * transform * _totalPos;
	//vertexColor = aColor;  
	//TexCoord = aTexCoord;
	fragPos = vec3(modelMat * transform * vec4(aPos, 1.0f));
	fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0f);
	//Normal = aNormal;
	normal = mat3(transpose(inverse(modelMat * transform))) * aNormal;
	texCoord = aTexCoord;
}

