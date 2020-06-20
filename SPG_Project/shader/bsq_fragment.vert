#version 400
out vec4 frag_colour;
in vec3 position;
in vec4 color;
void main() {
	frag_colour = color;
};
