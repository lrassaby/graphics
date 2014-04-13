/*  =================== File Information =================
  File Name: ply.cpp
  Description:
  Author: (You)

  Purpose:
  Examples:
  ===================================================== */
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <GL/glui.h>
#include "ply.h"
#include "geometry.h"


using namespace std;

/*  ===============================================
      Desc: Default constructor for a ply object
      Precondition:
      Postcondition:
    =============================================== */ 
ply::ply(string _filePath){
	filePath = _filePath;
	vertexList = NULL;
	faceList = NULL;
	properties = 0;	
	// Call helper function to load geometry
	loadGeometry();
}

/*  ===============================================
      Desc: Destructor for a ply object
      Precondition: Memory has been already allocated
      Postcondition:  
      =============================================== */ 
ply::~ply(){
  // Delete the allocated arrays
	delete[] vertexList;

  for (int i = 0; i < faceCount; i++) {
    delete [] faceList[i].vertexList;
  }

	delete[] faceList;
  // Set pointers to NULL
  vertexList = NULL;
  faceList = NULL;
}

/*  ===============================================
      Desc: reloads the geometry for a 3D object
      Precondition:
      Postcondition:
    =============================================== */ 
void ply::reload(string _filePath){
  filePath = _filePath;
  // reclaim memory allocated in each array
  delete[] vertexList;

  for (int i = 0; i < faceCount; i++) {
    delete [] faceList[i].vertexList;
  }

  delete[] faceList;
  // Set pointers to array
  vertexList = NULL;
  faceList = NULL;
  // Call our function again to load new vertex and face information.
  loadGeometry();
}
/*  ===============================================
      Desc: Parses a file and loads vertices into vertexList and faceList.
      Precondition:
      Postcondition:  
      =============================================== */ 
void ply::loadGeometry(){

 /* 
        1. Parse the header
        2.) Update any private or helper variables in the ply.h private section
        3.) allocate memory for the vertexList 
            3a.) Populate vertices
        4.) allocate memory for the faceList
            4a.) Populate faceList
  */


  // Parse header   
    ifstream myfile (filePath.c_str());
    if (myfile.is_open()) {
      string line;
      bool readVertices = false;  // start reading data into our vertex list
      bool readFaces = false; // start reading data into our face list
      int verticesRead = 0;   // How many vertices we have read
      int facesRead = 0;      // How many faces have we read
      properties = -2;      // Starts at -1 because it appears 1 additional time in the file
      // Read in an entire line
      while (getline (myfile,line) ) {
          // Split the line into space separated tokens
          char* delimeter_pointer;
          char* copy = new char[line.length()+1];
          strcpy(copy,line.c_str());
          delimeter_pointer = strtok(copy," ");

          while(delimeter_pointer != NULL){
                // ====== Parsing Header information ======
        if(strcmp(delimeter_pointer,"vertex") == 0){  // Set the vertex count
                    delimeter_pointer = strtok(NULL," ");
                    vertexCount = atoi(delimeter_pointer);
                    // Allocate memory for our vertices
                      vertexList = new vertex[vertexCount];
                      if(vertexList==NULL){
                        cout << "Cannot allocate memory for Vertex List: System out of memory" << endl;
                        exit(1);
                      }
              }
              else if(strcmp(delimeter_pointer,"face") == 0){ // Set the face count
                      delimeter_pointer = strtok(NULL," ");
                      faceCount = atoi(delimeter_pointer);
                      // Allocate memory for our faces
                      faceList = new face[faceCount];
                      if(faceList==NULL){
                        cout << "Cannot allocate memory for FaceList: System out of memory" << endl;
                        exit(1);
                      }
                }
              else if(strcmp(delimeter_pointer,"property") == 0){ // Count the properties
                    properties++;
              }
                // ====== Parsing Header information ======

                // Switch state so we read in faces
              if(verticesRead >= vertexCount && readFaces==false && readVertices==true){
                    readVertices=false;
                      readFaces = true;
              }
                //
              if(readVertices==true && verticesRead < vertexCount){
                  if(properties >= 0){  // Read in the x vertex
                    vertexList[verticesRead].x = atof(delimeter_pointer);
                }
                  if(properties >= 1){   // Read in the y vertex
                    delimeter_pointer = strtok(NULL," ");
                    vertexList[verticesRead].y = atof(delimeter_pointer);
                  }
                  if(properties >= 2){  // Read in the z vertex
                    delimeter_pointer = strtok(NULL," ");
                    vertexList[verticesRead].z = atof(delimeter_pointer);
                  }
                  if(properties >= 3){   //
                    delimeter_pointer = strtok(NULL," ");
                    vertexList[verticesRead].confidence = atof(delimeter_pointer);
                  }
                  if(properties >= 4){
                    delimeter_pointer = strtok(NULL," ");
                    vertexList[verticesRead].intensity = atof(delimeter_pointer);
                  }
          if(properties >= 5){
            delimeter_pointer = strtok(NULL," ");
            vertexList[verticesRead].nx = atof(delimeter_pointer);
          }
          if(properties >= 6){
            delimeter_pointer = strtok(NULL," ");
            vertexList[verticesRead].ny = atof(delimeter_pointer);
          }
          if(properties >=7){
            delimeter_pointer = strtok(NULL," ");
            vertexList[verticesRead].nz = atof(delimeter_pointer);
          }
                  verticesRead++;
        }
              else if(readFaces==true && facesRead < faceCount){
                  // first number is how many faces to read in
                  faceList[facesRead].vertexCount = atoi(delimeter_pointer);
                  // Allocate the vertex list with memory
          faceList[facesRead].vertexList = new int[faceList[facesRead].vertexCount];
                  // Then we store the vertices until our delimeter is null
                  int count = 0;
                  while(count < faceList[facesRead].vertexCount){
                    delimeter_pointer = strtok(NULL," ");
                    faceList[facesRead].vertexList[count] = atoi(delimeter_pointer);
                    count++;
                  }
                  facesRead++;
            }
                    
        // Switch state so we immedietely start reading in vertices the next iteration
        if(strcmp(delimeter_pointer,"end_header")==0){
          cout << "found end_header" << endl;
                  readVertices = true;
                }

                if(delimeter_pointer != NULL){
                  delimeter_pointer = strtok(NULL," ");
                }
      }
      delete copy;
      }
      myfile.close(); 

      scaleAndCenter();
    }
  else{
        cout << "Unable to open file: " << filePath << endl; 
  } 

}


/*  ===============================================
Desc: Moves all the geometry so that the object is centered at 0, 0, 0 and scaled to be between 0.5 and -0.5
Precondition: after all the vetices and faces have been loaded in
Postcondition:
=============================================== */
void ply::scaleAndCenter() {
    // Given a triangle so that you see something on the scene
    if (faceList==NULL || vertexList==NULL){
        return;
    } else {
        vertex total, center, p;
        total.x = 0;
        total.y = 0;
        total.z = 0;
        /* calculate center of mass */
        for(int i = 0; i < vertexCount; i++) {
            p = vertexList[i];
            total.x += p.x;
            total.y += p.y;
            total.z += p.z;
        }

        center.x = total.x/vertexCount;
        center.y = total.y/vertexCount;
        center.z = total.z/vertexCount;

        /* move the points */
        for(int i = 0; i < vertexCount; i++) { 
            p = vertexList[i];
            p.x -= center.x;
            p.y -= center.y;
            p.z -= center.z;
            vertexList[i] = p;
        }



        float xmax = vertexList[0].x, ymax = vertexList[0].y, zmax = vertexList[0].z;
        /* calculate scale factor */
        for(int i = 0; i < vertexCount; i++) {
            p = vertexList[i];
            if (abs(p.x) > xmax) xmax = abs(p.x);
            if (abs(p.y) > ymax) ymax = abs(p.y);
            if (abs(p.z) > zmax) zmax = abs(p.z);
        }
        float bigmax;
        if (xmax > ymax && xmax > zmax) {
            bigmax = xmax;
        } else if (ymax > xmax && ymax > zmax) {
            bigmax = ymax;
        } else bigmax = zmax;

        /* apply scale factor */
        for(int i = 0; i < vertexCount; i++) { 
            p = vertexList[i];
            p.x /= (bigmax * 2);
            p.y /= (bigmax * 2);
            p.z /= (bigmax * 2);
            vertexList[i] = p;
        }
    }
}

/*  ===============================================
Desc:   Finds the normal to three vertices(1 face making up a triangle)
Precondition:  
Postcondition:  Calls 'glNormal3f()' which takes in 3 parameters
=============================================== */
void ply::setNormal(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3) {


    float v1x, v1y, v1z;
    float v2x, v2y, v2z;
    float cx, cy, cz;

    //find vector between x2 and x1
    v1x = x1 - x2;
    v1y = y1 - y2;
    v1z = z1 - z2;

    //find vector between x3 and x2
    v2x = x2 - x3;
    v2y = y2 - y3;
    v2z = z2 - z3;

    //cross product v1xv2

    cx = v1y * v2z - v1z * v2y;
    cy = v1z * v2x - v1x * v2z;
    cz = v1x * v2y - v1y * v2x;

    //normalize

    float length = sqrt(cx * cx + cy * cy + cz * cz);
    cx = cx / length;
    cy = cy / length;
    cz = cz / length; 

    glNormal3f(cx, cy, cz);
}



void makeCube(float dim){
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glBegin(GL_QUADS);

    glVertex3f(dim,dim,dim);
    glVertex3f(-dim,dim,dim);
    glVertex3f(-dim,-dim,dim);
    glVertex3f(dim,-dim,dim);

    glVertex3f(dim,dim,-dim);
    glVertex3f(-dim,dim,-dim);
    glVertex3f(-dim,-dim,-dim);
    glVertex3f(dim,-dim,-dim);

    glVertex3f(dim,dim,dim);
    glVertex3f(dim,-dim,dim);
    glVertex3f(dim,-dim,-dim);
    glVertex3f(dim,dim,-dim);

    glVertex3f(-dim,dim,dim);
    glVertex3f(-dim,-dim,dim);
    glVertex3f(-dim,-dim,-dim);
    glVertex3f(-dim,dim,-dim);

    glVertex3f(dim,dim,dim);
    glVertex3f(-dim,dim,dim);
    glVertex3f(-dim,dim,-dim);
    glVertex3f(dim,dim,-dim);

    glVertex3f(dim,-dim,dim);
    glVertex3f(-dim,-dim,dim);
    glVertex3f(-dim,-dim,-dim);
    glVertex3f(dim,-dim,-dim);

    glEnd();

}



/*  ===============================================
Desc: Draws a filled 3D object
Precondition:
Postcondition:
Error Condition: If we haven't allocated memory for our
faceList or vertexList then do not attempt to render.
=============================================== */  
void ply::render(){
    // Given a triangle so that you see something on the scene
    if (faceList==NULL || vertexList==NULL){
        return;
    } else {
        // For each of our faces
        for(int i = 0; i < faceCount; i++){
            // Get the vertices that make up each face from the face list
            vertex p1 = vertexList[faceList[i].vertexList[0]];
            vertex p2 = vertexList[faceList[i].vertexList[1]];
            vertex p3 = vertexList[faceList[i].vertexList[2]];

            glBegin(GL_TRIANGLES);
            setNormal(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z);
            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
            glVertex3f(p3.x, p3.y, p3.z);
            glEnd();
        }
    }
    /* for testing use only */

    makeCube(0.5);
}

    /*  ===============================================
      Desc: Draws the wireframe(edges) of a 3D object.
            We could alternatively use 'glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);'

      Note, having two rendering functions is too much, but think
      about how you could modularize this, or other interseting
      ways of rendering. Could you render an object as points?
      What about rendering quads versus triangles, or using triangle
      strips if you know something about the data?

      Note that here we also do not set the normals for lighting. This way our
      geometry could load faster, perhaps different functions could
      have different special effects.

      Precondition:
      Postcondition:
    =============================================== */ 
void ply::renderWireFrame(){
  /* This is optional */
}


/*  ===============================================
      Desc: Prints some statistics about the file you have read in
      This is useful for debugging information to see if we parse our file correctly.

      Precondition:
      Postcondition:  
    =============================================== */ 
void ply::printAttributes(){
      cout << "==== ply Mesh Attributes=====" << endl;
      cout << "vertex count:" << vertexCount << endl;
      cout << "face count:" << faceCount << endl;	
      cout << "properties:" << properties << endl;
}

/*  ===============================================
      Desc: Iterate through our array and print out each vertex.

      Precondition:
      Postcondition:  
    =============================================== */ 
void ply::printVertexList(){
  	if(vertexList==NULL){
  		  return;
  	}else{
    		for(int i = 0; i < vertexCount; i++){
    			   cout << vertexList[i].x << "," << vertexList[i].y << "," << vertexList[i].z << endl;
    		}
  	}
}

/*  ===============================================
      Desc: Iterate through our array and print out each face.

      Precondition:
      Postcondition:  
    =============================================== */ 
void ply::printFaceList(){
	if(faceList==NULL){
		return;
	}else{
		// For each of our faces
		for(int i = 0; i < faceCount; i++){
			// Get the vertices that make up each face from the face list
			for(int j = 0; j < faceList[i].vertexCount; j++){
				// Print out the vertex
				int index = faceList[i].vertexList[j];
				cout << vertexList[index].x << "," << vertexList[index].y << "," << vertexList[index].z << endl;
			}
		}
	}
}
