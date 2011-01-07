#ifndef __OFXWEBSIMPLEGUITOO__
#define __OFXWEBSIMPLEGUITOO__

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxWebServer.h"

class ofxWebSimpleGuiToo: public ofxSimpleGuiToo, public ofxWSRequestHandler {
public:
	ofxWebSimpleGuiToo();
	void startServer(int port = 8910);
	void stopServer();
	
	void httpGet(string url);

private:
	ofxWebServer server;
	string describePage(ofxSimpleGuiPage *page);
	string describeControl(ofxSimpleGuiControl *control);
	ofxSimpleGuiControl *findControlByName(string name);
	
	void replaceAll(string &str, const string &find_what, const string &replace_with);


};
extern ofxWebSimpleGuiToo webGui;
#endif // __OFXWEBSIMPLEGUITOO__
