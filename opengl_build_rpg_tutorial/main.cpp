#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
	if (!glfwInit())
	{
		std::cerr << "failed to initialize glfw!" << std::endl;
		return -1;
	}

	// gl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// turn off depricated functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(800, 600, "rpg tutorial", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "failed to initialize glad!" << std::endl;
		return -1;
	}

	// game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();

		// exit on escape press
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

	return 0;
}