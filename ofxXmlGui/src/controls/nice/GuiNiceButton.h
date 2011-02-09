/*
 *  GuiNiceButton.h
 *  Gui
 *
 *  Created by Marek Bereza on 13/02/2010.
 *
 */
#include "GuiImageButton.h"
class GuiNiceButton: public GuiImageButton {
public:
	ofTrueTypeFont *font;
	int textXOffset;
	void load() {
		scalable = false;
		upImg = resources->getImage("lib/img/nice/buttonUp.png");
		overImg = resources->getImage("lib/img/nice/buttonOver.png");
		downImg = resources->getImage("lib/img/nice/buttonDown.png");
		font = resources->getFont("lib/font/verdana.ttf", 7);
		width = upImg->getWidth();
		height = upImg->getHeight();
		textXOffset = width/2 - font->stringWidth(name)/2;
	}
	void draw() {
		ofSetColor(255,255,255);
		if(down) downImg->draw(x, y);
		else if(over) overImg->draw(x, y);
		else upImg->draw(x,y);
		if(!down) ofSetColor(0,0,0);
		font->drawString(name, x+textXOffset, y+13);
	}
	
	vector<ParameterInfo> getParameterInfo() {
		vector<ParameterInfo> params;
		return params;
	}
};