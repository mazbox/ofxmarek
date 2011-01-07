/**
 * Locks a program to a single computer. To use it you must 
 * define UUID_STRING before including this file, and call
 * ofxCheckUUID() in the setup() method of your testApp.
 */

#include <string>
using namespace std;

#ifndef UUID_STRING
	// Marek's laptop
	#define UUID_STRING "12345678-1234-1234-1234-123453812345"

#endif

string ofxGetUUID();


void ofxCheckUUID();