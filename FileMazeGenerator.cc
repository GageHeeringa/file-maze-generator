//============================================================================
// FileMazeGenerator.cc
// @author Gage Heeringa	
// 
// Generates a maze of files in a specified directory on Windows.
//
// Maze dimensions are specified in MACROS.h  (clean and re-make after changing dimensions)

/* Example run with Make:
   make
   Mazes\mazeExample\
   C:\Users\Gage Heeringa\Desktop\C++_Workspace\Windows\Mazes\anotherExample
   a place\    //(creates directory in same folder as executable)
   exit
   make clean
   
   Example run:
   g++ -Wall DirectoryExplorer.cc Vertex.cc MazeAlgorithms.cc FileMazeGenerator.cc -o FileMazeGenerator
   FileMazeGenerator
   Mazes\EXAMPLE
   exit
*/

/*  TODO:
	set opened window to optimal dimensions
    in trickOrTreatMazeGen(): 
			adjacent un/processed subhelpers could return vertex*,
				but not big deal since never contains more than 4 vertices
			converted list to vector since list has no random iterator, wasteful
*/
//============================================================================
#include "MACROS.h" //maze dimensions, source files for images representing maze
#include "DirectoryExplorer.h" //dirExists
#include "Vertex.h" //Vertex, adjacentVertices
#include "MazeAlgorithms.h" //binaryMazeGeneration, solveMaze, trickOrTreatMazeGeneration

#include <iostream> //cin, cout
#include <cstdio> //printf
#include <windows.h> //CreateDirectory, ShellExecute
#include <fstream> //ofstream, ifstream
#include <sstream> //stringstream
#include <cstdlib> //rand, srand
#include <ctime> //time
using namespace std;


/** generate a maze of files in the specified file directory */
void generateMaze(const char* dir);

	
/** MAIN */
int main(){
	cout << "\nStarted File Maze Generator...  (enter \"exit\" or \"e\" to terminate)\n";
		
	//listOfMazes.txt stores directories containing generated mazes	
	ofstream outStream; 
	outStream.open("listOfMazes.txt", ios_base::app); //open file in append mode
		
	while(1){ 
		cout << "Create maze where?\n";
		char dir[260];
		cin.getline( dir, 259, '\n' ); //ignore \n from input
		
		//exit
		if( !strcmp(dir, "exit") || !strcmp(dir, "e") /*0 if equal*/ )
			break;
			
		//do not modify existing directories
		if( DirectoryExplorer::dirExists(dir) ){
			printf("ERROR: %s already exists.\n\n", dir);
			continue;
		}
		
		//create directory
		int ret = CreateDirectory(dir, NULL); //NULL -> default security settings
		if(ret == 0){
			cout << "ERROR: One or more intermediate directories do not exist.\n\n";
			continue;
		}
		
		//generate and solve maze @ directory
		cout << "  generating maze...\n";
		generateMaze(dir);
		
		//store maze directory in listOfMazes.txt
		outStream << dir << endl;

		//display folder containing maze minimized
		cout << "Opened and minimized folder containing maze.\n";
		ShellExecute(NULL, NULL, dir, NULL, NULL, SW_MINIMIZE); 
		
		cout << endl;
	}//end while(1)
	
	return 0;
}//end main


/** generate a maze of files in the specified file directory then solve it */
void generateMaze(const char* dir){
	Vertex arr[MAZE_LENGTH][MAZE_WIDTH]; //MAZE_LENGTH*MAZE_WIDTH files to create the maze
	for(int i = 0; i < MAZE_LENGTH; i++)
		for(int j = 0; j < MAZE_WIDTH; j++)
			arr[i][j].setPos(i, j);
	
	//method for maze generation
	srand(time(NULL) ); //set seed
	if(rand()%4 < 3)
		trickOrTreatMazeGeneration(arr); // 3/4 chance
	else
		binaryMazeGeneration(arr);

	//create files
	string folder(dir); //specify directory to place files in
	if(dir[folder.length()] != '\\') 
		folder += '\\'; //else files will be placed in parent directory of intentioned
	
	for(int i = 0; i < MAZE_LENGTH; i++){
		for(int j = 0; j < MAZE_WIDTH; j++){
			//every file name is its $row_$column in the file grid
			stringstream s;
			s << folder << i << '_' << j << ".jpeg";
			
			//copy corresponding image for vertex to its file
			char* sourceImage = adjacentVertices( &arr[i][j] , arr, 0); //correct image for file based on maze generation
			ifstream from( sourceImage, ios::binary ); //image being copied
			ofstream to( s.str().c_str(), ios::binary ); //file to be created (need c_str() for older C++ versions)
			to << from.rdbuf(); //copy
		}
	}
	
	//solve
	cout << "  maze generated. solving maze...\n";
	solveMaze(folder, arr);
	
}//end generateMaze()