//============================================================================
// DirectoryExplorer.cc
// @author Gage Heeringa
// 
// Library for exploring Windows directories.
//============================================================================
#include <windows.h> //WIN32_FIND_DATA, HANDLE, INVALID_HANDLE_VALUE
//FindClose, FindFirstFile, FindNextFile
#include <cstdio> //printf
#include <dirent.h> //DIR, opendir, closedir
#include <fstream> //ofstream
using namespace std;


/** true if saughtPath exists else false */
bool dirExists(const char* saughtPath){
	string s;
	if(!saughtPath)
		return false;

	DIR *dir; //directory stream
	dir = opendir(saughtPath);

	if(!dir)
		return false;
	
	closedir(dir);
	return true;
}//END dirExists()


/** print to stdout a list of files and folders in a directory.
depends on dirExists() */
bool listDirectory(const char* inputDirectory){ 
	if( !dirExists(inputDirectory) )
		return false;
		
	string temp(inputDirectory); //string temp to append '*' to directory name
	temp += "*";
	char* directory = new char[temp.length() + 1]; // '\0'
	strcpy( directory, temp.c_str() );
	
	
	WIN32_FIND_DATA foundFile; //Windows struct with file info
	memset(&foundFile, 0, sizeof(WIN32_FIND_DATA)); 

	HANDLE handle; //to examine or modify a Windows resource (foundFile)
	handle = FindFirstFile(directory, &foundFile); //foundFile = first file in DIR

	//process all files in directory
	while(handle != INVALID_HANDLE_VALUE){
	  printf("%s\n", foundFile.cFileName /*TCHAR*/);
		
	  if( !FindNextFile(handle, &foundFile) )
		break;
	}

	FindClose(handle); //must close to prevent memory leak
	delete [] directory; //delete pointer, not actual directory
	return true;
}//END listDirectory()


/** print to a file the list of files and folders in a directory.
depends on dirExists() */
bool outFileDirectory(const char* inputDirectory){ 
	if( !dirExists(inputDirectory) )
		return false;
		
	string temp(inputDirectory); //string temp to append '*' to directory name
	temp += "*";
	char* directory = new char[temp.length() + 1]; // '\0'
	strcpy( directory, temp.c_str() );
	
	
	WIN32_FIND_DATA foundFile; //Windows struct with file info
	memset(&foundFile, 0, sizeof(WIN32_FIND_DATA)); 

	HANDLE handle; //to examine or modify a Windows resource (foundFile)
	handle = FindFirstFile(directory, &foundFile); //foundFile = first file in DIR

	
	ofstream outStream("dirFiles.out"); //access outFile containing files in directory
	
	//process all files in directory
	while(handle != INVALID_HANDLE_VALUE){
	  outStream << foundFile.cFileName << endl;
		
	  if( !FindNextFile(handle, &foundFile) )
		break;
	}

	FindClose(handle); //must close to prevent memory leak
	outStream.close(); //note: ofstream destructor automatically closes access
	return true;
}//END outFileDirectory()






/*
int main(){

	while(1){
		char dir[259];
		cin.getline( dir, 260, '\n' ); //ignore \n from input
		
		if(dir[0] == '`') //exit
			return 0;
			
		if( !listDirectory(dir) )
			printf("FAIL! %s does not exist.\n", dir);
	}
	
	return 0;
}//end main
*/