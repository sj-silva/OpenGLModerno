#version 450

in layout (location=0) vec2 vertexIn;
in layout (location=1) vec3 colorIn;

in layout (location=2) mat4 Transformation;

out vec3 colorOut; 

void main()
{
	gl_Position =  Transformation * vec4(vertexIn, 0.f, 1.f);
	
	colorOut = colorIn;
}
