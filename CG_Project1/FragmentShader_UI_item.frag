#version 330 core

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct LightDirectional{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;
};

struct LightPoint{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;
	float constant;
	float linear;
	float quadratic;
};

struct LightSpot{
vec3 pos;
	vec3 color;
	vec3 dirToLight;
	//float constant;
	//float linear;
	//float quadratic;
	float cosInner;
	float cosOuter;
};

//in vec3 vertexColor;
//in vec2 TexCoord;
in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

uniform Material material;
uniform LightDirectional lightD;
uniform LightDirectional lightD1;
uniform LightPoint lightP0;
uniform LightPoint lightP1;
uniform LightPoint lightP2;
uniform LightPoint lightP3;
uniform LightSpot lightS;
uniform sampler2D ourTexture;
uniform sampler2D ourFace;
uniform vec3 objectColor;
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 lightDirUniform;
uniform vec3 viewPos;

out vec4 FragColor;

vec3 CalcLightDirectional(LightDirectional light, vec3 uNormal, vec3 dirToCamera)
{

	float diffIntensity = max(dot(uNormal, light.dirToLight), 0);
	vec3 diffColor = diffIntensity * light.color * texture(material.diffuse, texCoord).rgb;
	
	vec3 R = normalize(reflect(-light.dirToLight, uNormal)); 

	vec3 viewDirect = normalize(vec3(1.0f, 1.0f, 1.0f));
	float specIntensity = pow(max(dot(R, viewDirect), 0), 16);
	vec3 specColor = specIntensity * light.color * texture(material.specular, texCoord).rgb;

	vec3 ambientColor = 0.8 * texture(material.specular, texCoord).rgb;

	vec3 result = diffColor + specColor + ambientColor;

	return result;
};



void main() 
{
	vec3 finalResult = vec3(0, 0, 0);
	vec3 uNormal = normalize(normal);
	vec3 dirToCamera = normalize(viewPos - fragPos);

	finalResult += CalcLightDirectional(lightD, uNormal, dirToCamera);

	FragColor = vec4(finalResult, 1.0);
	//FragColor = vec4(texture(material.diffuse, texCoord).rgb, 1.0);


}

