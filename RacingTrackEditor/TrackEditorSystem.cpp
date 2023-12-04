#include "TrackEditorSystem.h"


TrackEditorSystem::TrackEditorSystem() {};

TrackEditorSystem::~TrackEditorSystem() {};


int TrackEditorSystem::GLFWInit()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Racing Track Editor", nullptr, nullptr);

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed no init GLEW." << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	return EXIT_SUCCESS;
}

int TrackEditorSystem::OpenGLSetup()
{
	glEnable(GL_BLEND);	// Enables blending ( glBlendFunc )
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	return EXIT_SUCCESS;
}

int TrackEditorSystem::TrackEditorSystemSetup()
{
	coreShader = Shader("Shaders/Core/core.vert", "Shaders/Core/core.frag");
	coreShader.Use();

	renderer = TrackRenderer();
	renderer.setup(window);

	editor = TrackEditor();
	editor.setup(window, &renderer);

	return EXIT_SUCCESS;
}

void TrackEditorSystem::Run()
{

#pragma GCC diagnostic push  // suppress compilation warnings
#pragma GCC diagnostic ignored "-Wwrite-strings"
	coreShader.Use();
	coreShader.LoadTexture("images/woodTexture.jpg", "texture1", "woodTexture");
#pragma GCC diagnostic pop

	bool keyPressed = false;  // flag to control key press

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			renderer.Clear();
		}
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && !keyPressed) {
			std::cout << "Writing files..." << std::endl;
			editor.writeAnimation();
			editor.writeOBJ();
			keyPressed = true;
		}

		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE) {
			keyPressed = false;
		}

		// editor.handleKeyPressed(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// coreShader.Use();
		// coreShader.UseTexture("woodTexture");

		renderer.drawControlPoints();
		renderer.drawLines(editor.controlPoints);
		renderer.drawInternalCurve(editor.internalPoints);
		renderer.drawExternalCurve(editor.externalPoints);
		renderer.drawTrack(editor.track);

		glfwSwapBuffers(window);
	}
}

void TrackEditorSystem::Finish()
{
	coreShader.Delete();

	glfwTerminate();
}
