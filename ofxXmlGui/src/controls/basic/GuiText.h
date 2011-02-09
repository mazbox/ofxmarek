/*
 *  GuiText.h
 *  Gui
 *
 *  Created by Marek Bereza on 28/03/2010.
 *
 */


#pragma once

#include "GuiControl.h"
class GuiText: public GuiControl {
public:
	
	
	void setup() {
		
		height = 20;
		width = 70;
		value = new string;
	}

	
	void draw() {
		
		ofSetColor(255, 255, 255);
		resources->getDefaultFont()->drawString(sval(value), x+3, y+14);
	}
	
	vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("Value", "value","text",value));
		return params;
	}
	
	string valueToString() { return sval(value); }
	void valueFromString(string inp) { sval(value) = inp; }
};