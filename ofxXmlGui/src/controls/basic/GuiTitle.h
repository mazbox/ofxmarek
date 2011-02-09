/*
 *  GuiTitle.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 17/01/2011.
 *
 */

class GuiTitle: public GuiControl {
public:
	
	
	void setup() {
		height = 24;
		width = 110;
	}
	
	
	
	void draw() {
		ofSetHexColor(0xFFFFFF);
		resources->getDefaultFont()->drawString(name, x+3, y+14);
		ofLine(x, y+height-5, x+width, y+height-5);
	}
};