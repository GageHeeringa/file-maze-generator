//============================================================================
// DirectoryExplorer.h
// @author Gage Heeringa
//
// Library for exploring Windows directories.
//============================================================================
#ifndef DirectoryExplorer
#define DirectoryExplorer


/** true if saughtPath exists else false */
bool dirExists(const char* saughtPath);


/** print to stdout a list of files and folders in a directory.
depends on dirExists() */
bool listDirectory(const char* inputDirectory);


/** print to a file the list of files and folders in a directory.
depends on dirExists() */
bool outFileDirectory(const char* inputDirectory);




#endif