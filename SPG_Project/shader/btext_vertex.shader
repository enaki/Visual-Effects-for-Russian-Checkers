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
	ourColor = aColor;
	TexCoord = aTexCoord;

	gl_Position = mvpMatrix * vec4(vPos, 1.0);
	normal = vec3(mvpMatrix * vec4(0, 0, 1.0, 0.0));

	pos = vPos;
}
