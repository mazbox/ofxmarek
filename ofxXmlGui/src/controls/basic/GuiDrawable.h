/**
 * GuiDrawable.h
 * KinectTracker
 *
 * Created by Marek Bereza on 08/02/2011.
 *
 */
#include "GuiControl.h"

class GuiDrawable: public GuiControl {
public:
	ofBaseDraws *drawable;
	void setup() {
		drawable = NULL;
	}
	
	void draw() {
		if(drawable!=NULL) {
			drawable->draw(x, y, width, height);
		}
	}
};