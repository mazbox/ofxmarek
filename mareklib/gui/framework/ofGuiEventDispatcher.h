/**
 * RootContainer.h
 * emptyExample
 *
 * Created by Marek Bereza on 16/06/2011.
 *
 */

#pragma once


#include "Container.h"

namespace gui {
	class ofGuiEventDispatcher {
	public:
		ofGuiEventDispatcher();
		void setup(gui::Container *root);
		
		void enableEvents();
		void disableEvents();
		bool isEnabled();
		void setEnabled(bool enabled);
		
		void draw(ofEventArgs &e);	
		
		void mousePressed(ofMouseEventArgs &e);
		void mouseMoved(ofMouseEventArgs &e);	
		void mouseDragged(ofMouseEventArgs &e);
		void mouseReleased(ofMouseEventArgs &e);
		
		void keyPressed(ofKeyEventArgs &e);
		void keyReleased(ofKeyEventArgs &e);
		
		void touchDown(ofTouchEventArgs &e);
		void touchMoved(ofTouchEventArgs &e);
		void touchUp(ofTouchEventArgs &e);
	private:
		gui::Container *root;
		bool enabled;
	};
};
