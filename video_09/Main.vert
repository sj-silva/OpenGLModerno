#version 450

uniform mat4 Matrix;

in layout(location=0) vec2 position;

void main()
{
	gl_Position=Matrix*vec4(position, 0.f, 1.f);
}