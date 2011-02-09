/*
 *  GuiFloatField.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */


#pragma once

#include "GuiControl.h"
class GuiFloatField: public GuiControl {
public:
	
	void setup() {
		
		height = 20;
		width = 70;
		value = new float[1];
		fval(value) = 0;
	}
	

	
	void draw() {
		
		ofSetColor(50, 50, 50);
		ofRect(x, y, width, height);
		ofNoFill();
		ofRect(x, y, width, height);
		ofFill();
		ofSetColor(255, 255, 255);
		if(focus && !editing && ((int)round(ofGetElapsedTimef()*2.5))%2==0) {
			resources->getDefaultFont()->drawString(ofToString(fval(value),3) + "|", x+3, y+14);
		} else {
			resources->getDefaultFont()->drawString(ofToString(fval(value),3), x+3, y+14);
		}
		resources->getDefaultFont()->drawString(name, x+3, y);
		
		
	}
	bool keyPressed(int key) {
		if(key>='0' && key<='9') { // if it's a printable char
			fval(value) *= 10;
			fval(value) += key - '0';
			
			return true;
		} else if(key==8 || key==127) { // delete
			fval(value) /= 10;
			if(ABS(fval(value))<0.01) fval(value) = 0;
			return true;
		} else if(key==10 || key==13) {
			// this is a return
			return true;
		}
		return false;
	}
	
	virtual vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("Value","value", "floatfield", value));
		return params;
	}
	
	string valueToString() { return ofToString(fval(value)); }
	void valueFromString(string inp) { fval(value) = atof(inp.c_str()); }
};
