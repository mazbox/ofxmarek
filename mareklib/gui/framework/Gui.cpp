/**
 * Gui.cpp
 * emptyExample
 *
 * Created by Marek Bereza on 19/06/2011.
 *
 */
#include "Gui.h"
#include "ofxXmlSettings.h"
#include "Instantiator.h"
gui::Gui::Gui(): gui::Container() {
	root = NULL;
}
bool gui::Gui::loadFromXml(string file) {
	if(root!=NULL) {
		removeChild(root);
		delete root;
		root = NULL;
	}
	
	if(file=="") file = fileName;
	else fileName = file;

	TiXmlDocument doc(ofToDataPath(file));
	if(!doc.LoadFile()) return false;
	TiXmlElement* rootElement = doc.FirstChildElement()->ToElement();
	
	// find out what the root tag is
	string firstTagName = rootElement->Value();
	
	
	root = (Container*)INSTANTIATE(firstTagName);
	
	
	
	// this should recurse, calling createControl as it goes.
	root->loadFromXmlObject(rootElement);
	
	addChild(root);
	return true;
}

void gui::Gui::saveToXml(string file) {
	//if(file=="") file = fileName;
	//else fileName = file;
	
//	printf("Gonna save to %s\n", file.c_str());
	//root->saveToXml(file);

	if(children.size()>0) {
		if(children[0]->isContainer()) {
			((Container*)children[0])->saveToXml(file);
		}
	}
//	Container::saveToXml(file);
}