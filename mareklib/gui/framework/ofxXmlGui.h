/**
 * ofxXmlGui.h
 * emptyExample
 *
 * Created by Marek Bereza on 16/06/2011.
 *
 */
#include "ofGuiEventDispatcher.h"
#include "Gui.h"

class ofxXmlGui: public gui::ofGuiEventDispatcher, public gui::Gui {
public:
	ofxXmlGui(): gui::ofGuiEventDispatcher() {}
	virtual bool loadFromXml(string path) {
		bool success = Gui::loadFromXml(path);
		setup(this);
		printf("Loaded %s\n", this->id.c_str());
		return success;
	}

};