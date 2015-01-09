//============================================================================
// Vertex.h
// @author Gage Heeringa
//
// Vertices represent relationships between files in maze.
//============================================================================
#ifndef VERTEX
#define VERTEX

#include <set> //set
#include <list> //list

/** representation of relationships between files in maze */
class Vertex{													
public:
	int i, j; //position in grid of files
	std::set<Vertex*> adj; //references to adjacent vertices (max of 4)
	
	//used in solving maze
	bool visited; 
	std::list<Vertex*> path; //stores path from start vertex to this vertex
	
	Vertex();
	~Vertex();
	//end constructors
	
	void setPos(int row, int col);

	/** for iterators */
	typedef std::set<Vertex*>::iterator iterator;
    iterator begin();
    iterator end();
	
	typedef std::list<Vertex*>::iterator pathItr;
    pathItr pathBegin();
    pathItr pathEnd();
	
};//END class Vertex


/** connect v1 and v2 such that they are adjacent */
void addEdge(Vertex* v1, Vertex* v2);


/** true if v1 and v2 are adjacent else false */
bool areConnected(Vertex* v1, Vertex* v2);


/** return directory to correct source image based on adjacent vertices of vertex.
version 0 = normal maze, version 1 = solution path, version 2 = start/end */
char* adjacentVertices(Vertex* v, Vertex fileGrid[][MAZE_WIDTH], int vers);



#endif