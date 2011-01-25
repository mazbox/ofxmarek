#include "testApp.h"
ofImage bg;

#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4

//--------------------------------------------------------------
void testApp::setup(){
	inspector.x = menu.x;
	inspector.y = menu.y + menu.height+10;
	inspector.setGui(&gui);
	gui.addListener(&inspector);
	menu.inspector = &inspector;
	menu.setGui(&gui);
	hierarchy.setGui(&gui, &menu);
	gui.width = 320;
	gui.height = 480;
	ofBackground(20, 2, 20);
	bg.loadImage(string("/Library/Desktop Pictures/Abstract/Abstract ") +ofToString((int)round(ofRandom(1,8))) +  ".jpg");
	
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetHexColor(0xFFFFFF);
	bg.draw(0,0, ofGetWidth(), ofGetHeight());
	ofEnableAlphaBlending();
	gui.draw();
	inspector.draw();
	menu.draw();
	hierarchy.draw();
	ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(!menu.keyPressed(key)) {
		if(!inspector.keyPressed(key)) {
			if(!gui.keyPressed(key)) {
				hierarchy.keyPressed(key);
			}
		}
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}



//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	menu.touchOver(x, y, 1);
	gui.touchOver(x, y, 1);
	inspector.touchOver(x, y, 1);
	hierarchy.touchOver(x, y, 1);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if(!menu.touchMoved(x, y, button)) {
		if(!inspector.touchMoved(x, y, button)) {
			if(!gui.touchMoved(x, y, button)) {
				hierarchy.touchMoved(x, y, button);
			}
		}
	}
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(!menu.touchDown(x, y, button)) {
		if(!inspector.touchDown(x, y, button)) {
			if(!gui.touchDown(x, y, button)) {
				hierarchy.touchDown(x, y, button);
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	if(!menu.touchUp(x, y, button)) {
		if(!inspector.touchUp(x, y, button)) {
			if(!gui.touchUp(x, y, button)) {
				hierarchy.touchUp(x, y, button);
			}
		}
	}
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}
