#include "ofMain.h"
#include "mareklib.h"
void mareklib::drawFramerate(int color) {
	ofSetHexColor(color);
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight()-20);
}

void mareklib::setDataPathRootToAppContents() {
	char path[512];
	getcwd(path, 512);
	string dataRoot = path;
	dataRoot += "/../data/";
	ofSetDataPathRoot(dataRoot);
}



string padZeros(int t) {
	if(t<10) return ofToString(t);
	else return "0"+ofToString(t);
}
string mareklib::dateTimeString() {
	return padZeros(ofGetDay())
	+ "." + padZeros(ofGetMonth()) + "." + padZeros(ofGetYear()) + " "
	+ padZeros(ofGetHours()) +"-"+padZeros(ofGetMinutes()) +"-"+
	padZeros(ofGetSeconds());
}


string mareklib::getHomeDirectory() {
	FILE *fp = popen("who am I", "r");
	if(fp!=NULL) {
		printf("popen made it\n");
		char name[512];
		string username;
		ofSleepMillis(100);
		if(fgets(name, 512, fp)) {
			printf("fgets\n");
			username = name;
			if(username.find(' ')!=-1) {
				username = username.substr(0, username.find(' '));
				string home = "/Users/"+username;
				return home;
			}
			
		}
		pclose(fp);
	} else {
		printf("Couldn't find user's name, going with default\n");
	}
	return "";
}

string mareklib::getDesktopPath() {
	return getHomeDirectory() + "/Desktop";
}

string mareklib::getPreferencesDirectory(string appName) {
	string prefsDir = getHomeDirectory() + "/Library/Preferences/"+appName;
	struct stat stFileInfo; 
	
	// Attempt to get the file attributes 
	if(stat(prefsDir.c_str(),&stFileInfo)!=0) { 
	
		if(mkdir(prefsDir.c_str(), 0777)==0) {
			return prefsDir;
		} else {
			printf("Failed to create preferences directory: %s\n", prefsDir.c_str());
			return "";
		}
	}
}