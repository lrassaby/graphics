CC= g++

LCFLAGS= -Wall -Wextra
LLFLAGS= -L/usr/local/Cellar/glui/2.36/lib -I/usr/local/Cellar/glui/2.36/include -framework OpenGL -framework GLUT -lglui 

MCFLAGS = -Wall -Wextra -stdlib=libstdc++
MCLIBS = /Library/Frameworks/GLUI.framework/Headers
MCFRAME = -framework OpenGL -framework GLUT -framework GLUI

CFILES = main.cpp ply.cpp entity.cpp 


louis: *.cpp
	$(CC) $(LCFLAGS) $(CFILES) $(LLFLAGS) -o ui

marcella: *.cpp
	$(CC) $(MCFLAGS) $(CFILES) -I $(MCLIBS) $(MCFRAME) -o ui

clean:
	-rm *.o $(BINARIES)

depend: 
	makedepend *.cpp			
