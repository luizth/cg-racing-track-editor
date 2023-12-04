#include "TrackRenderer.h"


TrackRenderer::TrackRenderer() {}

TrackRenderer::~TrackRenderer() {}

void TrackRenderer::Clear() {
    // Clean up
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}


static std::vector<GLfloat>* convertVectorToFloat(std::vector<glm::vec3*> points)
{
    std::vector<GLfloat>* pointsFloat = new std::vector<GLfloat>();
    for (const auto& point : points) {
        pointsFloat->push_back(point->x);
        pointsFloat->push_back(point->y);
        pointsFloat->push_back(point->z);
    }
    return pointsFloat;
}


void TrackRenderer::setup(GLFWwindow* win)
{
    window = win;
    glfwSetWindowUserPointer(win, this);  // set user pointer
	glfwMakeContextCurrent(window);
}


void TrackRenderer::drawControlPoints()
{
    for(int i = 0; i < pointVAOs->size(); i++) {
        glBindVertexArray(pointVAOs->at(i));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}


void TrackRenderer::drawLines(const std::vector<glm::vec3*>* controlPoints)
{
    const std::vector<glm::vec3*> points = *controlPoints;

    glBindVertexArray(lineVao);
    glDrawArrays(GL_LINE_STRIP, 0, points.size() + 1);  // +1 for the closing line
    glBindVertexArray(0);
}


void TrackRenderer::drawBspline(const std::vector<glm::vec3*>* points)
{
    const std::vector<glm::vec3*> _points = *points;

    glBindVertexArray(bsplineVao);
    glDrawArrays(GL_LINE_STRIP, 0, _points.size() + 1);  // +1 for the closing line
    glBindVertexArray(0);
}


void TrackRenderer::drawInternalCurve(const std::vector<glm::vec3*>* points)
{
    const std::vector<glm::vec3*> _points = *points;

    glBindVertexArray(internalCurveVao);
    glDrawArrays(GL_LINE_STRIP, 0, _points.size() + 1);  // +1 for the closing line
    glBindVertexArray(0);
}


void TrackRenderer::drawExternalCurve(const std::vector<glm::vec3*>* points)
{
    const std::vector<glm::vec3*> _points = *points;

    glBindVertexArray(externalCurveVao);
    glDrawArrays(GL_LINE_STRIP, 0, _points.size() + 1);  // +1 for the closing line
    glBindVertexArray(0);
}


void TrackRenderer::drawTrack(const std::vector<glm::vec3*>* points)
{
    const std::vector<glm::vec3*> _points = *points;

    glBindVertexArray(trackVao);
    glDrawArrays(GL_TRIANGLES, 0, _points.size() + 1);  // +1 for the closing line
    glBindVertexArray(0);
}


// binds
void TrackRenderer::bindControlPointVAO(float x, float y, float z)
{
    GLfloat cube[] =
	{
		// Positions                // Textures
		 x+0.01,    y+0.01, z,   1.0f, 1.0f, // Top Right
		 x+0.01,    y,      z,   1.0f, 0.0f, // Bottom Right
		 x,         y,      z,   0.0f, 0.0f, // Bottom Left

		 x,         y,      z,   0.0f, 0.0f, // Bottom Left
		 x,	        y+0.01, z,   0.0f, 1.0f, // Top Left
		 x+0.01,    y+0.01, z,   1.0f, 1.0f, // Top Right
	};

    glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind the VBO
    glBindVertexArray(0);  // Unbind VAO

    pointVAOs->push_back(vao);
    pointVBOs->push_back(vbo);
}


void TrackRenderer::bindLinesVAO(const std::vector<glm::vec3*>* controlPoints)
{
    // const std::vector<glm::vec3*> points = *controlPoints;

    glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Since GL_LINES connects every two points, we make sure that
    // the last point is connected to the first one by "closing" the loop
    std::vector<GLfloat> vertices;
    for (const auto& point : *controlPoints) {
        vertices.push_back(point->x);
        vertices.push_back(point->y);
        vertices.push_back(point->z);
    }
    // Connect the last point to the first to close the loop
    vertices.push_back((*controlPoints)[0]->x);
    vertices.push_back((*controlPoints)[0]->y);
    vertices.push_back((*controlPoints)[0]->z);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Set up vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind the VBO
    glBindVertexArray(0);  // Unbind the VAO

    lineVao = vao;
}


void TrackRenderer::bindBsplineVAO(const std::vector<glm::vec3*>* points)
{
    // const std::vector<glm::vec3*> points = *controlPoints;

    glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Since GL_LINES connects every two points, we make sure that
    // the last point is connected to the first one by "closing" the loop
    std::vector<GLfloat> vertices;
    for (const auto& point : *points) {
        vertices.push_back(point->x);
        vertices.push_back(point->y);
        vertices.push_back(point->z);
    }
    // Connect the last point to the first to close the loop
    vertices.push_back((*points)[0]->x);
    vertices.push_back((*points)[0]->y);
    vertices.push_back((*points)[0]->z);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // unbinds
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    bsplineVao = vao;
}


void TrackRenderer::bindInternalCurveVAO(const std::vector<glm::vec3*>* points)
{
    // const std::vector<glm::vec3*> points = *controlPoints;

    glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Since GL_LINES connects every two points, we make sure that
    // the last point is connected to the first one by "closing" the loop
    std::vector<GLfloat> vertices;
    for (const auto& point : *points) {
        vertices.push_back(point->x);
        vertices.push_back(point->y);
        vertices.push_back(point->z);
    }
    // Connect the last point to the first to close the loop
    vertices.push_back((*points)[0]->x);
    vertices.push_back((*points)[0]->y);
    vertices.push_back((*points)[0]->z);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // unbinds
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    internalCurveVao = vao;
}


void TrackRenderer::bindExternalCurveVAO(const std::vector<glm::vec3*>* points)
{
    // const std::vector<glm::vec3*> points = *controlPoints;

    glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Since GL_LINES connects every two points, we make sure that
    // the last point is connected to the first one by "closing" the loop
    std::vector<GLfloat> vertices;
    for (const auto& point : *points) {
        vertices.push_back(point->x);
        vertices.push_back(point->y);
        vertices.push_back(point->z);
    }
    // Connect the last point to the first to close the loop
    vertices.push_back((*points)[0]->x);
    vertices.push_back((*points)[0]->y);
    vertices.push_back((*points)[0]->z);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // unbinds
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    externalCurveVao = vao;
}


void TrackRenderer::bindTrackVAO(const std::vector<glm::vec3*>* points)
{
    // const std::vector<glm::vec3*> points = *controlPoints;

    glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Since GL_LINES connects every two points, we make sure that
    // the last point is connected to the first one by "closing" the loop
    std::vector<GLfloat> vertices;
    for (const auto& point : *points) {
        vertices.push_back(point->x);
        vertices.push_back(point->y);
        vertices.push_back(point->z);
    }
    // Connect the last point to the first to close the loop
    vertices.push_back((*points)[0]->x);
    vertices.push_back((*points)[0]->y);
    vertices.push_back((*points)[0]->z);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // unbinds
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    trackVao = vao;
}
