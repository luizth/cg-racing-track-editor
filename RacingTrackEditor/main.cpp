#include "TrackEditorSystem.h"

#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0


int main() {

	TrackEditorSystem system;

	if (system.GLFWInit() != 0) {
		return EXIT_FAILURE;
	}
	if (system.OpenGLSetup() != 0) {
		return EXIT_FAILURE;
	}
	if (system.TrackEditorSystemSetup() != 0) {
		return EXIT_FAILURE;
	}

	system.Run();

	system.Finish();

	return EXIT_SUCCESS;
}
