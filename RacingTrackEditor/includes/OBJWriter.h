// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <sstream>

// GLM Includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


class OBJWriter {
public:
	std::ofstream OBJFile;

	OBJWriter() {}

	void createOBJFile() {
		OBJFile.open("output/pista.obj", std::ofstream::out | std::ofstream::trunc);
		OBJFile << "mtllib " << "pista.mtl" << "\n" << std::endl;
		OBJFile << "g " << "road" << "\n" << std::endl;
		OBJFile << "usemtl road\n" << std::endl;
		OBJFile.close();
	}

	void addFaces(int index, int size, int faces, int n) {
		OBJFile.open("output/pista.obj", std::ios::app);
		if (n == 1) {
			OBJFile << "f " << index + size << "/" << 1 << "/" << faces << " " <<
				index + 1 + size << "/" << 2 << "/" << faces << " " <<
				index << "/" << 3 << "/" << faces << std::endl;
		}
		if (n == 2) {
			OBJFile << "f " << index + 1 + size << "/" << 2 << "/" << faces << " " <<
				index + 1 << "/" << 4 << "/" << faces << " " <<
				index << "/" << 3 << "/" << faces << std::endl;
		}
		if (n == 3) {
			OBJFile << "f " << index + size << "/" << 1 << "/" << faces << " " <<
				size + 1 << "/" << 2 << "/" << faces << " " <<
				index << "/" << 3 << "/" << faces << std::endl;
		}
		if (n == 4) {
			OBJFile << "f " << 1 + size << "/" << 2 << "/" << faces << " " <<
				1 << "/" << 4 << "/" << faces << " " <<
				index << "/" << 3 << "/" << faces << std::endl;
		}
		OBJFile.close();
	}

	void addNormalExternalCurve(glm::vec3 normal_vec_abac, glm::vec3 normal_vec_dbdc) {
		OBJFile.open("output/pista.obj", std::ios::app);
		OBJFile << "vn " << normal_vec_abac[0] << " " << normal_vec_abac[1] << " " << normal_vec_abac[2] << std::endl;
		OBJFile << "vn " << normal_vec_dbdc[0] << " " << normal_vec_dbdc[1] << " " << normal_vec_dbdc[2] << std::endl;
		OBJFile.close();
	}

	void addPointsFinalCurve(float x, float y, float z) {
		OBJFile.open("output/pista.obj", std::ios::app);
		float _x = x, _y = y;
		if (x >= 0) _x *= 20; else _x *= 20;
		if (y >= 0) _y *= 20; else _y *= 20;
		OBJFile << "v " << _x << " " << z << " " << _y << std::endl;  // invertemos o y com o z
		OBJFile.close();
	}

	void saveTextureValuesToOBJ() {
		OBJFile.open("output/pista.obj", std::ios::app);
		// OBJFile << std::endl;
		OBJFile << "vt " << 1.0 << " " << 1.0 << std::endl;
		OBJFile << "vt " << 1.0 << " " << 0.0 << std::endl;
		OBJFile << "vt " << 0.0 << " " << 1.0 << std::endl;
		OBJFile << "vt " << 0.0 << " " << 0.0 << std::endl;
		OBJFile << std::endl;
		OBJFile.close();
	}
};
