CC= g++

LCFLAGS_QUIET= -w -O2
LCFLAGS_VERBOSE= -g -Wall -Wextra
LLFLAGS= -L/usr/local/Cellar/glui/2.36/lib -I/usr/local/Cellar/glui/2.36/include -framework OpenGL -framework GLUT -lglui 

MCFLAGS = -Wall -Wextra -stdlib=libstdc++
MCLIBS = /Library/Frameworks/GLUI.framework/Headers
MCFRAME = -framework OpenGL -framework GLUT -framework GLUI

TARGET = run

CFILES = Assignment3.cpp SceneParser.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp Camera.cpp Cube.cpp Cylinder.cpp Cone.cpp Sphere.cpp Face.cpp Shape.cpp Round.cpp Special.cpp 

all: louis

louis: $(CFILES)
	$(CC) $(LCFLAGS_QUIET) $(CFILES) $(LLFLAGS) -o $(TARGET)

v: $(CFILES) 
	$(CC) $(LCFLAGS_VERBOSE) $(CFILES) $(LLFLAGS) -o $(TARGET)

q: louis

jayme: $(CFILES)
	$(CC) $(MCFLAGS) $(CFILES) -I $(MCLIBS) $(MCFRAME) -o $(TARGET)

clean:
	rm $(TARGET)
