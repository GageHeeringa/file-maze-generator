//============================================================================
// MazeAlgorithms.h
// @author Gage Heeringa
//
// Algorithms for generating mazes.
//============================================================================
#ifndef MAZEALGORITHMS
#define MAZEALGORITHMS

#include "MACROS.h" //maze dimensions, source files for images representing maze
#include <string> //string
#include <list> //list


/** generate shortest path solution between 2 randomly generated vertices in maze of files in the specified file directory.
folder is the input directory to generate the maze at, but ensured to end with \ so the maze is generated in the correct directory. */
void solveMaze(std::string folder, Vertex arr[][MAZE_WIDTH]);


/** return list of vertices that are adjacent to v and marked as unprocessed. */
std::list<Vertex> adjacentUnprocessed(Vertex* v, Vertex arr[][MAZE_WIDTH]);


/** return list of vertices that are adjacent to v and marked as processed. */
std::list<Vertex> adjacentProcessed(Vertex* v, Vertex arr[][MAZE_WIDTH]);


/** trick or treat */
void trickOrTreatMazeGeneration(Vertex arr[][MAZE_WIDTH]);


/** binary tree maze generation */
void binaryMazeGeneration(Vertex arr[][MAZE_WIDTH]);




#endif