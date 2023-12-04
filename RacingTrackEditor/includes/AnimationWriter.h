// Internal
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>


class AnimationWriter {
public:
	std::ofstream AnimationFile; // Fix: Declare AnimationFile as std::ofstream

	AnimationWriter() {} // Fix: Add a valid identifier for the constructor

	void createAnimationFile()
	{
		AnimationFile.open("output/pista.anim");
	}

	void addPoint(float x, float y, float z)
	{
		AnimationFile << "v " << x*10 << " " << z*10 << " " << y*10 << std::endl;
	}

	void closeAnimationFile()
	{
		AnimationFile.close();
	}
};
