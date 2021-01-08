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

	// opengl version setting, major.minor
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// use the deprecated functions? (CORE_PROFILE = no, COMPATABILITY = yes)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(800, 640, "Window", nullptr, nullptr);
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

		// exit on escape key press
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

	return 0;
}