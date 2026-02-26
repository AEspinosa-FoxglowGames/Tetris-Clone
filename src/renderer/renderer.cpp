#include "renderer.h"
#include "shader.h"

Renderer::Renderer(int w, int h, const std::string vert, const std::string frag)
{
	screenW = w;
	screenH = h;

	screenShader = makeShader(vert.c_str(), frag.c_str());

	frameBuffer.resize(screenW * screenH * 3, 0);

	// Fullscreen quad
	float quadVerts[] = {
		-1,-1, 0,1,
		 1,-1, 1,1,
		 1, 1, 1,0,
		-1,-1, 0,1,
		 1, 1, 1,0,
		-1, 1, 0,0,
	};

	//textures
	glGenTextures(1, &fbTexture);
	glBindTexture(GL_TEXTURE_2D, fbTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenW, screenH, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	//scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

	//position attrib.
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//uv attrib.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Renderer::SetPixel(int x, int y, unsigned int r, unsigned int g, unsigned int b)
{
	if (x < 0 || x >= screenW || y < 0 || y >= screenH) return;

	int idx = (y * screenW + x) * 3;
	frameBuffer[idx] = r;
	frameBuffer[idx + 1] = g;
	frameBuffer[idx + 2] = b;
}
void Renderer::DrawRect(int x, int y, int w, int h, unsigned int r, unsigned int g, unsigned int b)
{
	for(int i = x;i < x + w;i++)
	{
		for (int j = y;j < y + h;j++)
		{
			SetPixel(i, j, r, g, b);
		}
	}
}
void Renderer::Present()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screenW, screenH, GL_RGB, GL_UNSIGNED_BYTE, frameBuffer.data());
	glUseProgram(screenShader);
	glBindVertexArray(quadVAO);
	glUniform1i(glGetUniformLocation(screenShader, "uScreen"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Renderer::Clear(unsigned char r, unsigned char g, unsigned char b)
{
	for (int i = 0; i < screenW * screenH * 3; i += 3) {
		frameBuffer[i] = r;
		frameBuffer[i + 1] = g;
		frameBuffer[i + 2] = b;
	}
}
void Renderer::DrawBG(int w)
{
	int sides = screenW - w;
	DrawRect(0, 0, sides / 2, screenH, 155, 155, 155);
	DrawRect(screenW-sides/2, 0, sides / 2, screenH, 155, 155, 155);
}
