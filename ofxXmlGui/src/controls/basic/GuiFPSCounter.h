/*
 *  GuiFPSCounter.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */

class GuiFPSCounter: public GuiControl {
public:
	
	
	void setup() {
		height = 25;
		width = 60;
	}
	
	void draw() {
		ofSetColor(0, 100, 0);
		ofRect(x, y, width, height);
		ofSetColor(255, 255, 255);
		resources->getDefaultFont()->drawString(ofToString(ofGetFrameRate(), 3), x+3, y+14);
	}
};