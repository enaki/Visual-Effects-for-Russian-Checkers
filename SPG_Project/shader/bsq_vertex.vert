#version 400
layout(location = 0) in vec3 vp;
layout(location = 1) in vec4 vColor;

uniform mat4 modelMatrix;
out vec3 position;
out vec4 color;

void main() {
	gl_Position = vec4(vp, 1.0);
	color = vColor;
}