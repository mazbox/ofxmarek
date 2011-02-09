/*
 *  GuiSegmentedControl.h
 *  Gui
 *
 *  Created by Marek Bereza on 13/02/2010.
 *
 */
#include "GuiControl.h"
#include "GuiButton.h"
#include "StringUtils.h"


class GuiSegmentedControl: public GuiControl {
public:

	
	string options;

	vector<string> opts;
	
	void setup() {
		value = new int[1];
		ival(value) = 0;
		height = 20;
	}
	
	void load() {
		opts = split(options, "|");
		//height = 20;
		if(value==NULL) {
			value = new int[1];
			ival(value) = 0;
		}
		
	}
	void addOption(string opt) {
		if(options=="") options = opt;
		else options += string("|") + opt;
		opts = split(options, "|");
	}
	void draw() {
		ofSetColor(255,255,255);
		resources->getDefaultFont()->drawString(name, x, y);
		int size = opts.size();
		if(size==0) {
			ofSetColor(100, 100, 100);
			ofRect(x,y,width,height);
		}
		for(int i = 0; i < size; i++) {
			if(i==ival(value)) {
				ofSetColor(200, 0, 0);
				ofRect(x+(width/size)*i, y, (width/size)-3, height);
			} else {
				ofSetHexColor(0x444444);
				ofRect(x+(width/size)*i, y, (width/size)-3, height);
			}
			
			ofSetHexColor(0xFFFFFF);
			resources->getDefaultFont()->drawString(opts[i], x+(width/size)*i+3, y+height-3);
		}
	}
	
	vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("Options", "options","textfield",&options));
		params.push_back(ParameterInfo("Value", "value","intfield",value));
		return params;
	}

	
	bool touchDown(int _x, int _y, int touchId) {
		ival(value) = (int)((float)opts.size()*(_x-x)/width);
		return true;
	}
	bool touchMoved(int _x, int _y, int touchId) {
		ival(value) = (int)((float)opts.size()*(_x-x)/width);
		return true;
	}
	
	string valueToString() {
		return ofToString(ival(value));
	}
	
	void valueFromString(string inp) {
		ival(value) = atoi(inp.c_str());
	}
};