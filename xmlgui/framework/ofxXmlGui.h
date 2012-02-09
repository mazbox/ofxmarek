/**
 * ofxXmlGui.h
 * emptyExample
 *
 * Created by Marek Bereza on 16/06/2011.
 *
 */
#include "ofGuiEventDispatcher.h"
#include "Gui.h"

class ofxXmlGui: public xmlgui::ofGuiEventDispatcher, public xmlgui::Gui {
public:
	ofxXmlGui(): xmlgui::ofGuiEventDispatcher() {}
	virtual bool loadFromXml(string path) {
		bool success = Gui::loadFromXml(path);
		setup(this);
		printf("Loaded %s\n", this->id.c_str());
		return success;
	}

};