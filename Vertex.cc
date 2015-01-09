//============================================================================
// Vertex.cc
// @author Gage Heeringa
//
// Vertices represent relationship between files in maze.
//============================================================================
#include "MACROS.H" //maze dimensions, source files for images representing maze
#include "Vertex.h"
using namespace std;

/** Vertex: representation of relationships between files in maze */
Vertex::Vertex():i(-1), j(-1), adj()/*may store [0,4] elements*/, visited(false), path(){}
Vertex::~Vertex(){
	adj.clear();
}
//end constructors
void Vertex::setPos(int row, int col){
	i = row;
	j = col;
}//end setPos()


/** must overload for set<Vertex> to function.
unneeded because am now using set<Vertex*> *//*
const bool Vertex::operator < (const Vertex &o) const{
	if(i < o.i) 
		return true;
	if(i > o.i)
		return false;
	return j < o.j;	
}*/


/** for iterators */
Vertex::iterator Vertex::begin() {
    return adj.begin();
}
Vertex::iterator Vertex::end() {
    return adj.end();
}
Vertex::pathItr Vertex::pathBegin() {
    return path.begin();
}
Vertex::pathItr Vertex::pathEnd() {
    return path.end();
}


/** connect v1 and v2 such that they are adjacent */
void addEdge(Vertex* v1, Vertex* v2){
	v1->adj.insert(v2);
	v2->adj.insert(v1);
}//end addEdge


/** true if v1 and v2 are adjacent else false */
bool areConnected(Vertex* v1, Vertex* v2){
	if(v2->i < 0 || v2->i >= MAZE_LENGTH ||
	   v2->j < 0 || v2->j >= MAZE_WIDTH) //out of bounds
		return false;
	return v1->adj.find(v2) != v1->adj.end();
}//end areConnected


/** return directory to correct source image based on adjacent vertices of vertex.
version 0 = normal maze, version 1 = solution path, version 2 = start/end */
char* adjacentVertices(Vertex* v, Vertex fileGrid[][MAZE_WIDTH], int vers){
	/* 0,1,2,3 = N,S,E,W 
	ex. (neighbors[2] == true) -> is adjacent to vertex east of it in grid */
	bool neighbors[4] = {false};
	int count = 0; //# adjacent vertices
	
	int i = v->i; //row of vertex in grid
	int j = v->j; //column


	if( areConnected(v, &fileGrid[(i-1)][ j ] ) ){ //N
		neighbors[0] = true;
		count++;
	}
	if( areConnected(v, &fileGrid[(i+1)][ j ] ) ){ //S
		neighbors[1] = true;
		count++;
	}
	if( areConnected(v, &fileGrid[i][ (j+1) ] ) ){ //E
		neighbors[2] = true;
		count++;
	}
	if( areConnected(v, &fileGrid[i][ (j-1) ]) ) { //W
		neighbors[3] = true;
		count++;
	}
	
	//determine image
	switch(count){ 
		case 3:
			if( !neighbors[0] ) return vers==0 ? (char*)SEW : vers==1 ? (char*)PATH_SEW : (char*)START_SEW; //north not adjacent
			if( !neighbors[1] ) return vers==0 ? (char*)NEW : vers==1 ? (char*)PATH_NEW : (char*)START_NEW; //south not adjacent
			if( !neighbors[2] ) return vers==0 ? (char*)NSW : vers==1 ? (char*)PATH_NSW : (char*)START_NSW; //east not adjacent
								return vers==0 ? (char*)NSE : vers==1 ? (char*)PATH_NSE : (char*)START_NSE; //west not adjacent
			
		case 2: //4 choose 2 = 6 combos
			if( neighbors[0] ){ //N
				if( neighbors[1] ) return vers==0 ? (char*)NS : vers==1 ? (char*)PATH_NS : (char*)START_NS; //and S
				if( neighbors[2] ) return vers==0 ? (char*)NE : vers==1 ? (char*)PATH_NE : (char*)START_NE; //and E
				if( neighbors[3] ) return vers==0 ? (char*)NW : vers==1 ? (char*)PATH_NW : (char*)START_NW; //and W
			}
			if( neighbors[1] ){ //S
				if( neighbors[2] ) return vers==0 ? (char*)SE : vers==1 ? (char*)PATH_SE : (char*)START_SE; //and E
				if( neighbors[3] ) return vers==0 ? (char*)SW : vers==1 ? (char*)PATH_SW : (char*)START_SW; //and W
			}
								   return vers==0 ? (char*) EW : vers==1 ? (char*)PATH_EW : (char*)START_EW; //E and W
			
		case 1:
			if( neighbors[0] ) return vers==0 ? (char*)NORTH : vers==1 ? (char*)PATH_NORTH : (char*)START_NORTH; //N
			if( neighbors[1] ) return vers==0 ? (char*)S : vers==1 ? (char*)PATH_S : (char*)START_S; //S
			if( neighbors[2] ) return vers==0 ? (char*)E : vers==1 ? (char*)PATH_E : (char*)START_E; //E
							   return vers==0 ? (char*)W : vers==1 ? (char*)PATH_W : (char*)START_W; //W
			
		default: //4
			break;
	}//end switch(count)
	
	return vers==0 ? (char*)ALL : vers==1 ? (char*)PATH_ALL : (char*)START_ALL;
}//end adjacentVertices()


