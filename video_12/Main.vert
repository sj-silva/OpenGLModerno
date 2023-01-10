#version 450

uniform mat4 Matrix;

in layout(location=0) vec2 position;
in layout(location=1) vec3 colorIn;

out vec3 colorOut;

void main()
{
	gl_Position=Matrix*vec4(position, 0.f, 1.f);

	colorOut = colorIn;
}