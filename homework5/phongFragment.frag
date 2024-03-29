#version 330 core
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// ambient
	float ambientStrength = 0.6f;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse
	vec3 uNormal = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(uNormal, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;

	// specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, uNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = specularStrength * spec * lightColor;

	// result
	vec3 result = (ambient + diffuse + specular) * objectColor;

	fragColor = vec4(result, 1.0f);
}