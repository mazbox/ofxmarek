/*
 *  GuiTextField.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */

#pragma once

#include "GuiControl.h"
class GuiTextField: public GuiControl {
public:

	void setup() {
		
		height = 20;
		width = 70;
		value = new string;
	}
	
	
	void draw() {
		
		ofSetColor(50, 50, 50);
		ofRect(x, y, width, height);
		ofNoFill();
		ofRect(x, y, width, height);
		ofFill();
		ofSetColor(255, 255, 255);
		if(focus && !editing && ((int)round(ofGetElapsedTimef()*2.5))%2==0) {
			resources->getDefaultFont()->drawString(sval(value) + "|", x+3, y+14);
		} else {
			resources->getDefaultFont()->drawString(sval(value), x+3, y+14);
		}
		resources->getDefaultFont()->drawString(name, x+3, y);
		
		
	}
	bool keyPressed(int key) {
		if(key>=32 && key<=126) { // if it's a printable char
			sval(value) += (char*)&key;
			return true;
		} else if((key==8 || key==127) && sval(value).size()>0) { // delete
			sval(value) = sval(value).erase(sval(value).size()-1);
			return true;
		} else if(key==10 || key==13) {
			// this is a return
			return true;
		}
		return false;
	}
	
	vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("Value", "value","textfield",value));
		return params;
	}
	string valueToString() { return sval(value); }
	void valueFromString(string inp) { sval(value) = inp; }
};