/*
 *  Logger.h
 *  Pandemonium
 *
 *  Created by Marek Bereza on 20/11/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include <fstream>


using namespace std;


#define INIT_LOG(A) initLog(A);

#define LOG(TEXT){ ofstream out; \
out.open(getLogFile().c_str(), ios::out | ios::app ); \
out << TEXT << endl; \
out.close();}


void initLog(string path);

string getLogFile();

