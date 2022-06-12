#version 330 core
in vec4 vertexColor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(vertexColor.x + 0.3f, vertexColor.y + 0.3f, vertexColor.z + 0.3f, 1.0f);
}