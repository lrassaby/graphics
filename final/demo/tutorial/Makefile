CC= clang++

LCFLAGS_QUIET= -w -g -O2
LCFLAGS_VERBOSE= -g -Wall -Wextra 

LLFLAGS= -I/usr/local/Cellar/glew/1.10.0/include -L/usr/local/Cellar/glew/1.10.0/lib -I/usr/local/Cellar/glfw3/3.0.4/include -L/usr/local/Cellar/glfw3/3.0.4/lib -lglew -lglfw3 -framework CoreVideo -framework OpenGL -framework GLUT -framework Cocoa -framework IOKit -lpthread

PTARGET = fountain 

PFILES = tutorial18_particles.cpp common/shader.cpp common/controls.cpp common/texture.cpp

all: fountain 

fountain: 
	$(CC) $(LCFLAGS_QUIET) $(PFILES) $(LLFLAGS) -o $(PTARGET)

dependencies:
	brew install glm
	brew tap homebrew/versions
	brew install --build-bottle --static glfw3

clean:
	rm $(BTARGET) $(PTARGET)
