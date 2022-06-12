#version 330 core
in vec4 vertexColor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(0.8f, 0.3f, 0.2f, 1.0f);

	if (vertexColor.x * vertexColor.x + vertexColor.y * vertexColor.y < 0.04f) {
		FragColor = vec4(0, 1.0f, 0, 1.0f);
	}
}