//============================================================================
// MazeAlgorithms.cc
// @author Gage Heeringa
//
// Algorithms for generating mazes.
//============================================================================
#include "MACROS.h" //maze dimensions, source files for images representing maze
#include "Vertex.h" //addEdge, areConnected, adjacentVertices
#include "MazeAlgorithms.h" 

#include <cstdlib> //rand, srand
#include <ctime> //time
#include <sstream> //stringstream
#include <fstream> //ifstream, ofstream
#include <string> //string
#include <cstdio> //remove
#include <list> //list
#include <queue> //queue
#include <iostream> //cout
#include <algorithm> //random_shuffle
#include <vector> //vector
using namespace std;


/** generate shortest path solution between 2 randomly generated vertices in maze of files in the specified file directory.
folder is the input directory to generate the maze at, but ensured to end with \ so the maze is generated in the correct directory. */
void solveMaze(string folder, Vertex arr[][MAZE_WIDTH]){
	srand(time(NULL) ); //set seed
	
	//randomly generate begin and end positions in maze
	int begini = rand() % MAZE_LENGTH;
	int beginj = rand() % MAZE_WIDTH;
	int endi = rand() % MAZE_LENGTH;
	int endj = rand() % MAZE_WIDTH;
	//printf("bi, bj, ei, ej = %d %d %d %d\n", begini, beginj, endi, endj);
	
	//ensure start & end aren't same
	while(begini == endi && beginj == endj){
		 endi = rand() % MAZE_LENGTH;
		 endj = rand() % MAZE_WIDTH;
	}
	
	//ensure all vertices are marked unvisited
	for(int i = 0; i < MAZE_LENGTH; i++)
		for(int j = 0; j < MAZE_WIDTH; j++)
			arr[i][j].visited = false;
	
	
	/* solve maze.  perform a breadth-first search on the graph 
	beginning at the start vertex and ending at the target vertex, storing all possible paths */
	
	list< Vertex* > shortestPath; //shortest path from start to end
	queue<Vertex*> q; //store visited vertices with one or more unvisited adjacent vertices while searching graph
	
	arr[ begini ][ beginj ].visited = true; //mark start vertex as visited, as the search will begin here
	arr[ begini ][ beginj ].path.push_back( &arr[ begini ][ beginj ] ); //all paths begin at start vertex
	q.push( &arr[begini][beginj] );
	
	//explore all vertices in maze
	while( !q.empty() ){ 
		Vertex* cur = q.front(); //next vertex to backtrack
		q.pop();
		
		//for all vertices adjacent of cur
		Vertex::iterator it; //thanks to typedef
		for(it = (cur->begin()); it != (cur->end()); it++){
			//if adjacent vertex has not been visited
			if( !(*it)->visited ){ 
				 (*it)->visited = true; //mark vertex visited
				 (*it)->path = cur->path; //save directory from start vertex..
				 //cout << "1\n";
				 ((*it)->path).push_back( *it ); //..to this vertex     i think this line is main problem
				 //cout << "2\n";
				 q.push( &arr[ ((*it)->i) ][ ((*it)->j) ]); //current path continues, so store vertex to backtrack later
			}
		}

		//path to end found, so store it if it's shortest found
		if( (cur->i) == endi && (cur->j) == endj ){
			if( shortestPath.empty() ){
				shortestPath = ( cur->path );
				
			}
			else if( (cur->path).size() < shortestPath.size() ){
				shortestPath = cur->path;
				
			}
		}
		
	}//end while( !q.empty() )
	
	//cout << "    -finished breadth first searching\n";
	
	/* update images accordingly */
	stringstream s;
	char* sourceImage; //correct image for file based on maze generation
	ifstream from; //image being copied
	ofstream to; //file to be created
	
	//before switched path from <Vertex> to <Vertex*>: list<Vertex>::iterator listItr = shortestPath.begin(); listItr != shortestPath.end(); listItr++
	for(Vertex::pathItr listItr = shortestPath.begin(); listItr != shortestPath.end(); listItr++){
		//cout << "(" << (listItr)->i << ", " << (listItr)->j << ") -> ";
		
		//begin/end vertices have different images
		if( ( (*listItr)->i==begini && (*listItr)->j==beginj ) || ( (*listItr)->i==endi && (*listItr)->j==endj ) )
			continue;
		
		//every file name is its $row_$column in the file grid
		s << folder << (*listItr)->i << '_' << (*listItr)->j << ".jpeg";
		
		//copy corresponding image for vertex to its file
		sourceImage = adjacentVertices( &arr[ (*listItr)->i ][ (*listItr)->j ] , arr, 1); //correct image for file based on maze generation
		from.open( sourceImage, ios::binary ); //image being copied
		to.open( s.str().c_str(), ios::binary ); //file to be created (need c_str() for older C++ versions)
		to << from.rdbuf(); //copy
			
		
		from.close();
		to.close();
		s.str( string() ); //reset stringstream
		s.clear(); //clear EOF flags
	} //cout << endl;
	
	
	/* update images for start/end vertices */
	
	//begin file of maze will be renamed
	s << folder << begini << '_' << beginj << " BEGIN.jpg";

	//copy corresponding image for begin vertex to its file
	sourceImage = adjacentVertices( &arr[begini][beginj] , arr, 2); //correct image for file based on maze generation
	from.open( sourceImage, ios::binary ); //image being copied
	to.open( s.str().c_str(), ios::binary ); //file to be created (need c_str() for older C++ versions)
	to << from.rdbuf(); //copy
	////close/reset variables
	from.close();
	to.close();
	s.str( string() ); //reset stringstream
	s.clear(); //clear EOF flags
	
	//end file of maze will be renamed
	s << folder << endi << '_' << endj << " END.jpg";
	
	//copy corresponding image for begin vertex to its file
	sourceImage = adjacentVertices( &arr[endi][endj] , arr, 2); //correct image for file based on maze generation
	from.open( sourceImage, ios::binary ); //image being copied
	to.open( s.str().c_str(), ios::binary ); //file to be created (need c_str() for older C++ versions)
	to << from.rdbuf(); //copy
	
	//remove old images that do not have BEGIN or END prefix for start and end vertices
	s.str( string() ); s.clear();
	s << folder << begini << '_' << beginj << ".jpeg";
	remove( s.str().c_str() );
	s.str( string() ); s.clear();
	s << folder << endi << '_' << endj << ".jpeg";
	remove( s.str().c_str() );
	
	cout << "  maze solved.\n";
}//end solveMaze()


/** binary tree maze generation */
void binaryMazeGeneration(Vertex arr[][MAZE_WIDTH]){
	//set seed for random # generation
	srand( time(NULL) ); 
	

	/* for each vertex V, create one edge such that it is connected to one other vertex as in one of the 4 cases:
	 * [ ][*][ ]		[ ][*][ ]		[ ][ ][ ]		[ ][ ][ ]
	 * [ ][V][*]		[*][V][ ]		[ ][V][*]		[*][V][ ]
	 * [ ][ ][ ]		[ ][ ][ ]		[ ][*][ ]		[ ][*][ ]								*/
	int option = rand()%4; // [0,3]
	for(int i = 0; i < MAZE_LENGTH; i++){
		for(int j = 0; j < MAZE_WIDTH; j++){
			//attempt one of 4 edge-connecting cases (chosen randomly)
			int suboption = rand()%2; // [0,1]
			switch(option){
			case 0: //up OR right
				if(suboption == 0){ //up
					if(i != 0) //not top row
						addEdge(&arr[i][j], &arr[i-1][j]);
					else if(j < MAZE_WIDTH-1) //not right border
						addEdge(&arr[i][j], &arr[i][j+1]);
				}
				else{ //right
					if(j < MAZE_WIDTH-1) //not right border
						addEdge(&arr[i][j], &arr[i][j+1]);
					else if(i != 0) //not top row
						addEdge(&arr[i][j], &arr[i-1][j]);

				}
				break;
			case 1: //up OR left
				if(suboption == 0){ //up
					if(i != 0) //not top row
						addEdge(&arr[i][j], &arr[i-1][j]);
					else if(j > 0) //not left border
						addEdge(&arr[i][j], &arr[i][j-1]);
				}
				else{ //left
					if(j > 0) //not left border
						addEdge(&arr[i][j], &arr[i][j-1]);
					else if(i != 0) //not top row
						addEdge(&arr[i][j], &arr[i-1][j]);

				}
				break;
			case 2: //down OR right
				if(suboption == 0){ //down
					if(i != MAZE_LENGTH-1) //not bottom row
						addEdge(&arr[i][j], &arr[i+1][j]);
					else if(j < MAZE_WIDTH-1) //not right border
						addEdge(&arr[i][j], &arr[i][j+1]);
				}
				else{ //right
					if(j < MAZE_WIDTH-1) //not right border
						addEdge(&arr[i][j], &arr[i][j+1]);
					else if(i != MAZE_LENGTH - 1)
						addEdge(&arr[i][j], &arr[i+1][j]);
				}
				break;
			case 3: //down OR left
				if(suboption == 0){ //down
					if(i != MAZE_LENGTH-1) //not bottom row
						addEdge(&arr[i][j], &arr[i+1][j]);
					else if(j > 0) //not left border
						addEdge(&arr[i][j], &arr[i][j-1]);
				}
				else{ //left
					if(j > 0) //not left border
						addEdge(&arr[i][j], &arr[i][j-1]);
					else if(i != MAZE_LENGTH-1) //not bottom row
						addEdge(&arr[i][j], &arr[i+1][j]);
				}
				break;
			}
		}//END for(int j = 0; j < MAZE_WIDTH; j++)
	}//END for(int i = 0; i < MAZE_LENGTH; i++)
}//end binaryMazeGeneration()


/** return list of vertices that are adjacent to v and marked as unprocessed */
list<Vertex> adjacentUnprocessed(Vertex* v, Vertex arr[][MAZE_WIDTH]);


/** return list of vertices that are adjacent to v and marked as processed */
list<Vertex> adjacentProcessed(Vertex* v, Vertex arr[][MAZE_WIDTH]);


/** trick-or-treat */
void trickOrTreatMazeGeneration(Vertex arr[][MAZE_WIDTH]){
	srand(time(NULL) ); //set seed
	//randomly select a house to begin trick-or-treating at at
	int curi = rand() % MAZE_LENGTH;
	int curj = rand() % MAZE_WIDTH;
	arr[ curi ][curj ].visited = true; //start here

	//begin in run around mode
	while(1){
		list<Vertex> adj = adjacentUnprocessed( &arr[curi][curj], arr ); //unvisited houses adjacent to trick-or-treater's current position
		
		//no unvisited adjacent houses implies dead end reached, so begin run back mode
		if(adj.size() == 0){
			bool back = true;
			//search methodically throughout entire neighborhood for an unvisited house adjacent to a visited house
			for(int g = 0; g < MAZE_LENGTH; g++){
				for(int h = 0; h < MAZE_WIDTH; h++){
					if(!arr[g][h].visited){ //unvisited house found
							adj = adjacentProcessed(&arr[g][h], arr); //list of visited houses adjacent to this found house
							if(adj.size() == 0) //no visited houses are adjacent, so keep looking
								continue;
							//else trick-or-treat at this found house
							//can't use random_shuffle on list since it has no random iterator, so must convert to vector
							vector<Vertex> vec( adj.begin(), adj.end());
							random_shuffle( vec.begin(), vec.end() );
							Vertex temp = vec.front();
							int tempi = temp.i;
							int tempj = temp.j;
							addEdge( &arr[g][h], &arr[tempi][tempj]);
/*	for(list<Vertex>::iterator listItr = shortestPath.begin(); listItr != shortestPath.end(); listItr++){ */
							//return to run around mode starting at this house
							curi = g;
							curj = h;
							arr[curi][curj].visited = true;
							back = false;
							break;
					}//end if(!arr[g][h].visited)
				}//end for(int h = 0
				
				if(!back)
					break;
				
			}//end for(int g = 0;
			
			if(back) //still checking back after all houses visited -> maze is completely generated
				break;
			continue; //else continue to while(1)
					
		}//END BACK MODE

		
		//... else there exist unvisited adjacent houses, so keep trick-or-treating
	
		//can't use random_shuffle on list since it has no random iterator, so must convert to vector
		vector<Vertex> vec( adj.begin(), adj.end());
		random_shuffle( vec.begin(), vec.end() );
		Vertex nextHouse = vec.front(); //prepare to run to some unvisited, adjacent house
		int stumblei = nextHouse.i; //coordinates of nextHouse
		int stumblej = nextHouse.j;

		addEdge( &arr[stumblei][stumblej], &arr[curi][curj]); //trick-or-treat at next house
		curi = stumblei;
		curj = stumblej;
		arr[curi][curj].visited = true;

	}//end while(1)
	
}//end huntAndKillMazeGeneration()


/** helper for trickOrTreat()
	return list of vertices that are adjacent to v and marked as unprocessed */
list<Vertex> adjacentUnprocessed(Vertex* v, Vertex arr[][MAZE_WIDTH]){
	list<Vertex> listy;
	int row = v->i;
	int col = v->j;
	//if not out of bounds && not processed
	if(row - 1 >= 0 && !arr[row - 1][col].visited){ //up
		listy.push_back(arr[row - 1][col]); //store as an adjacent vertex that has not been processed
	}
	if(row + 1 < MAZE_LENGTH && !arr[row + 1][col].visited){ //down
		listy.push_back(arr[row + 1][col]);
	}
	if(col - 1 >= 0 && !arr[row][col - 1].visited){ //left
		listy.push_back(arr[row][col - 1]);
	}
	if(col + 1 < MAZE_WIDTH  && !arr[row][col + 1].visited){ //right
		listy.push_back(arr[row][col + 1]);
	}
	return listy;
}//end adjacentUnprocessed()


/** return list of vertices that are adjacent to v and marked as processed */
list<Vertex> adjacentProcessed(Vertex* v, Vertex arr[][MAZE_WIDTH]){
	list<Vertex> listy;
	int row = v->i;
	int col = v->j;
	//if not out of bounds && processed
	if(row - 1 >= 0 && arr[row - 1][col].visited){ //up
		listy.push_back(arr[row - 1][col]); //store as an adjacent vertex that is in the maze
	}
	if(row + 1 < MAZE_LENGTH && arr[row + 1][col].visited){ //down
		listy.push_back(arr[row + 1][col]);
	}
	if(col - 1 >= 0 && arr[row][col - 1].visited){ //left
		listy.push_back(arr[row][col - 1]);
	}
	if(col + 1 < MAZE_WIDTH  && arr[row][col + 1].visited){ //right
		listy.push_back(arr[row][col + 1]);
	}
	return listy;
}//END adjacentProcessed()


