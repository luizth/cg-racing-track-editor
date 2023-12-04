// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>


class MTLWriter {
public:
	MTLWriter() {}

	static void createMTLFile()
	{
		static std::ofstream mtlFile;
		mtlFile.open("output/pista.mtl");
		mtlFile << "newmtl " << "road\n" << std::endl;
		mtlFile << "Kd " << 1.0 << " " << 0.5 << " " << 0 << std::endl;
		mtlFile << "Ka " << 1.0 << " " << 1.0 << " " << 1.0 << std::endl;
		mtlFile << "Tf " << 1.0 << " " << 1.0 << " " << 1.0 << std::endl;
		mtlFile << "map_Kd " << "textures/road.png" << std::endl;
		mtlFile << "Ni " << 1.0 << std::endl;
		mtlFile << "Ns " << 100.0 << std::endl;
		mtlFile << "Ks " << 1.0 << " " << 1.0 << " " << 1.0 << std::endl;
		mtlFile.close();
	}
};
