#version 330 core
in vec4 vertexColor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(0.8f, 0.3f, 0.2f, 1.0f);

	float y = abs(vertexColor.y);

	if ((0.3 < y && y < 0.4) || (0.1 < y && y < 0.2)) {
		FragColor = vec4(0, 1.0f, 0, 1.0f);
	}
}