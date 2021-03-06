/*
 *  GuiButton.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */
#pragma once

#include "LabeledControl.h"
class PushButton: public LabeledControl {
public:

	string upImgUrl;
	string downImgUrl;
	ofImage *upImg, *downImg;
	int upColor;
	int downColor;
	int overColor;
	PushButton(): LabeledControl() {
		height = 20;
		width = 110;
		upImg = downImg = NULL;
		upImgUrl = downImgUrl = "";
		upColor = 0xFFFFFF;
		downColor = 0;
		overColor = 0xCCCCCC;
	}
	
	void load() {
		upImg = gui::Resources::getImage(upImgUrl);
		downImg = gui::Resources::getImage(downImgUrl);
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
				setRGBA(downColor);
				ofRect(x, y, width, height);
			}
		} else if(over && upImg==NULL && downImg==NULL) { // draw grey bg for nice menus
			setRGBA(overColor);
			ofRect(x, y, width, height);
		} else {
			setRGBA(upColor);
			if(upImg!=NULL) {
				upImg->draw(x, y);
			} else {
				ofRect(x, y, width, height);
			}
		}
		
		drawLabel(x+3, y+14);
	}
	
	
	virtual void getParameterInfo(vector<ParameterInfo> &params) {
		LabeledControl::getParameterInfo(params);
		params.push_back(ParameterInfo("Button Up", "upImgUrl", "file", &upImgUrl));
		params.push_back(ParameterInfo("Button Down", "downImgUrl", "file", &downImgUrl));
		params.push_back(ParameterInfo("Up Color", "upColor", "colorpicker", &upColor));
		params.push_back(ParameterInfo("Down Color", "downColor", "colorpicker", &downColor));
		params.push_back(ParameterInfo("over Color", "overColor", "colorpicker", &overColor));
		
	}
};