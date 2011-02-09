/*
 *  GuiColorPicker.h
 *  Gui
 *
 *  Created by Marek Bereza on 12/04/2010.
 *
 */

#pragma once

#include "GuiControl.h"
class GuiColorPicker: public GuiControl {
public:
	
	void setup() {
		
		height = 55;
		width = 70;
		value = new int[1];
		ival(value) = 0;
	}
	
	
	
	void draw() {
		
		ofSetHexColor(ival(value));
		ofRect(x, y, width, height);
		
		float h3 = height/3;
		ofSetColor(255, 255, 255);
		ofNoFill();
		ofRect(x, y, width, height);
		ofLine(x, y + h3,  x+width, y + h3);
		ofLine(x, y + h3*2,  x+width, y + h3*2);
		
		
		float c = ((ival(value)&0xFF0000)>>16);
		c /= 255.f;
		ofLine(x + width*c, y, x + width*c, y+h3);
		
		
		c = ((ival(value)&0xFF00)>>8);
		c /= 255.f;
		ofLine(x + width*c, y+h3, x + width*c, y+h3*2);
		
		
		c = ival(value)&0xFF;
		c /= 255.f;
		ofLine(x + width*c, y+h3*2, x + width*c, y+height);
		
		ofFill();
		
		resources->getDefaultFont()->drawString("red", x+3, y+h3-5);
		resources->getDefaultFont()->drawString("green", x+3, y+h3*2 - 5);
		resources->getDefaultFont()->drawString("blue", x+3, y+height - 5);
		resources->getDefaultFont()->drawString(name, x+3, y);
	}
	
	
	bool touchDown(int _x, int _y, int touchId) {
		if(_y-y<height/3) { // r value
			ival(value) &= 0x0000FFFF;
			int red = (255.f*(_x-x)/width);
			ival(value) += red << 16;
		} else if(_y-y<(height*2)/3) { // g value
			ival(value) &= 0x00FF00FF;
			int green = (255.f*(_x-x)/width);
			ival(value) += green << 8;
		} else { // b value
			ival(value) &= 0x00FFFF00;
			int blue = (255.f*(_x-x)/width);
			ival(value) += blue;
		}
		
		return true;
	}
	
	
	
	bool touchMoved(int _x, int _y, int touchId) {
		touchDown(_x, _y, touchId);
		return true;
	}
	
	
	
	virtual vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("Value","value", "intfield", value));
		return params;
	}
	
	string valueToString() { return ofToString(ival(value)); }
	void valueFromString(string inp) { ival(value) = atoi(inp.c_str()); }
};