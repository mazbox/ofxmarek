/*
 *  FBOMSAA.h
 *  Snowboard
 *
 *  Created by Marek Bereza on 18/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */
#include "ofMain.h"
#include "ofxFbo.h"

#define FBOMSAA_LEVEL 2
class FBOMSAA {
public:
	void setup(int width, int height) {
		this->width = width;
		this->height = height;
		fbo.setup(width*FBOMSAA_LEVEL, height*FBOMSAA_LEVEL, GL_RGB, false);
	}
	
	void begin() {
		fbo.begin();
		glPushMatrix();
		glScalef(FBOMSAA_LEVEL, FBOMSAA_LEVEL, 1);
		ofClear(0,0,0,0);
	}
	
	void end() {
		glPopMatrix();
		fbo.end();
		ofSetHexColor(0xFFFFFF);
		fbo.draw(0, 0, width, height);
	}
	
private:
	ofxFbo fbo;
	float width, height;
};