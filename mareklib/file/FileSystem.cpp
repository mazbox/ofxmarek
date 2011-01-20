/*
 *  FileSystem.cpp
 *  Snowboard
 *
 *  Created by Marek Bereza on 09/12/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */


#include "FileSystem.h"


#include "ofxDirList.h"
#include "FileUtils.h"


void FileSystem::open(int i) {
	open(folder[i]);
}

/**
 * Opens a folder relatively or absolutely.
 */
void FileSystem::open(string folderName) {
	folder.clear();
	if(folderName[0]=='/') {
		currDir = folderName;
	} else {
		currDir = currDir + "/" + folderName;
	}
	
	// remove trailing slashes
	if(currDir[currDir.size()-1]=='/') {
		currDir = currDir.substr(0, currDir.size()-1);
	}
	
	// list the dir
	ofxDirList dir;
	int numItems = dir.listDir(currDir);
	for(int i = 0; i < numItems; i++) {
		folder.push_back(dir.getName(i));
	}
}

/**
 * Access items numerically like an array
 */
string &FileSystem::operator[] (unsigned i) {
	return folder[i];
}


bool FileSystem::isDir(int i) {
	return isDir(folder[i]);
}

bool FileSystem::isDir(string s) {
	return isDirectory(currDir + "/" + s);
}

string FileSystem::getPath(int i) {
	if(i==-1) return cwd();
	return getPath(folder[i]);
	
}
string FileSystem::getPath(string s) {
	return currDir + "/" + s;
}

/**
 * How many items are there?
 */
int FileSystem::size() {
	return folder.size();
}
void FileSystem::print() {
	printf("%s:\n", currDir.c_str());
	for(int i = 0; i < size(); i++) {
		bool isFolder = isDirectory(currDir + "/" + folder[i]);
		
		printf("\t%s", folder[i].c_str());
		if(isFolder) {
			printf("/");
		}
		printf("\n");
	}
}

/**
 * One level up
 */
void FileSystem::up() {
	string dir = currDir.substr(0, currDir.rfind('/'));
	open(dir);
}

/** 
 * Get current working directory
 */
string FileSystem::cwd() {
	return currDir;
}
