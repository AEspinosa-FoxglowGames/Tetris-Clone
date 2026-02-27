#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Renderer.h"
#include "entity.h"
#include "game.h"
#include <iostream>

// in main.cpp
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_R && game->IsOver()) game->Reset();
		game->OnInput(key); // still fires once on press
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) game->SetHold(true, false);
		if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) game->SetHold(false, true);
	}
	if (action == GLFW_RELEASE)
	{
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) game->SetHold(false, false);
		if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) game->SetHold(false, false);
	}
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
	//renderer.LoadFont("assets/fonts/Aclonica.ttf"); //meh
	renderer.LoadFont("assets/fonts/IMMORTAL.ttf"); //okay?
	//renderer.LoadFont("assets/fonts/AvenueX.ttf"); //wrong file type...
	//renderer.LoadFont("assets/fonts/Megrim.ttf"); //unreadable
	//renderer.LoadFont("assets/fonts/ROBOTECH.ttf"); //okayish?
	//renderer.LoadFont("assets/fonts/OpenSans.ttf"); //ugh...

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

		
		if (!game.IsOver())
		{
			game.Update(dt);
			renderer.Clear();
			renderer.DrawBG(worldW);
			renderer.DrawText("Score: "+std::to_string(game.GetScore()), 10, 25, 255, 255, 255);
			renderer.DrawText("Level: "+std::to_string(game.GetLevel()), 10, 50, 255, 255, 255);
			game.Draw(renderer);
			game.DrawPreview(renderer);
		}
		else
		{
			renderer.Clear();
			renderer.DrawBG(worldW);
			renderer.DrawText("GAME OVER", 370, 280, 255, 0, 0);
			renderer.DrawText("Press R", 380, 300, 255, 255, 255);
		}
		
		renderer.Present();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}