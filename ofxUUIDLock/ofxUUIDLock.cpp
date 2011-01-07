
#include "ofxUUIDLock.h"

string ofxGetUUID() {	
	char path[512];
	
	//	FILE *fp = popen("system_profiler SPHardwareDataType |grep \"Hardware UUID\"", "r");
	FILE *fp = popen("system_profiler SPHardwareDataType", "r");
	if (fp == NULL) {
		printf("Error authenticating\n");
		exit(1);
	}
	
	while (fgets(path, 512, fp) != NULL) {
		string s = string(path);
    	size_t pos = s.find("Hardware UUID");
    	if(pos!=string::npos) {
    		s = s.substr(pos+15); // remove preceding stuff
    		size_t carriageReturn = s.find("\n");
    		if(carriageReturn!=string::npos) {
	    		s = s.substr(0, carriageReturn);
	    	}
			return s;
    		break;
    	}
	}
	pclose(fp);
	return "";
}



void ofxCheckUUID() {
	//printf("%s\n", ofxGetUUID().c_str());
	string uuid = UUID_STRING;
	string myUUID = ofxGetUUID();
	if(uuid!=myUUID) {
				//printf("%s != %s\n", uuid.c_str(), myUUID.c_str());
		exit(1);
	}
}
