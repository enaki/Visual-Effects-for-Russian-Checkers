#version 400
in vec3 vp;
uniform mat4 modelMatrix;
out vec3 position;
void main() {
	gl_Position = modelMatrix * vec4(vp, 1.0);
}