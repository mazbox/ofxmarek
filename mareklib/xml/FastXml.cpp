#include "FastXml.h"



enum LineType {
	openTag,
	closeTag,
	emptyTag,
	dataTag
};



class FastXmlLine {
public:
	string str;
	LineType type;
	string name;
	FastXmlLine(string str) {
		this->str = str;
		bool hasSlashAtBeginning = false;
		bool hasSlashAtEnd = false;
		bool hasOpen = false;
		bool hasClosed = false;
		for(int i = 0; i < str.size(); i++) {
			if(str[i]=='<') hasOpen = true;
			else if(str[i]=='>') hasClosed = true;
			else if(str[i]=='/') {
				if(i>0 && str[i-1]=='<') hasSlashAtBeginning = true;
				else hasSlashAtEnd = true;
			}
			
		}
		if(hasOpen && hasClosed) {
			if(hasSlashAtBeginning) {
				type = closeTag;
			} else if(hasSlashAtEnd) {
				type = emptyTag;
			} else {
				type = openTag;
			}
		} else {
			type = dataTag;
		}
		
		
		// find tag name
		if(type==openTag || type==emptyTag) {
			int start = str.find('<')+1;
			int spacePos = str.find(' ');
			int brackPos = str.find('>');
			int length = 0;
			if(spacePos!=-1) length = spacePos - start;
			else length = brackPos - start;
			name = str.substr(start, length);
		} else if(type==closeTag) {
			int start = str.find('/')+1;
			int brackPos = str.find('>');
			int length = brackPos - start;
			name = str.substr(start, length);
		}
		// find attributes
		
		if(type==openTag || type==emptyTag) {
			parseAttributes();
		}
	}
	
	vector<pair<string,string> > attributes;
	
	
	
	
	//--------------------------------------------------
	vector<string> splitString(const string& str, const string& delimiter = " "){
		vector<string> elements;
		// Skip delimiters at beginning.
		string::size_type lastPos = str.find_first_not_of(delimiter, 0);
		// Find first "non-delimiter".
		string::size_type pos     = str.find_first_of(delimiter, lastPos);
		
		while (string::npos != pos || string::npos != lastPos)
		{
			// Found a token, add it to the vector.
			elements.push_back(str.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = str.find_first_not_of(delimiter, pos);
			// Find next "non-delimiter"
			pos = str.find_first_of(delimiter, lastPos);
		}
		return elements;
	}
	
	
	
	void parseAttributes() {
		// check to see if there are any attributes
		if(str.find('=')==-1) return;
		string attrs = str.substr(str.find(' ')+1);
		if(type==openTag) {
			attrs = attrs.substr(0, attrs.find('>'));
		} else {
			attrs = attrs.substr(0, attrs.find('/'));
		}
		vector<string> att = splitString(attrs, " ");
		for(int i = 0; i < att.size(); i++) {
			string key = att[i].substr(0, att[i].find('='));
			string value = att[i].substr(att[i].find("\"")+1);
			value = value.substr(0, value.find("\""));
			attributes.push_back(make_pair(key,value));
		}
	}
	
	void print() {
		if(type==openTag) printf("open tag(%s) %s\n",name.c_str(), str.c_str());
		else if(type==closeTag) printf("close tag(%s) %s\n", name.c_str(), str.c_str());
		else if(type==emptyTag) printf("empty tag (%s) %s\n",name.c_str(), str.c_str());
		else if(type==dataTag) printf("data tag %s\n", str.c_str());
		for(int i = 0; i < attributes.size(); i++) {
			printf("\t'%s' = %s\n", attributes[i].first.c_str(), attributes[i].second.c_str());
		}
	}
};


	void FastXmlElement::loadFromLine(FastXmlLine *line) {
		name = line->name;
		for(int i = 0; i < line->attributes.size(); i++) {
			attributes[line->attributes[i].first] = line->attributes[i].second;
		}
	}
	
FastXmlElement *FastXmlElement::operator[] (unsigned i) {
	return children[i];
}


string FastXmlElement::getAttribute(string key) {
	return attributes[key];
}

bool FastXmlElement::getAttributeB(string key) {
	return attributes[key]=="TRUE" || attributes[key]=="true" || attributes[key]=="1";
}

double FastXmlElement::getAttributeD(string key) {
	return atof(attributes[key].c_str());
}
int    FastXmlElement::getAttributeI(string key) {
	return atoi(attributes[key].c_str());
}
float  FastXmlElement::getAttributeF(string key) {
	return atof(attributes[key].c_str());
}


int FastXmlElement::size() {
	return children.size();
}
	void FastXmlElement::print(int level) {
		for(int i = 0; i < level; i++) printf("\t");
		
		if(attributes.size()==0) {
			printf("<%s>\n", name.c_str());
		} else {
			printf("<%s", name.c_str());
			map<string,string>::iterator it;
			for(it = attributes.begin(); it!=attributes.end(); it++) {
				
				printf(" %s=\"%s\"", (*it).first.c_str(), (*it).second.c_str());	
			}
			printf(">\n");
		}
		
		if(children.size()==0 && data!="") {
			for(int i = 0; i < level+1; i++) printf("\t");
			printf("%s\n", data.c_str());
		}
		for(int i = 0; i < children.size(); i++) {
			children[i]->print(level+1);
		}
		for(int i = 0; i < level; i++) printf("\t");
		printf("</%s>\n", name.c_str());
	}
	void FastXmlElement::deleteChildren() {
		for(int i = 0; i < children.size(); i++) {
			if(children[i]->children.size()==0) {
				delete children[i];
				children.erase(children.begin()+i);
				i--;
			}
		}
	}
	

	FastXmlDocument::FastXmlDocument() {
		loaded = false;
		end = 0;
	}
	
	FastXmlDocument::~FastXmlDocument() {
		//unload();
	}
	
	void FastXmlDocument::unload() {
		if(loaded) {
			while(children.size()>0) deleteChildren();
			loaded = false;
		}
	}
	
	
	
	char *FastXmlDocument::readTextFile(string path) {
		FILE *f = fopen(path.c_str(), "r");
		
		if(f==NULL) {
			printf("Error opening file %s\n", path.c_str());
			return false;
		}
		
		fseek(f, 0L, SEEK_END);
		int size = ftell(f);
		fseek(f, 0L, SEEK_SET);
		
		
		char *data = new char[size+1];
		data[size] = '\0';
		end = data+size;
		size++;
		fread(data, 1, size, f);
		return data;
	}
	void FastXmlDocument::copy(char *label, char *start, char *end) {
		int length = end - start;
		if(length<0) return;
		tagBuffer[length] = '\0';
		memcpy(tagBuffer, start, length);
		//printf("%s %s\n",label, tagBuffer);
	}
	char *FastXmlDocument::nextTagFlat(char *head) {
		
		while(head[0]!='<' && head<end) head++;
		int depth = 1;
		head++;
		char *endTag = head;
		
		while(endTag[0]!='>' && depth>0 && endTag<end) {
			//if(endTag[0]=='<') depth++;
			//else 
			if(endTag[0]=='/') depth--;
			else if(endTag[0]=='<' && endTag[1]!='/') depth++;
			endTag++;
			//printf("Depth: %d\n", depth);
		}
		
		while(endTag[0]!='>' && endTag<end) {
			endTag++;
		}
		
		copy("Tag: ", head-1, endTag-1);
		head = endTag;
		head++;
		return head;
	}
	
	
	char *FastXmlDocument::nextTag(char *head) {
		
		while(head[0]!='<' && head<end) head++;
		int depth = 1;
		head++;
		char *endTag = head;
		
		
		while(endTag[0]!='>' && endTag<end) {
			endTag++;
		}
		
		copy("Tag: ", head-1, endTag+1);
		head = endTag;
		head++;
		return head;
	}
	
	char *FastXmlDocument::nextData(char *head, bool &foundData) {
		
		char *endOfData = head;
		while(endOfData[0]!='<' && endOfData<end) endOfData++;
		char *pos = head;
		bool onlyWhiteSpace = true;
		while(pos<endOfData-1) {
			if(pos[0]!='\r' && pos[0]!='\n' && pos[0]!='\t' && pos[0]!=' ') {
				onlyWhiteSpace = false;
				break;
			}
			pos++;
		}
		if(!onlyWhiteSpace) {
			copy("Data: ", head, endOfData);
			foundData = true;
		} else {
			foundData = false;
		}
		return head;
	}
	
	void FastXmlDocument::getXMLAsLines(string path, vector<FastXmlLine*> *lines) {
		
		char *data = readTextFile(path);
		char *head = data;
		head = nextTag(head);
		bool foundData = false;
		while(head<end) {
			
			head = nextData(head, foundData);
			if(foundData) lines->push_back(new FastXmlLine(tagBuffer));
			head = nextTag(head);
			lines->push_back(new FastXmlLine(tagBuffer));
			
		}
		delete [] data;
	}
	
	
	bool FastXmlDocument::loadFile(string path) {
		
		unload();
		tagBuffer = new char[4096];
		vector<FastXmlLine*> lines;	
		getXMLAsLines(path, &lines);
		
		FastXmlElement *root = NULL;
		int i;
		for(i = 0; i < lines.size(); i++) {
			if(lines[i]->type==openTag) {
				root = this;
				root->parent = NULL;
				root->loadFromLine(lines[i]);
				break;
			}
		}
		
		if(root==NULL) {
			printf("Couldn't find an open tag\n");
			for(int i = 0; i < lines.size(); i++) {
				delete lines[i];
			}
			return false;
		}
		root->name = "root";
		FastXmlElement *currElement = root;
		for(; i < lines.size(); i++) {
			if(lines[i]->type==emptyTag) {
				FastXmlElement *el = new FastXmlElement();
				el->loadFromLine(lines[i]);
				el->parent = currElement;
				currElement->children.push_back(el);
			} else if(lines[i]->type==openTag) {
				FastXmlElement *el = new FastXmlElement();
				el->loadFromLine(lines[i]);
				el->parent = currElement;
				currElement->children.push_back(el);
				currElement = el;
			} else if(lines[i]->type==closeTag) {
				currElement = currElement->parent;
			} else if(lines[i]->type==dataTag) {
				currElement->data = lines[i]->str;
			}
		}
		
		for(int i = 0; i < lines.size(); i++) {
			delete lines[i];
		}
		//root->print();
		delete [] tagBuffer;
		loaded = true;
		return true;
		
	}
