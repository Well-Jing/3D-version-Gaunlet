#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 inBoneWeights;
layout(location = 4) in ivec4 inBoneIds;

const int MAX_BONES = 250;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 _finalBoneMatrices[MAX_BONES];
uniform mat4 lightSpaceMatrix;
uniform mat4 model;

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

    gl_Position = lightSpaceMatrix * model * _totalPos;
}  