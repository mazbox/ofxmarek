/**
 * GuiDrawable.h
 * KinectTracker
 *
 * Created by Marek Bereza on 08/02/2011.
 *
 */
#include "Control.h"

class Drawable: public gui::Control {
public:
	ofBaseDraws *drawable;
	Drawable(): Control() {
		drawable = NULL;
	}
	
	void draw() {
		if(drawable!=NULL) {
			drawable->draw(x, y, width, height);
		}
	}
};