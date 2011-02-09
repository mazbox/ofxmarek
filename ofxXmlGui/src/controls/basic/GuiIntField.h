/*
 *  GuiIntField.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */


#pragma once

#include "GuiControl.h"
class GuiIntField: public GuiControl {
public:
	
	void setup() {
		
		height = 20;
		width = 70;
		value = new int[1];
		ival(value) = 0;
	}
	
	void draw() {
		
		ofSetColor(50, 50, 50);
		ofRect(x, y, width, height);
		ofNoFill();
		ofRect(x, y, width, height);
		ofFill();
		ofSetColor(255, 255, 255);
		if(focus && !editing && ((int)round(ofGetElapsedTimef()*2.5))%2==0) {
			resources->getDefaultFont()->drawString(ofToString(ival(value)) + "|", x+3, y+14);
		} else {
			resources->getDefaultFont()->drawString(ofToString(ival(value)), x+3, y+14);
		}
		resources->getDefaultFont()->drawString(name, x+3, y);
		
		
	}
	bool keyPressed(int key) {
		if(key>='0' && key<='9') { // if it's a printable char
			ival(value) *= 10;
			ival(value) += key - '0';
			
			return true;
		} else if(key==8 || key==127) { // delete
			ival(value) /= 10;
			return true;
		} else if(key==10 || key==13) {
			// this is a return
			return true;
		}
		return false;
	}
	
	virtual vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("Value","value", "intfield", value));
		return params;
	}
	
	string valueToString() { return ofToString(ival(value)); }
	void valueFromString(string inp) { ival(value) = atoi(inp.c_str()); }
};
