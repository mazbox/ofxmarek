#pragma once
#include "json.h"
#include "ofMain.h"

using namespace std;

#define OFXJSON_NICE 0
#define OFXJSON_CONDENSED 1

class ofxJSON: public Json::Value {
public:
	
	ofxJSON(): Json::Value() {}
	
	ofxJSON(string jsonStr) {
		parse(jsonStr);
	}
	ofxJSON(const Json::Value &other): Json::Value(other) {}
	
	ofxJSON(string a, string b): Json::Value() {
		(*this)[a.c_str()] = b.c_str();

	}
	
	ofxJSON(string a, Json::Value b): Json::Value() {
		(*this)[a.c_str()] = b;
	}
	
	void parse(string &jsonStr) {
		clear();
		Json::Reader reader;
		reader.parse(jsonStr, *this);
	}
	
	
	ofxJSON(vector<string> &array) {
		ostringstream toParse;
		toParse << "[";
		for(int i = 0; i < array.size(); i++) {
			toParse << "\""  <<array[i] << "\"";
			if(i<array.size()-1) toParse << ",";
		}
		toParse << "]";
		string output = toParse.str();
		parse(output);
		
	}
		
	string str(int type = OFXJSON_CONDENSED) {
		if(type==OFXJSON_CONDENSED) {
			Json::FastWriter writer;
			return writer.write(*this);
		} else {
			Json::StyledWriter writer;
			return writer.write(*this);
		}
	}
	
	bool loadFromFile(string path) {
		path = ofToDataPath(path, false);
		ifstream ifs(path.c_str());
		if(!ifs || !ifs.is_open()) {
			cout << "ofxJSON::loadFromFile(): Can't open file: " << path << endl;
			return false;
		}
		
		std::string json((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		clear();
		parse(json);
		return true;
		
	}
	
	vector<string> getKeys() {
		return getMemberNames();
	}
	
	bool saveToFile(string path) {
		
		
		path = ofToDataPath(path, false);
		
		
		ofstream ofs(path.c_str(), ios::out);
		if(!ofs) {
			cout << "ofxJSON::saveToFile(): Can't open file: " << path << endl;
			return false;
		}
		ofs << str(OFXJSON_NICE);
		ofs.close();

		return true;
	}

};