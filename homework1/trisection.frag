#version 330 core
in vec4 vertexColor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(0, 1.0f, 0, 1.0f);

	if (vertexColor.y > vertexColor.x && vertexColor.x < 0) {
		FragColor = vec4(1.0f, 0, 0, 1.0f);
	}
	else if (vertexColor.y > -vertexColor.x && vertexColor.x > 0) {
		FragColor = vec4(1.0f, 1.0f, 0, 1.0f);
	}
}