CC= g++

LCFLAGS= -w
LLFLAGS= -L/usr/local/Cellar/glui/2.36/lib -I/usr/local/Cellar/glui/2.36/include -framework OpenGL -framework GLUT -lglui 

MCFLAGS = -Wall -Wextra -stdlib=libstdc++
MCLIBS = /Library/Frameworks/GLUI.framework/Headers
MCFRAME = -framework OpenGL -framework GLUT -framework GLUI

CFILES = main.cpp solarSystem.cpp 
BNAME = ss

louis: *.cpp
	$(CC) $(LCFLAGS) $(CFILES) $(LLFLAGS) -o $(BNAME)

marcella: *.cpp
	$(CC) $(MCFLAGS) $(CFILES) -I $(MCLIBS) $(MCFRAME) -o $(BNAME) 

clean:
	-rm *.o $(BINARIES)

depend: 
	makedepend *.cpp			
