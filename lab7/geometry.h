#ifndef GEOMETRY_H
#define GEOMETRY_H

/*  ============== Vertex ==============
	Purpose: Stores properties of each vertex
	Use: Used in face structure
	==================================== */  
struct vertex{
	float x,y,z;		// position in 3D space
	float confidence;
	float intensity;
	float r,g,b;		// Color values
//	int facesnum;           //these aren't actually used for silhouette
//	int faces[20];          //usually 20 is enough
};

/*  ============== Face ==============
	Purpose: Store list of vertices that make up a polygon.
	In modern versions of OpenGL this value will always be 3(a triangle)
	Use: Used in Shape data structure.
	==================================== */  
struct face{
	int vertexCount;  // assuming 3 for this
	int* vertexList;  // assuming this has size 3
  
        //normal vector
        float normX, normY, normZ;
        //dot of normal and look
        float dotProd;

        // Default constructor
	face(){
		vertexCount = 0;
	}

};

/* Edge: Connects two vertices, and two faces. 
 * Held in an an array of linked lists, the array is sorted by 1st (numbered) vertex
 * but the linked list does not need to be sorted
 */
struct edge{
        struct edge* next;
        int vertexes[2];
        int faces[2];
        
        //default constructor
        edge(){
            next = NULL;
            //these are -1 because 0 would be a meaningful value
            vertexes[0] = -1;
            vertexes[1] = -1;
            faces[0] = -1;
            faces[1] = -1;
        }
};
#endif
