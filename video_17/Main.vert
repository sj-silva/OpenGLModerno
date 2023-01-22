#version 450

uniform mat4 Matrix;

in layout (location=0) vec3 vertexIn;
in layout (location=1) vec3 colorIn;

out vec3 colorOut; 
void main()
{
	gl_Position =  Matrix * vec4(vertexIn, 1.0);
	colorOut = colorIn;
}


