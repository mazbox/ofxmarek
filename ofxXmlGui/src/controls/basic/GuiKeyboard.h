/*
 *  GuiKeyboard.h
 *  Gui
 *
 *  Created by Marek Bereza on 12/04/2010.
 *
 */

#pragma once

#include "GuiControl.h"
class GuiKeyboard: public GuiControl {
public:
	
	int blackColor;
	int whiteColor;
	int blackActiveColor;
	int whiteActiveColor;
	
	bool vertical;
	int offset;
	int keys;
	void setup() {
		offset = 12;
		keys = 12;
		vertical = false;
		height = 20;
		width = 110;
		blackColor = 0;
		whiteColor = 0xFFFFFF;
		value = new int[1];
		ival(value) = 0;
		blackActiveColor = 0xcc0000;
		whiteActiveColor = 0xcc0000;
	}
	
	
	bool isABlackNote(int note) {
		note %= 12;
		return (note==1 || note==3 || note==6 || note==8 || note==10);
	}
	
	void draw() {
		
		
	
		
		ofSetHexColor(whiteColor);
		ofRect(x, y, width, height);
		ofSetHexColor(blackColor);
		
		
		
		if(vertical) {
			
			
			float keyHeight = (float) height / keys;
			
			
			for(int i = 0; i < keys; i++) {
				if(ival(value)==i+offset || isABlackNote(i+offset)) {
					setColorForNote(i+offset);
					ofRect(x, y+i*keyHeight, width, keyHeight);
					ofSetHexColor(blackColor);
				}
				
				
				ofLine(x, y+i*keyHeight, x+width, y+i*keyHeight);
			}
			
			
		} else {
		
			float keyWidth = (float) width / keys;
			
			
			for(int i = 0; i < keys; i++) {
				
				
				
				if(ival(value)==i+offset || isABlackNote(i+offset)) {
					setColorForNote(i+offset);
					ofRect(x+i*keyWidth, y, keyWidth, height);
					ofSetHexColor(blackColor);
				}
				
				ofLine(x+i*keyWidth, y, x+i*keyWidth, y+height);
			}
		}
	}
	
	
	void setColorForNote(int note) {
		if(isABlackNote(note)) {
			if(ival(value)==note) {
				ofSetHexColor(blackActiveColor);
			} else {
				ofSetHexColor(blackColor);
			}
		} else {
			// active white
			ofSetHexColor(whiteActiveColor);
		}
	}
	
	bool touchDown(int _x, int _y, int touchId) {
		if(vertical) {
			ival(value) = offset + floor((_y-y)*keys/height);
		} else {
			ival(value) = offset + floor((_x-x)*keys/width);
		}
		return true;
	}
	
	bool touchMoved(int _x, int _y, int touchId) {
		return touchDown(_x, _y, touchId);
	}
	
	bool touchUp(int _x, int _y, int touchId) {
		ival(value) = 0;
		return true;
	}
	
	
	
	virtual vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("Black Key Color", "blackColor", "colorpicker", &blackColor));
		params.push_back(ParameterInfo("White Key Color", "whiteColor", "colorpicker", &whiteColor));
		params.push_back(ParameterInfo("Black Active Color", "blackActiveColor", "colorpicker", &blackActiveColor));
		params.push_back(ParameterInfo("White Active Color", "whiteActiveColor", "colorpicker", &whiteActiveColor));
		params.push_back(ParameterInfo("Note Offset", "noteOffset", "intfield", &offset));
		params.push_back(ParameterInfo("Key Count", "numberOfKeys", "intfield", &keys));
		params.push_back(ParameterInfo("Vertical", "vertical", "toggle", &vertical));
		return params;
	}
};