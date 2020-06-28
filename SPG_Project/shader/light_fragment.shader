#version 400
out vec4 fragColor;

in vec3 normal;
in vec3 pos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 color;
uniform bool enableLighting;
uniform bool lightingType;
uniform float specPowerInput;

vec3 lighting_1(vec3 pos, vec3 normal, vec3 lightPos, vec3 viewPos,
				vec3 ambient, vec3 diffuse, vec3 specular, float specPower)
{
	vec3 L = normalize(lightPos - pos);
	vec3 N = normalize(normal);

	vec3 V = normalize(viewPos - pos);
	vec3 R = reflect(-L, N);
	float spec = pow(max(0, dot(R, V)), specPower);

	vec3 color_final = ambient * diffuse + diffuse*max(0, dot(L, N)) + specular * spec;
	return color_final;
}

vec3 lighting_2(vec3 pos, vec3 normal, vec3 lightPos, vec3 viewPos,
				vec3 ambient, vec3 lightColor, vec3 specular, float specPower)
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
	vec3 diffuse = vec3(1.0, 0, 0);
	vec3 specular = vec3(0.8);
	float specPower = max(1, specPowerInput);
	vec3 color_process;
	if (enableLighting){
		if (lightingType) {
			color_process = lighting_2(pos, normal, lightPos, viewPos, ambient, color, specular, specPower);
		}
		else {
			color_process = lighting_1(pos, normal, lightPos, viewPos, ambient, color, specular, specPower);
		}
		fragColor = vec4(color_process, 1.0);
	} else {
		fragColor = vec4(color, 1.0);
	}

	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}