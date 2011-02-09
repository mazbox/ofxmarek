/*
 *  GuiImageButton.h
 *  Gui
 *
 *  Created by Marek Bereza on 13/02/2010.
 *
 */

#pragma once

#include "GuiControl.h"
class GuiImageButton: public GuiControl {
public:
	
	string upImgUrl;
	string overImgUrl;
	string downImgUrl;
	
	ofImage *upImg;
	ofImage *overImg;
	ofImage *downImg;
	
	bool blank;
	
	
	void load() {
		scalable = false;
		upImg = resources->getImage(upImgUrl);
		overImg = resources->getImage(downImgUrl);
		downImg = resources->getImage(overImgUrl);
		width = 0;
		if(upImg->getWidth()>0) {
			width = upImg->getWidth();
			height = upImg->getHeight();
		} else if(downImg->getWidth()>0) {
			width = downImg->getWidth();
			height = downImg->getHeight();
		} else if(overImg->getWidth()>0) {
			width = overImg->getWidth();
			height = overImg->getHeight();
		}
		
		blank = (width==0);
		if(blank) width = height = 20;
	}
	
	
	
	void draw() {
		ofSetColor(255, 255, 255);
		if(blank || (editing && upImg->getWidth()==0)) {
			ofNoFill();
			ofRect(x, y, width, height);
			ofFill();
		} else {
			if(down) downImg->draw(x,y,width,height);
			else if (over) overImg->draw(x,y,width,height);
			else upImg->draw(x,y,width,height);
		}
	}
	
	vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("up image", "upImg","textfield",&upImgUrl));
		params.push_back(ParameterInfo("down image", "downImg","textfield",&downImgUrl));
		params.push_back(ParameterInfo("over image", "overImg","textfield",&overImgUrl));
		return params;
	}
};