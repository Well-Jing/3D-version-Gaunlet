#version 330 core

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct LightPoint{
	float constant;
	float linear;
	float quadratic;
};

struct LightSpot{
	float cosInner;
	float cosOuter;
};

//in vec3 vertexColor;
//in vec2 TexCoord;
in vec3 normal;
in vec3 fragPos;
in vec2 texCoord;

uniform Material material;
uniform LightPoint lightPoint;
uniform LightSpot lightSpot;
uniform sampler2D ourTexture;
uniform sampler2D ourFace;
uniform vec3 objectColor;
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 lightDirUniform;
uniform vec3 viewPos;

out vec4 FragColor;

void main() 
{
	//	//float distance = length(lightPos - fragPos);
	//	//float attenuation = 1.0 / (lightPoint.constant + lightPoint.linear * distance + lightPoint.quadratic * pow(distance, 2));
	//
	//	vec3 norm = normalize(normal);
	//	vec3 lightDir = -normalize(lightPos - fragPos);
	//
	//	// Diffuse
	//	float diff = max(dot(norm, lightDir), 0.0);
	//	//vec3 diffuse = material.diffuse * diff * lightColor;
	//	vec3 diffuse = texture(material.diffuse, texCoord).rgb * diff * lightColor;
	//
	//	// Spectural
	//	float specularStength = 1.0f;
	//	vec3 viewDir = normalize(viewPos - fragPos);
	//	vec3 reflectDir = reflect(-lightDir, norm);
	//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//	vec3 specular = texture(material.specular, texCoord).rgb * specularStength * spec * lightColor;
	//	//vec3 specular = vec3(1.0f, 0.0f, 0.0f) * texture(material.specular, texCoord).rgb * specularStength * spec * lightColor;
	//
	//	// Emission
	//	vec3 emission = texture(material.emission, texCoord).rgb;
	//
	//	// Ambient
	//	vec3 ambient = texture(material.diffuse, texCoord).rgb * ambientColor;
	//
	//	// Output Color
	//	//vec3 Result =  ambient + (diffuse + specular) * attenuation + emission;
	//	vec3 Result =  ambient + diffuse + specular;
	//
	//	float cosNumber = dot(lightDir, lightDirUniform);
	//	if (cosNumber > lightSpot.cosInner)
	//	{
	//		;
	//	}
	//	else if (cosNumber > lightSpot.cosOuter)
	//	{
	//		;
	//	}
	//	else
	//	{
	//		Result = vec3(0);
	//	}

	//  Result =  Result + emission;
	FragColor = vec4(lightColor, 1.0f);
	//FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0f);
	//FragColor = mix(texture(ourTexture, TexCoord), texture(ourFace, TexCoord), 0.2);
	//FragColor = vec4((diffuse + specular) * ObjectColor, 1.0f);
	//FragColor = vec4((AmbientColor + diffuse) * ObjectColor, 1.0f);
}

