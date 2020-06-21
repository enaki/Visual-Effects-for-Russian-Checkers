#version 400
out vec4 fragColor;

in vec3 normal;
in vec3 pos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 color;
uniform bool enableLighting;

vec3 lighting(vec3 pos, vec3 normal, vec3 lightPos, vec3 viewPos,
				vec3 ambient, vec3 diffuse, vec3 specular, float specPower)
{
	//functia calculeaza si returneaza culoarea conform cu modelul de iluminare Phong descris in documentatie
	// ...
	
	vec3 L = normalize(lightPos - pos);
	vec3 N = normalize(normal);

	vec3 V = normalize(viewPos - pos);
	vec3 R = reflect(-L, N);
	float spec = pow(max(0, dot(R, V)), specPower);

	vec3 color_final = ambient + diffuse*max(0, dot(L, N)) + specular * spec;
	return color_final;
}

void main() 
{
	vec3 ambient = vec3(0.2);
	vec3 diffuse = vec3(1.0, 0, 0);
	vec3 specular = vec3(0.8);
	float specPower = 32;
	
	if (enableLighting){
		vec3 color_process = lighting(pos, normal, lightPos, viewPos, ambient, color, specular, specPower);
		fragColor = vec4(color_process, 1.0);
	} else {
		fragColor = vec4(color, 1.0);
	}

	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}