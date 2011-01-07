/*
 *  ofxZip.cpp
 *  GuiDownloader
 *
 *  Created by Marek Bereza on 28/03/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "FileUtils.h"


#include <zlib.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

#include <sys/stat.h>
#include <dirent.h>

#define WRITEBUFFERSIZE (8192)

string fileName(string path) {
	// take off trailing slash
	if(path.rfind("/")==path.size()-1) {
		path.resize(path.size()-1);
	}
	return path.substr(path.rfind("/")+1);
}


bool copyFile (string source, string destination) {
	FILE *in, *out;
	
	in = fopen( source.c_str(), "r" );
	out = fopen( destination.c_str(), "w" );
	if(in==NULL || !in) {

		printf("%s: No such file or directory\n",source.c_str());

		return 0;
	} else if(out==NULL || !out) {
		printf("%s: No such file or directory\n",destination.c_str());
		return 0;
	}
	int c;
	while((c=getc(in))!=EOF)
		putc(c,out);
	
	fclose(in);
	fclose(out);
	return 1;
}


void emptyDir(string path) {
	char file[512];
	DIR *dp;
	struct dirent *dirp;
	struct stat statbuf;
	
	dp = opendir(path.c_str());
	
	while((dirp=readdir(dp))!=NULL) {
		if((strcmp(dirp->d_name,".")!=0) && (strcmp(dirp->d_name,"..")!=0)) {
			sprintf(file,"%s/%s",  path.c_str(), dirp->d_name);
			if(dirp->d_type==DT_DIR) {
				emptyDir(file);
				rmdir(file);
			} else {
				remove(file);
			}
		}
	}
}

void deleteDir(string path) {
	emptyDir(path);
	rmdir(path.c_str());
}

void deleteFile(string path) {
	remove(path.c_str());
}



bool isDirectory(string filename) {
	struct stat stFileInfo; 
	
	// Attempt to get the file attributes 
	if(stat(filename.c_str(),&stFileInfo)!=0) {
		printf("File not found\n");
		return false;
	}
	return (stFileInfo.st_mode & S_IFMT) == S_IFDIR;
//	return stFileInfo.st_mode==S_IFDIR;
}


bool fileExists(string filename) { 
	struct stat stFileInfo; 
	
	// Attempt to get the file attributes 
	return stat(filename.c_str(),&stFileInfo)==0; 
}




bool createDirectory(string path) {
	if(mkdir(path.c_str(), 0777)==0) {
		return true;
	} else {
		return false;
	}
}



