#version 330

in vec4 vPosition;
in vec2 vTexCoords;

out vec2 texCoords;

uniform mat4 projection, model_view, ctm;

void main()
{
	texCoords = vTexCoords;
	gl_Position = projection * model_view * ctm * vPosition;
}