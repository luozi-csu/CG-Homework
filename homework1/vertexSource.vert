#version 330 core
layout (location = 0) in vec3 pos;

out vec4 vertexColor;

void main()
{
	gl_Position = vec4(pos, 1.0f);
	vertexColor = vec4(pos.x + 0.3f, pos.y + 0.3f, pos.z + 0.3f, 1.0f);
}