#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Renderer
{
	unsigned int quadVBO, quadVAO;
	int screenW, screenH;
	unsigned int screenShader;
	std::vector<unsigned char> frameBuffer;
	unsigned int fbTexture;

public:
	Renderer(int w, int h, const std::string shaderVert, const std::string shaderFrag);
	void SetPixel(int x, int y, unsigned int r, unsigned int g, unsigned int b);
	void DrawRect(int x, int y, int w, int h, unsigned int r, unsigned int g, unsigned int b);
	void Present();
	void Clear(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0);
	void DrawBG(int worldW);
};