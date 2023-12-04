#ifndef TRACKRENDERER_H
#define TRACKRENDERER_H

// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// External Libs
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>

// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


class TrackRenderer
{
private:
    GLFWwindow* window;
    std::vector<GLuint>* pointVAOs = new std::vector<GLuint>();
    std::vector<GLuint>* pointVBOs = new std::vector<GLuint>();

    GLuint vao, vbo;
    GLuint lineVao;
    GLuint bsplineVao;
    GLuint internalCurveVao;
    GLuint externalCurveVao;
    GLuint trackVao;

    // GLuint vaoPoints, vboPoints;

    std::vector<GLfloat>* controlPointsFloat = new std::vector<GLfloat>();

public:
    TrackRenderer();
    ~TrackRenderer();
    void Clear();

    void setup(GLFWwindow* window);

    void bindControlPointVAO(float x, float y, float z);
    void bindLinesVAO(const std::vector<glm::vec3*>* controlPoints);
    void bindBsplineVAO(const std::vector<glm::vec3*>* points);
    void bindInternalCurveVAO(const std::vector<glm::vec3*>* points);
    void bindExternalCurveVAO(const std::vector<glm::vec3*>* points);
    void bindTrackVAO(const std::vector<glm::vec3*>* points);

    void drawControlPoints();
    void drawLines(const std::vector<glm::vec3*>* controlPoints);
    void drawBspline(const std::vector<glm::vec3*>* points);
    void drawInternalCurve(const std::vector<glm::vec3*>* points);
    void drawExternalCurve(const std::vector<glm::vec3*>* points);

    void drawTrack(const std::vector<glm::vec3*>* points);
};

#endif
