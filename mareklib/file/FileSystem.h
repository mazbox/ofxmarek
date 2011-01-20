#include <string>
#include <vector>
using namespace std;

/**
 * Helps you navigate the filesystem
 */
class FileSystem {
public:
	
	
	
	void open(int i);		
	/**
	 * Opens a folder relatively or absolutely.
	 */
	void open(string folderName);	
	/**
	 * Access items numerically like an array
	 */
	string &operator[] (unsigned i);	
	
	bool isDir(int i);	
	bool isDir(string s);
	
	string getPath(int i = -1);
	string getPath(string s);
	
	/**
	 * How many items are there?
	 */
	int size();
	
	/**
	 * Lists the dir to standard out
	 */
	void print();
	
	/**
	 * One level up
	 */
	void up();
	
	/** 
	 * Get current working directory
	 */
	string cwd();
	
private:
	vector<string> folder;
	string currDir;
};