# file-maze-generator

Generates an M by N maze of files in a specified directory on Windows only.  (only tested on Windows 8.1)
Maze dimensions are specified in MACROS.h  (clean and re-make after changing dimensions)


See Example.jpg and FileMazeGenerator.cc for more information.


Example run with Make:
make
Mazes\mazeExample\
C:\Users\MY USERNAME\Desktop\C++_Workspace\Windows\file-maze-generator\Mazes\anotherExample
a place\    //(creates directory in same folder as executable)
exit
make clean

Example run:
g++ -Wall DirectoryExplorer.cc Vertex.cc MazeAlgorithms.cc FileMazeGenerator.cc -o FileMazeGenerator
FileMazeGenerator
Mazes\EXAMPLE
exit