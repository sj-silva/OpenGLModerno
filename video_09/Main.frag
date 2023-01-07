#version 450

//uniform vec3 triangleColor;
uniform float u_deltaTime;

out vec4 fragColor;

void main()
{
	float value=abs(sin(u_deltaTime/4.f));
	fragColor = vec4(value, 0.f, value, 1.f);
}