#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>	
using namespace std;



class FastXmlLine;

class FastXmlElement {
public:
	// the tag's name
	string name;
	
	// any data contained in the tag
	string data;
	
	// the tag's children
	vector<FastXmlElement*> children;
	
	// the tag's parent
	FastXmlElement *parent;
	
	// the attributes
	map<string,string> attributes;
	
	FastXmlElement *operator[] (unsigned i);
	
	string getAttribute(string key);
	double getAttributeD(string key);
	int    getAttributeI(string key);
	float  getAttributeF(string key);
	bool   getAttributeB(string key);
	// number of children
	int size(); 
	void print(int level = 0);

public:
// don't use these!
	void deleteChildren();
	void loadFromLine(FastXmlLine *line);
};

class FastXmlDocument: public FastXmlElement {
public: 
	
	FastXmlDocument();	
	~FastXmlDocument();

	bool loadFile(string path);
	
	
private:
	void unload();
	bool loaded;
	char *end;
	char *tagBuffer;
	char *readTextFile(string path);
	void copy(char *label, char *start, char *end);
	char *nextTagFlat(char *head);
	char *nextTag(char *head);
	char *nextData(char *head, bool &foundData);


public: // don't use this!
	void getXMLAsLines(string path, vector<FastXmlLine*> *lines);
};
