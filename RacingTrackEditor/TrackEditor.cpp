#include "TrackEditor.h"


TrackEditor::TrackEditor() {}

TrackEditor::~TrackEditor() {}

// wrapper to call member function
static void handleMouseLeftClickStatic(GLFWwindow* window, int button, int action, int mods) {
    TrackEditor* editor = static_cast<TrackEditor*>(glfwGetWindowUserPointer(window));  // get user pointer and cast it to TrackEditor*
    editor->handleMouseLeftClick(window, button, action, mods);  // call member function
}


// util functions
static void convertCoordinates(float& x, float& y) {
    int WIDTH = 600, HEIGHT = 600;
	// se x for maior que a metade da tela da esquerda, ou seja clique na direita, resulta em valores entre 0 e 1
	if (x > (WIDTH / 2)) {
		x = x - (WIDTH / 2);
		x = x / (WIDTH / 2);
	}
	// se estiver no meio
	else if (x == (WIDTH / 2)) {
		x = 0;
	}
	// se o clique for na parte esquerda da tela, entao converte para valores entre -1 e 0
	else {
		x = -(((WIDTH / 2) - x) / (WIDTH / 2));
	}

	// se y for maior que a metade de baixo da tela, ou seja clique na parte de baixo, resulta em valores entre 0 e -1
	if (y > (HEIGHT / 2)) {
		y = y - (HEIGHT / 2);
		y = y / (HEIGHT / 2);
		y = y * (-1);
	}
	// se estiver no meio
	else if (y == (HEIGHT / 2)) {
		y = 0;
	}
	// se o clique for na parte de cima da tela, entao converte para valores entre 0 e 1
	else {
		y = -(((HEIGHT / 2) - y) / (HEIGHT / 2));
		y = y * (-1);
	}
}


void TrackEditor::setup(GLFWwindow* win, TrackRenderer* ren) {
    renderer = ren;
    glfwSetWindowUserPointer(win, this);  // set user pointer
    glfwSetMouseButtonCallback(win, handleMouseLeftClickStatic);  // set callback
}


void TrackEditor::handleMouseLeftClick(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

		this->addControlPoint(xpos, ypos);

		if (controlPoints->size() > 2) {
			this->curve();
		};
	}
}


void TrackEditor::handleKeyPressed(GLFWwindow* window)
{
	// TODO

	/*
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !keyPressed) {
		height += 0.01;
		// keyPressed = true;
		std::cout << "current height: " ;<< height << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !keyPressed) {
		height -= 0.01;
		// keyPressed = true;
		std::cout << "current height: " << height << std::endl;
	}
	*/
}


void TrackEditor::addControlPoint(float xpos, float ypos)
{
	convertCoordinates(xpos, ypos);

	renderer->bindControlPointVAO(xpos, ypos, height);

	glm::vec3* point = new glm::vec3(xpos, ypos, 0.0);  // gera um novo vec3 com o ponto para a curva

	controlPoints->push_back(point);
}


std::vector<glm::vec3*>* TrackEditor::bspline(std::vector<glm::vec3*>* controlPoints)
{
	std::vector<glm::vec3*>* _points = new std::vector<glm::vec3*>();

	for (int i = 0; i < controlPoints->size(); i++) {
		_points->push_back(new glm::vec3(
			controlPoints->at(i)->x,
			controlPoints->at(i)->y,
			controlPoints->at(i)->z
		));
	}

	// cria mais um ponto para terminar a curva, com o ponto inicial
	_points->push_back(controlPoints->at(0));
	_points->push_back(controlPoints->at(1));
	_points->push_back(controlPoints->at(2));

	std::vector<glm::vec3*>* _bsplinePoints = new std::vector<glm::vec3*>();

	for (int i = 0; i < (_points->size() - 3); i++) {
		// itera entre 99 variacoes para a distancia entre cada ponto
		for (int j = 0; j < 100; ++j) {

			float t = static_cast<float>(j) / 99.0;

			GLfloat x = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * _points->at(i)->x +
				(3 * pow(t, 3) - 6 * pow(t, 2) + 4) * _points->at(i + 1)->x +
				(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * _points->at(i + 2)->x +
				pow(t, 3) * _points->at(i + 3)->x) / 6);

			GLfloat y = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * _points->at(i)->y +
				(3 * pow(t, 3) - 6 * pow(t, 2) + 4) * _points->at(i + 1)->y +
				(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * _points->at(i + 2)->y +
				pow(t, 3) * _points->at(i + 3)->y) / 6);

			GLfloat z = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * _points->at(i)->z +
				(3 * pow(t, 3) - 6 * pow(t, 2) + 4) * _points->at(i + 1)->z +
				(-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * _points->at(i + 2)->z +
				pow(t, 3) * _points->at(i + 3)->z) / 6);

			glm::vec3* point = new glm::vec3(x, y, z);

			_bsplinePoints->push_back(point);
		}
	}
	return _bsplinePoints;
}


std::vector<glm::vec3*>* TrackEditor::sideCurve(std::vector<glm::vec3*>* points, bool external)
{
	std::vector<glm::vec3*>* curve = new std::vector<glm::vec3*>();

	for (int j = 0; j < points->size() - 1; j += 1) {
		glm::vec3* a = points->at(j);
		glm::vec3* b;

		if (j == points->size() - 1) b = points->at(0);
		else b = points->at(j + 1);

		GLfloat dx = b->x - a->x;
		GLfloat dy = b->y - a->y;

		if (dx == 0 || dy == 0) {
			dx = b->x - points->at(j - 1)->x;
			dy = b->y - points->at(j - 1)->y;
		}

		// arco tangente
		GLfloat angle = glm::atan(dy, dx);

		if (external) angle += HALF_PI;
		else angle -= HALF_PI;

		// multiplica por um fator de escala para mudar a largura da curva
		GLfloat offsetX = cos(angle) * 0.05;
		GLfloat offsetY = sin(angle) * 0.05;

		glm::vec3* pontoGerado = new glm::vec3(a->x + offsetX, a->y + offsetY, a->z);

		curve->push_back(pontoGerado);
	}
	return curve;
}

std::vector<glm::vec3*>* TrackEditor::finalCurve(std::vector<glm::vec3*>* internalCurve, std::vector<glm::vec3*>* externalCurve)
{
	std::vector<glm::vec3*>* finalCurve = new std::vector<glm::vec3*>();

	// Assuming internalCurve and externalCurve have the same size
	int n = std::min(internalCurve->size(), externalCurve->size());

	for (int i = 0; i < n; i++)
	{
		// exterior t0
		finalCurve->push_back(externalCurve->at(i));

		// interior t0
		finalCurve->push_back(internalCurve->at(i));

		// exterior t1
		finalCurve->push_back(externalCurve->at( (i+1) % n ));

		// interior t0
		finalCurve->push_back(internalCurve->at(i));

		// interior t1
		finalCurve->push_back(internalCurve->at( (i+1) % n ));

		// exterior t1
		finalCurve->push_back(externalCurve->at( (i+1) % n ));
	}

	return finalCurve;



	for (int i = 0; i < n; ++i) {
		finalCurve->push_back(internalCurve->at(i));

		// Check if there's a next point or wrap around to the beginning
		int nextIndex = (i + 1) % n;
		finalCurve->push_back(internalCurve->at(nextIndex));

		finalCurve->push_back(externalCurve->at(i));
		finalCurve->push_back(externalCurve->at(nextIndex));
	}

	return finalCurve;

	/*
	int index = 1;
	int faces = 0;

	int i = 0;
	for (; i < internalCurve->size() - 2; i++)  // i += 2)
	{
		// Ponto Interno 1
		finalCurve->push_back(internalCurve->at(i));
		finalCurve->push_back(internalCurve->at(i + 1));

		// Ponto Interno 2
		finalCurve->push_back(internalCurve->at(i + 2));
		finalCurve->push_back(internalCurve->at(i + 3));

		// Ponto Externo 1
		finalCurve->push_back(externalCurve->at(i));
		finalCurve->push_back(externalCurve->at(i + 1));

		writer->addFaces(index, externalCurve->size(), ++faces, 1);

		// Ponto Interno 2
		finalCurve->push_back(internalCurve->at(i + 2));
		finalCurve->push_back(internalCurve->at(i + 3));

		// Ponto Externo 2
		finalCurve->push_back(externalCurve->at(i + 2));
		finalCurve->push_back(externalCurve->at(i + 3));

		// Ponto Externo 1
		finalCurve->push_back(externalCurve->at(i));
		finalCurve->push_back(externalCurve->at(i + 1));

		writer->addFaces(index, externalCurve->size(), ++faces, 2);
	}

	// O trecho abaixo liga os últimos pontos com primeiro os primeiros
	// Ponto Interno 1
	finalCurve->push_back(internalCurve->at(i));
	finalCurve->push_back(internalCurve->at(i + 1));

	// Ponto Interno 2
	finalCurve->push_back(internalCurve->at(0));
	finalCurve->push_back(internalCurve->at(1));

	// Ponto Externo 1
	finalCurve->push_back(externalCurve->at(i));
	finalCurve->push_back(externalCurve->at(i + 1));

	writer->addFaces(index, externalCurve->size(), ++faces, 3);

	// Ponto Interno 2
	finalCurve->push_back(internalCurve->at(0));
	finalCurve->push_back(internalCurve->at(1));

	// Ponto Externo 2
	finalCurve->push_back(externalCurve->at(0));
	finalCurve->push_back(externalCurve->at(1));

	// Ponto Externo 1
	finalCurve->push_back(externalCurve->at(i));
	finalCurve->push_back(externalCurve->at(i + 1));

	writer->addFaces(index, externalCurve->size(), ++faces, 4);

	return finalCurve;
	*/
}


void TrackEditor::normals(std::vector<glm::vec3*>* internalCurve, std::vector<glm::vec3*>* externalCurve, OBJWriter* writer)
{
	int i = 0;
	int index = 1;

	for (; i < internalCurve->size() - 2; i += 2) {
		glm::vec3* a_int = internalCurve->at(i);
		glm::vec3* b_int = internalCurve->at(i + 2);
		glm::vec3* c_ext = externalCurve->at(i);
		glm::vec3* d_ext = externalCurve->at(i + 2);

		// pega os vetores das normais
		// y e z sao invertidos para modificar os eixos
		// produto escalar
		glm::vec3 ab = glm::vec3(b_int->x - a_int->x, b_int->z - a_int->z, b_int->y - a_int->y);
		glm::vec3 ac = glm::vec3(c_ext->x - a_int->x, c_ext->z - a_int->z, c_ext->y - a_int->y);
		glm::vec3 dc = glm::vec3(c_ext->x - d_ext->x, c_ext->z - d_ext->z, c_ext->y - d_ext->y);
		glm::vec3 db = glm::vec3(b_int->x - d_ext->x, b_int->z - d_ext->z, b_int->y - d_ext->y);

		// produto vetorial
		glm::vec3 normal_vec_abac = cross(ac, ab);
		glm::vec3 normal_vec_dbdc = cross(db, dc);

		writer->addNormalExternalCurve(normal_vec_abac, normal_vec_dbdc);

		index++;
	}

	glm::vec3* a_int = internalCurve->at(i);
	glm::vec3* b_int = internalCurve->at(0);
	glm::vec3* c_ext = externalCurve->at(i);
	glm::vec3* d_ext = externalCurve->at(0);

	// pega os vetores das normais
	// y e z sao invertidos para modificar os eixos
	// produto escalar
	glm::vec3 ab = glm::vec3(a_int->x - b_int->x, a_int->z - b_int->z, a_int->y - b_int->y);
	glm::vec3 ac = glm::vec3(a_int->x - c_ext->x, a_int->z - c_ext->z, a_int->y - c_ext->y);
	glm::vec3 dc = glm::vec3(d_ext->x - c_ext->x, d_ext->z - c_ext->z, d_ext->y - c_ext->y);
	glm::vec3 db = glm::vec3(d_ext->x - b_int->x, d_ext->z - b_int->z, d_ext->y - b_int->y);

	// produto vetorial
	glm::vec3 normal_vec_abac = cross(ab, ac);
	glm::vec3 normal_vec_dbdc = cross(db, dc);

	writer->addNormalExternalCurve(normal_vec_abac, normal_vec_dbdc);
}


void TrackEditor::faces(std::vector<glm::vec3*>* internalCurve, std::vector<glm::vec3*>* externalCurve, OBJWriter* writer)
{
	int index = 1;
	int faces = 0;

	int i = 0;
	for (; i < internalCurve->size() - 2; i++)  // i += 2)
	{
		// interno 1, interno 2, externo 1
		writer->addFaces(index, externalCurve->size(), ++faces, 1);

		// interno 2, externo 2, externo 1
		writer->addFaces(index, externalCurve->size(), ++faces, 2);
	}

	// O trecho abaixo liga os últimos pontos com primeiro os primeiros
	writer->addFaces(index, externalCurve->size(), ++faces, 3);

	writer->addFaces(index, externalCurve->size(), ++faces, 4);
}


void TrackEditor::curve()
{
	track->clear();

	// calculate points for the curve
	bsplinePoints = this->bspline(controlPoints);
	internalPoints = this->sideCurve(bsplinePoints, false);
	externalPoints = this->sideCurve(bsplinePoints, true);

	track = this->finalCurve(internalPoints, externalPoints);

	// finalCurveFloat = convertVectorToFloat(finalCurve);

	renderer->bindLinesVAO(controlPoints);
	renderer->bindInternalCurveVAO(internalPoints);
	renderer->bindExternalCurveVAO(externalPoints);

	renderer->bindTrackVAO(track);
}


void TrackEditor::writeAnimation()
{
	if (this->bsplinePoints->size() == 0) {
		std::cout << "B-Spline not calculated" << std::endl;
		return;
	}

	AnimationWriter* writer = new AnimationWriter();
	writer->createAnimationFile();
	for (int i = 0; i < bsplinePoints->size(); ++i) {
		writer->addPoint(bsplinePoints->at(i)->x, bsplinePoints->at(i)->y, bsplinePoints->at(i)->z);
	}
	writer->closeAnimationFile();
}


void TrackEditor::writeOBJ()
{
	if (this->track->size() == 0) {
		std::cout << "Track not calculated" << std::endl;
		return;
	}

	MTLWriter* mtl = new MTLWriter();
	mtl->createMTLFile();

	OBJWriter* writer = new OBJWriter();

	writer->createOBJFile();

	// vertices
	for (int i = 0; i < track->size(); ++i) {
		writer->addPointsFinalCurve(track->at(i)->x, track->at(i)->y, track->at(i)->z);
	}

	// texture
	writer->saveTextureValuesToOBJ();

	// normals
	this->normals(internalPoints, externalPoints, writer);

	// faces
	this->faces(internalPoints, externalPoints, writer);
}
