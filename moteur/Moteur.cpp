// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <time.h>       /* time */
#include <fstream>
#include <string>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>

using namespace glm;

#include <Global.hpp>
#include <Game/Game.hpp>

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;
const int limiteur = 60;

Game* game;

// glfw: whenever the global_window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	screen_width = width;
	screen_height = height;
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


void mainLoop() {
	game = new Game();
	double lastFrame = 0.0f;
	double currentFrame = glfwGetTime();
	double deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	double sumDelta = 0.0f;
	do {
		// Measure speed
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		sumDelta += deltaTime;
		// FPS : float fps = 1.0f/deltaTime;
		// input
		// -----
		if (sumDelta >= 1.0f / (limiteur + 1)) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			game->Loop((float)sumDelta);

			// Swap buffers
			glfwSwapBuffers(global_window);
			glfwPollEvents();
			sumDelta = 0.0f;
		}
	} // Check if the ESC key was pressed or the global_window was closed
	while (!global_stop && glfwWindowShouldClose(global_window) == 0);
}



int main(void)
{
	//Init GLFW
	screen_width = SCR_WIDTH;
	screen_height = SCR_HEIGHT;
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Open a global_window and create its OpenGL context
	global_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Moteur - GLFW", NULL, NULL);
	if (global_window == NULL) {
		fprintf(stderr, "Failed to open GLFW global_window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(global_window);
	glfwSetFramebufferSizeCallback(global_window, framebuffer_size_callback);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	printf("OpenGL version supported by this platform : %s\n", glGetString(GL_VERSION));

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(global_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(global_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Hide the mouse and enable unlimited mouvement
	//  glfwSetInputMode(global_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(global_window, SCR_WIDTH / 2, SCR_HEIGHT / 2);

	//Init OpenGL

	// Dark blue background
	glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	/*GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);*/

	//Launch main loop
	mainLoop();

	// Cleanup VBO and shader

	//glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL global_window and terminate GLFW
	glfwTerminate();



	return 0;
}

