#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in  vec3 position;
in  vec4 colour;

out Vertex {
	vec4 colour;
	vec3 worldPos;
} OUT;

void main(void)	{
	//gl_Position   = vec4(position, 1.0);
	OUT.worldPos 	  = (modelMatrix * vec4(position, 1)).xyz;
	gl_Position 	  = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
	OUT.colour    = vec4(1.0, 0, 0, 1.0); //colour;
}