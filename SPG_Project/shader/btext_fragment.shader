#version 400
out vec4 fragColor;

in vec3 normal;
in vec3 pos;
in vec2 TexCoord;
in vec3 ourColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool enableLighting;

uniform sampler2D textureColor;
uniform sampler2D textureNormal;

uniform mat4 mvpMatrix;
uniform int enableNormal;

vec3 lighting(vec3 pos, vec3 normal, vec3 lightPos, vec3 viewPos, vec3 ambient, vec3 lightColor, vec3 specular, float specPower)
{

	vec3 L = normalize(lightPos - pos);
	vec3 N = normalize(normal);

	vec3 V = normalize(viewPos - pos);
	vec3 R = reflect(-L, N);

	float diffCoef = max(0, dot(L, N));
	float specCoef = pow(max(0, dot(R, V)), specPower);

	vec3 ambientColor = ambient * lightColor;
	vec3 diffuseColor = diffCoef * lightColor;
	vec3 specularColor = specCoef * specular * lightColor;
	vec3 col = (ambientColor + diffuseColor + specularColor);

	return col;
}

void main()
{
	vec3 ambient = vec3(0.2);
	//vec3 diffuse = vec3(1.0, 0, 0);
	vec3 diffuse = ourColor;
	vec3 specular = vec3(0.8);
	float specPower = 32;

	vec3 normal_to_use = normal;
	if (enableNormal == 1) {
		vec3 normalFromMap = texture(textureNormal, TexCoord).rgb;
		normalFromMap.g = 1 - normalFromMap.g;
		normalFromMap = normalFromMap * 2 - 1;
		normalFromMap = normalize(vec3(mvpMatrix * vec4(normalize(normalFromMap), 0.0)));
		normal_to_use = normalFromMap;
	}

	vec3 colorFromTexture = texture2D(textureColor, TexCoord).rgb;

	vec3 color = lighting(pos, normal_to_use, lightPos, viewPos, ambient, diffuse, specular, specPower) * colorFromTexture;
	if (enableLighting) {
		fragColor = vec4(color, 1.0);
	}
	else {
		fragColor = vec4(ourColor, 1.0) * texture(textureColor, TexCoord);
	}
}