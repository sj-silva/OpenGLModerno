#version 450

//uniform mat4 projection;
//uniform mat4 model;
uniform mat4 Transformation;

in layout (location=0) vec2 vertexIn;
in layout (location=1) vec3 colorIn;
in layout (location=2) vec2 offSet;

out vec3 colorOut; 
void main()
{
	//gl_Position =  projection * model * vec4(vertexIn, 1.0);
	vec4 position=Transformation * vec4(vertexIn, 0.f, 1.f);
	position.xy+=offSet.xy;

	gl_Position = position;

	colorOut = colorIn;
}
