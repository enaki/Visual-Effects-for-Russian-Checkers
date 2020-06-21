#version 400
layout(location = 0) in vec3 vPos;

// se adauga un atribut ce reprezinta normala la suprafata, la location = 1
//layout (location = 1) in vec3 normalInput;

uniform mat4 mvpMatrix;
uniform mat4 normalMatrix;

out vec3 normal;
out vec3 pos;

void main() 
{
	gl_Position = mvpMatrix * vec4(vPos, 1.0);
	//gl_Position = vec4(vPos, 1.0);
	
	//normala furnizata ca atribut se inmulteste cu normalMatrix si se trimite la fragment shader prin intermediul unei variabile out
	normal = vec3(normalMatrix * vec4(0, 0, 1.0, 1.0));

	pos = vPos;
}