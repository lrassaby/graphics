CC= g++

LCFLAGS_QUIET= -g -w
LCFLAGS_VERBOSE= -g -Wall -Wextra
LLFLAGS= -L/usr/local/Cellar/glui/2.36/lib -I/usr/local/Cellar/glui/2.36/include -framework OpenGL -framework GLUT -lglui 

MCFLAGS = -Wall -Wextra -stdlib=libstdc++
MCLIBS = /Library/Frameworks/GLUI.framework/Headers
MCFRAME = -framework OpenGL -framework GLUT -framework GLUI

TARGET = run

CFILES = main.cpp spline.cpp 

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
