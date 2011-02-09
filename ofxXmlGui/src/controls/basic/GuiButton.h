/*
 *  GuiButton.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */
#pragma once

#include "GuiControl.h"
class GuiButton: public GuiControl {
public:

	string upImgUrl;
	string downImgUrl;
	ofImage *upImg, *downImg;
	void setup() {
		height = 20;
		width = 110;
		upImg = downImg = NULL;
		upImgUrl = downImgUrl = "";
	}
	
	void load() {
		upImg = resources->getImage(upImgUrl);
		downImg = resources->getImage(downImgUrl);
		if(upImg!=NULL) {
			scalable = false;
			width = upImg->getWidth();
			height = upImg->getHeight();
		} else if(downImg!=NULL) {
			scalable = false;
			width = downImg->getWidth();
			height = downImg->getHeight();
		}
	}
	
	
	void draw() {
		
		if(down) {
			if(downImg!=NULL) {
				ofSetHexColor(0xFFFFFF);
				downImg->draw(x, y);
			} else {
				ofSetColor(0, 0, 0);
				ofRect(x, y, width, height);
			}
		} else if(over && upImg==NULL && downImg==NULL) { // draw grey bg for nice menus
			ofSetHexColor(0xCCCCCC);
			ofRect(x, y, width, height);
		} else {
			ofSetHexColor(0xFFFFFF);
			if(upImg!=NULL) {
				upImg->draw(x, y);
			} else {
				ofRect(x, y, width, height);
			}
		}
		
		if(down) ofSetHexColor(0xFFFFFF);
		else ofSetHexColor(0);
		resources->getDefaultFont()->drawString(name, x+3, y+14);
	}
	
	
	virtual vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("Button Up", "upImgUrl", "file", &upImgUrl));
		params.push_back(ParameterInfo("Button Down", "downImgUrl", "file", &downImgUrl));
		
		return params;
	}
};