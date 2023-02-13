#version 450

in vec3 colorOut;

out vec4 fragColor;

void main()
{
	fragColor = vec4(colorOut, 1.f);
}