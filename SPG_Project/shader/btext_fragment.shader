
#version 400
out vec4 FragColor;
in vec2 TexCoord;
in vec3 ourColor;


uniform sampler2D ourTexture;
void main()
{
	FragColor = vec4(ourColor, 1.0) * texture(ourTexture, TexCoord);
}