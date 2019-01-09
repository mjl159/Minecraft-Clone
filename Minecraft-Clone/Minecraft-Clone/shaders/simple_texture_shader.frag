#version 330

in vec2 texCoords;

uniform int selected;
uniform sampler2D texture;

void main()
{
	if(selected == 1) {
		gl_FragColor = texture2D(texture, texCoords) + vec4(1.0, 1.0, 1.0, 1.0);
	}
	else {
		gl_FragColor = texture2D(texture, texCoords);
	}
}
