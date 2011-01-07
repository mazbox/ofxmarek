#ifndef __ofxFileUtils__
#define __ofxFileUtils__
#include <string> 
using namespace std;


/**
 * iPhone apps have tmp directories per application. Use this
 * to get the path for this app.
 */
string getTmpPath();

bool isDirectory(string filename);
/**
 * Check if a file exists, given its path. Returns 1 if it exists.
 */
bool fileExists(string filename);

/**
 * Create a new directory. Returns 1 if successful.
 */
bool createDirectory(string dir);

/**
 * Copies a file from one place to another. Returns 1 if successful.
 */
bool copyFile(string source, string destination);

/**
 * Copies all files in a directory to another directory. (non-recursive)
 */
bool copyDirContentsToDir(string source, string destination);

/**
 * Empties a directory given its path. It's recursive and doesn't 
 * delete the directory itself.
 */
void emptyDir(string path);

/**
 * Same as ofxEmptyDir but also deletes the dir.
 */
void deleteDir(string path);

void deleteFile(string path);


/**
 * Returns the name component of a path
 */
string fileName(string path);

#endif // __ofxZip__
