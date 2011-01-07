/*
 *  Logger.cpp
 *  Pandemonium
 *
 *  Created by Marek Bereza on 18/12/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */


#include "Logger.h"

string logFilePath;

void initLog(string path) {
	logFilePath = path;
// just writes an empty string to the log, but doesn't append, so it clears.
	{ ofstream out; 
		out.open(getLogFile().c_str(), ios::out); 
		out << "" << endl; 
		out.close();}
}

string getLogFile() {
	return logFilePath;
}


