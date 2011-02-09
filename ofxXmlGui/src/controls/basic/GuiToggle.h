/*
 *  GuiToggle.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */
#pragma once

#include "GuiControl.h"
class GuiToggle: public GuiControl {
public:
	
	
	int offColor;
	int onColor;
	
	ofImage *toggleOn;
	ofImage *toggleOff;
	ofImage *toggleDown;
	
	string toggleOnUrl;
	string toggleOffUrl;
	string toggleDownUrl;
	
	
	void setup() {
		offColor = 0x505050;
		onColor = 0x995050;
		height = 20;
		width = 70;
		if(value==NULL) {
			value = new bool[1];
			bval(value) = false;
		}
		toggleOn = toggleOff = toggleDown = NULL;
		toggleOnUrl = toggleOffUrl = toggleDownUrl = "";
	}
	
	void load() {
		toggleOn = resources->getImage(toggleOnUrl);
		toggleOff = resources->getImage(toggleOffUrl);
		toggleDown = resources->getImage(toggleDownUrl);
		if(toggleOn!=NULL) {
			scalable = false;
			width = toggleOn->getWidth();
			height = toggleOn->getHeight();
		} else if(toggleOff!=NULL) {
			scalable = false;
			width = toggleOff->getWidth();
			height = toggleOff->getHeight();
		} else if(toggleDown!=NULL) {
			scalable = false;
			width = toggleDown->getWidth();
			height = toggleDown->getHeight();
		}
	}
	
	void draw() {
		

		if(bval(value)) {
			if(toggleDown!=NULL && down) {
				ofSetHexColor(0xFFFFFF);
				toggleDown->draw(x, y);
			} else if(toggleOn && !down) {
				ofSetHexColor(0xFFFFFF);
				toggleOn->draw(x, y);
			} else {
				ofSetHexColor(onColor);
				ofRect(x, y, width, height);
			}
		} else {
			if(toggleDown!=NULL && down) {
				ofSetHexColor(0xFFFFFF);
				toggleDown->draw(x, y);
			
			} else if(toggleOff!=NULL) {
				ofSetHexColor(0xFFFFFF);
				toggleOff->draw(x, y);
			} else {
				//if (over) {
					ofSetHexColor(offColor);
					ofRect(x, y, width, height);
				//} else {
				//	ofSetColor(offColor);
				//	ofRect(x, y, width, height);
				//}
			}
		}
		
		
		ofSetColor(255, 255, 255);
		resources->getDefaultFont()->drawString(name, x+3, y+14);
	}
	bool touchDown(int _x, int _y, int button) {
		bval(value) ^= true;
		return true;
	}
	
	virtual vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("Value","value", "toggle",value));
		
		params.push_back(ParameterInfo("Off Color", "offColor", "colorpicker", &offColor));
		params.push_back(ParameterInfo("On Color", "onColor", "colorpicker", &onColor));
		params.push_back(ParameterInfo("Toggle On", "toggleOnUrl", "file", &toggleOnUrl));
		params.push_back(ParameterInfo("Toggle Off", "toggleOffUrl", "file", &toggleOffUrl));
		params.push_back(ParameterInfo("Toggle Down", "toggleDownUrl", "file", &toggleDownUrl));
		
		return params;
	}
	
	string valueToString() { return bval(value)?"true":"false"; }	
	void valueFromString(string inp) { bval(value) = (inp=="true"); }

};