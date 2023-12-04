# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -IExternal/GLEW/include/ -IExternal/GLFW/include -IExternal/SOIL2/include -IExternal/GLM -IRacingTrackEditor/includes

# Linker flags
LDFLAGS = -LExternal/GLEW/lib/x64 -LExternal/GLFW/lib -LExternal/SOIL2/lib -lglew32 -lglfw3 -lopengl32 -lsoil2 -lgdi32 -luser32

# Source files
SOURCES = RacingTrackEditor/main.cpp RacingTrackEditor/TrackEditorSystem.cpp RacingTrackEditor/TrackRenderer.cpp RacingTrackEditor/TrackEditor.cpp RacingTrackEditor/Shader.cpp RacingTrackEditor/Texture.cpp # Add all your .cpp files

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = app

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(EXECUTABLE)

.PHONY: all clean
