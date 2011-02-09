/*
 *  GuiDropDown.h
 *  Gui
 *
 *  Created by Marek Bereza on 05/05/2010.
 *
 */

#pragma once

#define SCROLL_NONE 0
#define SCROLL_UP 2
#define SCROLL_DOWN -2

#include "GuiControl.h"

class GuiList: public GuiControl {
public:
	
	int bgColor;
	int fgColor;
	
	int scrollOffset;
	int itemHeight;
	int scrollerWidth;
	int scrolling;
	bool dragging;

	string options;
	void setup() {
		bgColor = 0x333333;
		fgColor = 0xFFFFFF;
		dragging = false;
		scrolling = SCROLL_NONE;
		itemHeight = scrollerWidth = 22;
		scrollOffset = 0;
		value = new int[1];
		ival(value) = -1;
		width = 100;
		height = 100;
		x = y = 0;
	}
	
	void load() {
		items = split(options, "|");
		
	}
	
	void addItem(string tem) {
		items.push_back(tem);
	}
	void clearItems() {
		items.clear();
	}
	
	string getSelectedString() {
		if(ival(value)==-1) return "";
		else return items[ival(value)];
	}
	
	void setSelection(string itemName) {
		for(int i = 0; i < items.size(); i++) {
			if(items[i]==itemName) {
				ival(value) = i;
				return;
			}
		}
		ival(value) = -1;
	}
	void draw() {
		
		scrollOffset += scrolling;
		if(dragging) {
			scrolling = SCROLL_NONE;
		}
		if(scrollOffset>0) scrollOffset = 0;
		
		if(needsScrollbar() && items.size()*itemHeight+scrollOffset<height) {
			scrollOffset = height - items.size()*itemHeight;
		}
		
		ofSetHexColor(bgColor);
		ofRect(x, y, width, height);
		
		glEnable(GL_SCISSOR_TEST);
		
		ofNoFill();
		ofSetHexColor(fgColor);
		ofRect(x, y, width, height);
		
		ofPoint abs = getAbsoluteCoords();
		maskOn(abs.x, abs.y, width, height);
		
		
		
		
		for(int i =0; i < items.size(); i++) {
			ofSetHexColor(fgColor);
			
			
			ofRect(x, scrollOffset + y + i*itemHeight, width-scrollerWidth, itemHeight);
			
			if(i==ival(value)) ofSetHexColor(0x00FF00);
			resources->getDefaultFont()->drawString(items[i], x+5, scrollOffset + y + (i+1)*itemHeight-4);
			
			// only draw as much as we need
			//if((i+1)*itemHeight>height) break;
		}
		ofNoFill();
		if(needsScrollbar()) ofSetHexColor(fgColor);
		else ofSetHexColor(blendColor(fgColor, bgColor, 0.2));
		ofLine(x+width-scrollerWidth, y+height/2, x+width, y+height/2);
		ofLine(x+width-scrollerWidth, y, x+width-scrollerWidth, y+height);
		resources->getDefaultFont()->drawString("UP", 4+x+width-scrollerWidth, y+height/2 - 5);
		resources->getDefaultFont()->drawString("DN", 4+x+width-scrollerWidth, y+height/2 + 15);
		maskOff();
		ofFill();
	}
	
	bool needsScrollbar() {
		return items.size()*itemHeight>height;
	}
	bool touchDown(int _x, int _y, int touchId) {
		if(_x>x+width-scrollerWidth && needsScrollbar()) {
			// scrolling with scroll bars
			float yy = _y - y;
			yy /= height;
			if(yy<0.5) {
				scrolling = SCROLL_UP;
			} else {
				scrolling = SCROLL_DOWN;
			}
			
		} else {
			scrolling = SCROLL_NONE;
			lastMouse = ofPoint(_x, _y);
		}
		return true;
	}
	
	bool touchMoved(int _x, int _y, int touchId) {
		if(_x>x+width-scrollerWidth) {
			
		} else if(needsScrollbar()) {
			dragging = true;
			scrolling = _y - lastMouse.y;
			// do some drag scrolling
			lastMouse = ofPoint(_x, _y);
		}
		return true;
	}
	
	
	bool touchUp(int _x, int _y, int touchId) {
		if(!dragging && _x==lastMouse.x && _y==lastMouse.y) {
			// click!
			float offset = (float)(_y - y - scrollOffset)/itemHeight;
			int i = floor(offset);
			if(i>=0 && i < items.size()) {
				ival(value) = i;

				if(listeners!=NULL) {
					for(int i = 0; i < numListeners; i++) {
						listeners[i]->controlChanged(this);
					}
				}
			}
		} else {
			touchMoved(_x, _y, touchId);
		}
		dragging = false;
		scrolling = SCROLL_NONE;
		return true;
	}
	
	vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		
		params.push_back(ParameterInfo("BG Color", "bgColor", "colorpicker", &bgColor));
		params.push_back(ParameterInfo("FG Color", "fgColor", "colorpicker", &fgColor));
		params.push_back(ParameterInfo("Options", "options","textfield",&options));
		return params;
	}
	
	
	
	ofPoint lastMouse;
	vector<string> items;
};