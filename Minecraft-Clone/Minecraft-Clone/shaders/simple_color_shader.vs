#version 330

in vec4 vPosition;
in vec4 vColor;

out vec4 color;

uniform mat4 projection, model_view, ctm;

void main()
{
	color = vColor;
	gl_Position = projection * model_view * ctm * vPosition;
}