//Model Matrix  with Minor Changes
#version 450

uniform mat4 projection;
uniform mat4 model;

in layout (location=0) vec3 vertexIn;
in layout (location=1) vec3 colorIn;

out vec3 colorOut; 
void main()
{
	gl_Position =  projection * model * vec4(vertexIn, 1.0);
	colorOut = colorIn;
}


