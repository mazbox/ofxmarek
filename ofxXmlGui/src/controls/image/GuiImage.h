/*
 *  GuiImage.h
 *  Gui
 *
 *  Created by Marek Bereza on 12/02/2010.
 *
 */
#include "GuiResources.h"

class GuiImage: public GuiControl {

public:
	string imgUrl;
	ofImage *img;

	// if there's no image, this is true
	bool blank;
	

	
	void load() {
		scalable = false;
		img = resources->getImage(imgUrl);
		width = img->getWidth();
		height = img->getHeight();
		blank = (width==0 || height==0);
		if(blank) width = height = 20;
	}
	
	// load in custom vars, then call setup, then load in defaults
	
	void draw() {
		ofSetColor(255, 255, 255);
		if(blank) ofRect(x, y, width, height);
		else img->draw(x, y, width, height);
	}
	
	vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		params.push_back(ParameterInfo("Image", "image","textfield",&imgUrl));
		return params;
	}
};