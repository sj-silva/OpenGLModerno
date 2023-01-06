#version 450

uniform vec3 triangleColor;

out vec4 fragColor;

void main()
{
	fragColor = vec4(triangleColor, 1.f);
}