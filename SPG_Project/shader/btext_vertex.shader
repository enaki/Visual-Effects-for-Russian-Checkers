#version 400
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
out vec3 ourColor;
out vec2 TexCoord;
out vec3 normal;
out vec3 pos;

uniform mat4 mvpMatrix;
uniform mat4 normalMatrix;


void main()
{
	gl_Position = vec4(vPos, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;

	gl_Position = mvpMatrix * vec4(vPos, 1.0);
	normal = vec3(normalMatrix * vec4(0, 0, 1.0, 1.0));

	pos = vPos;
}
