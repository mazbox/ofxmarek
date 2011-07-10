/*
 *  GuiTitle.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 17/01/2011.
 *
 */

class Title: public gui::Control {
public:
	
	
	Title(): Control() {
		height = 24;
		width = 110;
	}
	
	
	
	void draw() {
		ofSetHexColor(0xFFFFFF);
		gui::Resources::drawString(name, x+3, y+14);
		ofLine(x, y+height-5, x+width, y+height-5);
	}
};