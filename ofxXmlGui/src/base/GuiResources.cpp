/*
 *  GuiResources.cpp
 *  Gui
 *
 *  Created by Marek Bereza on 12/02/2010.
 *
 */

#include "GuiResources.h"

#include <sys/stat.h>

ofTrueTypeFont emptyFont; 
GuiResources GuiResources::globals;
GuiResources::GuiResources() {
	defaultFont = NULL;
}

void GuiResources::init(string xmlPath) {
	int lastSlash = xmlPath.rfind("/");
	if(lastSlash!=-1) {
		basePath = xmlPath.substr(0, lastSlash+1);
	} else {
		basePath = "./";
		printf("GuiResources: could not set basePath for %s, trying ./\n", xmlPath.c_str());
	}
} 




ofTrueTypeFont *GuiResources::getDefaultFont() {
	if(defaultFont==NULL) {
		defaultFont = getFont(DEFAULT_FONT);
	}
	return defaultFont;
}



bool guiResFileExists(string filename) { 
	struct stat stFileInfo; 
	
	// Attempt to get the file attributes 
	return stat(filename.c_str(),&stFileInfo)==0; 
}


ofImage *GuiResources::getImage(string path) {
	if(path=="") return NULL;
	
	if(!guiResFileExists(ofToDataPath(basePath+path, true))) {
		return NULL;
	}
	if(path.find("global")==0) {
		globals.init("global/gui.xml");
		path = path.substr(7);
		return globals.getImage(path);
	}
	// cache the image if it's not already loaded
	if(images.find(path)==images.end()) {
		ofImage *img = new ofImage();
		img->loadImage(basePath+path);
		images[path] = img;
		printf("Loaded file from %s\n", string(basePath+path).c_str());
	}
	
	return images[path];
}

ofTrueTypeFont *GuiResources::getFont(string path, int size) {
	if(path=="") return &emptyFont;
	
	if(path.find("global")==0) {
		globals.init("global/gui.xml");
		path = path.substr(7);
		return globals.getFont(path, size);
	}
	
	// cache the image if it's not already loaded
	if(fonts.find(path+ofToString(size))==fonts.end()) {
		ofTrueTypeFont *font = new ofTrueTypeFont();
		
		if(path.find("automat")!=-1) { // we want automat anti aliased
			font->loadFont(basePath+path, size, false, true);
		} else {
			font->loadFont(basePath+path, size);
		}
		fonts[path+ofToString(size)] = font;
	}
	
	return fonts[path+ofToString(size)];
}