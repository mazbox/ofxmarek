/*
 *  GuiImageToggle.h
 *  Gui
 *
 *  Created by Marek Bereza on 13/02/2010.
 *
 */
#include "GuiToggle.h"
class GuiImageToggle: public GuiToggle {
public:
	
	string offImgUrl;
	string onImgUrl;
	
	ofImage *offImg;
	ofImage *onImg;
	bool blank;
	
	void load() {
		scalable = false;
		offImg = resources->getImage(offImgUrl);
		onImg =  resources->getImage(onImgUrl);
		width = 0;
		if(offImg->getWidth()>0) {
			width = offImg->getWidth();
			height = offImg->getHeight();
		} else if(onImg->getWidth()>0) {
			width = onImg->getWidth();
			height = onImg->getHeight();
		}
		
		blank = (width==0);
		if(blank) width = height = 20;
	}
	
	void draw() {
		ofSetColor(255, 255, 255);
		if(blank || (editing && offImg->getWidth()==0)) {
			ofNoFill();
			ofRect(x, y, width, height);
			ofFill();
		} else {
			if(bval(value)) onImg->draw(x,y,width,height);
			else offImg->draw(x,y,width,height);
		}
	}
	
	virtual vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("on image", "onImg","textfield",&onImgUrl));
		params.push_back(ParameterInfo("off image", "offImg","textfield",&offImgUrl));
		params.push_back(ParameterInfo("Value","value", "toggle",value));
		return params;
	}
};