/**
 * Resources.cpp
 * emptyExample
 *
 * Created by Marek Bereza on 29/06/2011.
 *
 */

#include "Resources.h"
#include <sys/stat.h>

//#define DEFAULT_FONT "OCRAStd.ttf"
//#define DEFAULT_FONT_SIZE 12
#define DEFAULT_FONT "me-nlo.ttf"
#define DEFAULT_FONT_SIZE 9


ofTrueTypeFont *gui::Resources::font = NULL;
bool gui::Resources::customFontNotAvailable = false;
map<string,ofImage*> gui::Resources::images;

ofImage *gui::Resources::getImage(string path) {


	if(path=="") return NULL;

	struct stat stFileInfo; 
	
	if(stat(ofToDataPath(path, true).c_str(),&stFileInfo)!=0) {
		printf("Can't find the file %s\n", (path).c_str());
		return NULL;
	}
	
	// cache the image if it's not already loaded
	if(images.find(path)==images.end()) {
		ofImage *img = new ofImage();
		img->loadImage(path);
		images[path] = img;
		//printf("Loaded file from %s\n", string(basePath+path).c_str());
	}

	return images[path];
}

void gui::Resources::drawString(string str, int x, int y) {
	if(font==NULL && !customFontNotAvailable) {
		// try to load font
		ofFile f(DEFAULT_FONT);
		if(f.exists()) {
			font = new ofTrueTypeFont();
			font->loadFont(DEFAULT_FONT, DEFAULT_FONT_SIZE);
		} else {
			customFontNotAvailable = true;
		}
	}
	if(customFontNotAvailable) {
		ofDrawBitmapString(str, x, y);
	} else {
		font->drawString(str, x, y);
	}

}