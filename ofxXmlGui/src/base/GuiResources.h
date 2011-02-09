#pragma once

/*
 *  GuiResources.h
 *  Gui
 *
 *  Created by Marek Bereza on 12/02/2010.
 *
 */
#include <map>
#include "ofImage.h"
#include "ofTrueTypeFont.h"

#define DEFAULT_FONT "global/font/automat.ttf", 6


class GuiResources {
private:
	map<string,ofImage*> images;
	map<string,ofTrueTypeFont*> fonts;
	
	// not implemented yet, but basically, if you request a
	// resource with a prepended global/ it will look it up
	// in this object
	static GuiResources globals;
	ofTrueTypeFont *defaultFont;
	
public:
	GuiResources();
	ofImage *getImage(string path);
	ofTrueTypeFont *getFont(string path, int size);
	string basePath;
	void init(string xmlPath);
	ofTrueTypeFont *getDefaultFont();

};