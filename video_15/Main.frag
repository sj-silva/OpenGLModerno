#version 450

in vec3 colorOut;

//uniform float u_deltaTime;

out vec4 fragColor;

void main()
{
	fragColor = vec4(colorOut, 1.f);
}