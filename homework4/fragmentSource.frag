#version 330 core
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D flagTexture;
uniform sampler2D jerryTexture;

void main()
{
//	fragColor = texture(flagTexture, texCoord);
	fragColor = texture(jerryTexture, texCoord);
}