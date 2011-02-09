/*
 *  GuiSlider.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */

#pragma once

#include "GuiControl.h"

class GuiSlider: public GuiControl {
public:
	
	bool vertical;
	float min;
	float max;
	int bgColor;
	int fgColor;
	bool stepped;
	bool showValue;
	
	string sliderBGUrl;
	string sliderFGUrl;
	string sliderHandleUrl;
	ofImage *sliderBG, *sliderFG, *sliderHandle;
	void setup() {
		vertical = false;
		height = 20;
		width = 100;
		stepped = false;
		value = new float[1];
		fval(value) = 0.5;
		min = 0;
		max = 1;
		bgColor = 0x505050;
		fgColor = 0x960000;
		sliderBGUrl = "";
		sliderFGUrl = "";
		sliderHandleUrl = "";
		showValue = false;
	}
	
	void load() {
		if(fval(value)<min) fval(value) = min;
		if(fval(value)>max) fval(value) = max;
		sliderBG = resources->getImage(sliderBGUrl);
		sliderFG = resources->getImage(sliderFGUrl);
		sliderHandle = resources->getImage(sliderHandleUrl);
		if(sliderBG!=NULL) {
			scalable = false;
			width = sliderBG->getWidth();
			height = sliderBG->getHeight();
		}
		
	}
	void draw() {
		
		if(sliderBG!=NULL) {
			ofSetHexColor(0xFFFFFF);
			sliderBG->draw(x, y);
		} else {
			ofSetHexColor(bgColor);
			ofRect(x, y, width, height);
		}
		
		float val = (fval(value)-min)/(max-min);
		
		if(sliderFG!=NULL) {
			ofSetHexColor(0xFFFFFF);
			ofPoint abs = getAbsoluteCoords();
			if(vertical) maskOn(abs.x, abs.y + height-height*val, width, height*val);
			else maskOn(abs.x, abs.y, width*val, height);
			sliderFG->draw(x, y);
			//ofRect(0, 0, ofGetWidth(), ofGetHeight());
			maskOff();
		} else {
			ofSetHexColor(fgColor);
			
			if(vertical) ofRect(x, y+height-height*val, width, height*val);
			else ofRect(x, y, width*val, height);
		}
		
		if(sliderHandle!=NULL) {
			ofSetHexColor(0xFFFFFF);
			if(vertical) {
				
				sliderHandle->draw(x, y + (1.f - val)*(height - sliderHandle->getHeight()));
			} else {
				sliderHandle->draw(x+val*(width-sliderHandle->getWidth()), y);
			}
		}
		ofSetColor(255, 255, 255);
		drawLabel();
	}
	
	void drawLabel() {
		if(showValue) {
			if(stepped) {
				resources->getDefaultFont()->drawString(name + "   " + ofToString((int)round(fval(value))), x, y);
			} else {
				resources->getDefaultFont()->drawString(name + "   " + ofToString(fval(value), 3), x, y);
			}
		} else {
			resources->getDefaultFont()->drawString(name, x, y);
		}
	}
	
	bool touchDown(int _x, int _y, int touchId) {
		
		if(vertical) {
			float val = 1 - (_y-y)/height;
			fval(value) = val*(max-min) + min;
			if(stepped) {
				fval(value) = round(fval(value));
			}
		} else {
			float val = (_x-x)/width;
			fval(value) = val*(max-min) + min;
			if(stepped) {
				fval(value) = round(fval(value));
			}
		}
		return true;
	}
	
	bool touchMoved(int _x, int _y, int touchId) {
		touchDown(_x, _y, touchId);
		return true;
	}
	
	vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("Vertical", "vertical", "toggle", &vertical));
		params.push_back(ParameterInfo("Min", "min", "floatfield", &min));
		params.push_back(ParameterInfo("Max", "max", "floatfield", &max));
		params.push_back(ParameterInfo("Stepped", "stepped", "toggle", &stepped));
		params.push_back(ParameterInfo("Value", "value", "floatfield", value));
		params.push_back(ParameterInfo("Show Value", "showvalue", "toggle", &showValue));
		params.push_back(ParameterInfo("BG Color", "bgColor", "colorpicker", &bgColor));
		params.push_back(ParameterInfo("Slider Color", "fgColor", "colorpicker", &fgColor));
		
		params.push_back(ParameterInfo("Slider BG", "sliderBGUrl", "file", &sliderBGUrl));
		params.push_back(ParameterInfo("Slider FG", "sliderFGUrl", "file", &sliderFGUrl));
		params.push_back(ParameterInfo("Slider Handle", "sliderHandleUrl", "file", &sliderHandleUrl));
		
		return params;
	}
	
	string valueToString() {
		return ofToString(fval(value), 9);
	}
	
	void valueFromString(string inp) {
		fval(value) = atof(inp.c_str());
		//printf("Got value from string %s: %f\n", controlId.c_str(), fval(value));
	}
};