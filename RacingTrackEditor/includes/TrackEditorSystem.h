#ifndef TRACKEDITORSYSTEM_H
#define TRACKEDITORSYSTEM_H

// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>

// External Libs
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>

// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

// Headers
#include "Shader.h"
#include "AssetManager.h"
#include "Time.h"
#include "TrackRenderer.h"
#include "TrackEditor.h"


class TrackEditorSystem
{
private:
	const GLint WIDTH = 600, HEIGHT = 600;
	int screenWidth, screenHeight;

public:
	GLFWwindow* window;
	Shader coreShader;

	TrackRenderer renderer;
	TrackEditor editor;

public:
	TrackEditorSystem();
	~TrackEditorSystem();

	int GLFWInit();
	int OpenGLSetup();
	int TrackEditorSystemSetup();

	void Run();

	void Finish();
};

#endif
