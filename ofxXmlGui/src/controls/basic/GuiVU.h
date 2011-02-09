/*
 *  GuiVU.h
 *  Gui
 *
 *  Created by Marek Bereza on 05/05/2010.
 *
 */



#pragma once

#include "GuiControl.h"

class GuiVU: public GuiControl {
public:
	
	int bgColor;
	int fgColor;

	float minAngle;
	float maxAngle;
	string bgImageUrl, needleImageUrl;
	ofImage *bgImage, *needleImage;
	
	int pivotX, pivotY;
	int radius;
	void setup() {


		height = 80;
		width = 80;


		pivotX = width/2;
		pivotY = height;
		radius = height;
		
		value = new float[1];
		fval(value) = 0.0;


		bgColor = 0x505050;
		fgColor = 0x960000;
		maxAngle = 4.f;
		minAngle = 5.5f;
		bgImageUrl = needleImageUrl = "";
		bgImage = needleImage = NULL;
	}
	
	void load() {

				
		bgImage = resources->getImage(bgImageUrl);
		needleImage = resources->getImage(needleImageUrl);
		if(needleImage!=NULL) {
			needleImage->setAnchorPercent(0.5, 0.5);
		}
		if(bgImage!=NULL) {
			scalable = false;
			width = bgImage->getWidth();
			height = bgImage->getHeight();
		}		
		
	}
	
	
	
	void draw() {
		fval(value) = 0.5 + 0.5*sin(ofGetElapsedTimef());
		
		if(bgImage!=NULL) {
			ofSetHexColor(0xFFFFFF);
			bgImage->draw(x,y);
		} else {
			ofSetHexColor(bgColor);	
			ofRect(x, y, width, height);
		}
		

		
		
		float angle = fval(value);
		
		angle *= (maxAngle - minAngle);
		angle += minAngle;
		
		
		if(needleImage!=NULL) {
			glPushMatrix();
			glTranslatef(x + pivotX, y+pivotY, 0);
			glRotatef(angle*180.f/PI, 0, 0, 1);
			ofSetHexColor(0xFFFFFF);
			needleImage->draw(0,0);
			glPopMatrix();
		} else {
			ofSetHexColor(fgColor);
			ofLine(x+pivotX, y+pivotY, x+pivotX + radius*cos(angle), y+pivotY + radius*sin(angle));	
			ofSetColor(255, 255, 255);
		}
		resources->getDefaultFont()->drawString(name, x, y);
	}
	
	
	
	
	
	vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;





		params.push_back(ParameterInfo("Min Angle", "minAngle", "floatfield", &minAngle));
		params.push_back(ParameterInfo("Max Angle", "maxAngle", "floatfield", &maxAngle));
		params.push_back(ParameterInfo("Pivot X", "pivotX", "intfield", &pivotX));
		params.push_back(ParameterInfo("Pivot Y", "pivotY", "intfield", &pivotY));
		params.push_back(ParameterInfo("Radius", "radius", "intfield", &radius));
		params.push_back(ParameterInfo("BG Color", "bgColor", "colorpicker", &bgColor));
		params.push_back(ParameterInfo("Dial Color", "fgColor", "colorpicker", &fgColor));
		params.push_back(ParameterInfo("Background Image", "bgImage", "file", &bgImageUrl));
		params.push_back(ParameterInfo("Needle Image", "needleImage", "file", &needleImageUrl));
		return params;
	}
	
	string valueToString() {
		return ofToString(fval(value), 9);
	}
	
	void valueFromString(string inp) {
		fval(value) = atof(inp.c_str());
	}
};