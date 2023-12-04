#ifndef TRACKEDITOR_H
#define TRACKEDITOR_H

// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

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

// Writers
#include "MTLWriter.h"
#include "OBJWriter.h"
#include "AnimationWriter.h"


#define PI 3.14159265359
#define HALF_PI PI/2.0

class TrackEditor
{
private:
    TrackRenderer* renderer;

    float height = 0.0;
    bool keyPressed;

    void addControlPoint(float x, float y);

    void curve();
    void normals(std::vector<glm::vec3*>* internalCurve, std::vector<glm::vec3*>* externalCurve, OBJWriter* writer);
    void faces(std::vector<glm::vec3*>* internalCurve, std::vector<glm::vec3*>* externalCurve, OBJWriter* writer);

    std::vector<glm::vec3*>* bspline(std::vector<glm::vec3*>* controlPoints);
    std::vector<glm::vec3*>* sideCurve(std::vector<glm::vec3*>* points, bool external);
    std::vector<glm::vec3*>* finalCurve(std::vector<glm::vec3*>* internalCurve, std::vector<glm::vec3*>* externalCurve);

public:
    std::vector<glm::vec3*>* controlPoints = new std::vector<glm::vec3*>();
    std::vector<glm::vec3*>* bsplinePoints = new std::vector<glm::vec3*>();
    std::vector<glm::vec3*>* internalPoints = new std::vector<glm::vec3*>();
    std::vector<glm::vec3*>* externalPoints = new std::vector<glm::vec3*>();

    std::vector<glm::vec3*>* track = new std::vector<glm::vec3*>();

public:
    TrackEditor();
    ~TrackEditor();

    void setup(GLFWwindow* win, TrackRenderer* ren);
    void handleMouseLeftClick(GLFWwindow* window, int button, int action, int mods);
    void handleKeyPressed(GLFWwindow* window);

    void writeAnimation();
    void writeOBJ();
};

#endif
