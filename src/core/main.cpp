#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Renderer.h"
#include "entity.h"
#include "game.h"

// in main.cpp
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS) return;
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
	game->OnInput(key);
}
void processInput(GLFWwindow* window, Game& game)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	
	// etc...
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//buffer size
	const int screenW = 800;
	const int screenH = 600;
	//window size
	const int windowW = 1600;
	const int windowH = 1200;

	const int worldW = 300; // tetris is 2:1(h:w), so if screen is
	const int fieldX = (screenW - worldW) / 2;

	GLFWwindow* window = glfwCreateWindow(windowW, windowH, "Tetris Clone", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, windowW, windowH);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);


	Renderer renderer(screenW, screenH, "assets/shaders/screen.vert", "assets/shaders/screen.frag");
	float lastTime = glfwGetTime();

	Game game;
	glfwSetWindowUserPointer(window, &game);
	game.Init(screenW, screenH,fieldX,worldW);
	//loop
	while (!glfwWindowShouldClose(window))
	{

		float now = glfwGetTime();
		float dt = now - lastTime;
		lastTime = now;


		processInput(window,game);

		//TEMP
		

		//rendering
		
		game.Update(dt);
		renderer.Clear();
		renderer.DrawBG(worldW);
		game.Draw(renderer);
		renderer.Present();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}