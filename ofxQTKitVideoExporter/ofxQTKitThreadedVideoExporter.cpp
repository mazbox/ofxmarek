/*
 *  ofxQTKitThreadedVideoExporter.cpp
 *  Snowboard
 *
 *  Created by Marek Bereza on 15/12/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include "ofxQTKitThreadedVideoExporter.h"

#define MSAA_LEVEL 2
void ofxQTKitThreadedVideoExporter::setTempFile(string tempFile) { exporter.setTempFile(tempFile); }
void ofxQTKitThreadedVideoExporter::setup(
		   int width, 
		   int height, 
		   int glType, // currently supports GL_RGB and GL_RGBA
		   float fps, 
		   ofxQTKitFormat format, 
		   ofxQTKitQuality quality,
		   bool useMSAA
		   ) {
	
	exporter.setup(width, height, glType, fps, format, quality);

	
	this->useMSAA = useMSAA;
	if(this->useMSAA) {
		fbo.setup(width*MSAA_LEVEL, height*MSAA_LEVEL, glType, false);
		fbo2.setup(width, height, glType, false);
	} else {
		fbo.setup(width, height, glType, false);//useMSAA?ofxFbo::maxSamples():0);
	}
	int bpp = 3;
	switch(glType) {
		case GL_RGB: bpp = 3; break;
		case GL_RGBA: bpp = 4; break;
		case GL_RGB16: bpp = 6; break;
		case GL_RGBA16: bpp = 8; break;
		case GL_RGB32F_ARB: bpp = 12; break;
		case GL_RGBA32F_ARB: bpp = 16; break;
	}
	pixels = new unsigned char[width*height*bpp];
	this->glType = glType;
}



void ofxQTKitThreadedVideoExporter::begin() {
	fbo.begin();
	if(useMSAA) {
		glPushMatrix();
		glScalef(MSAA_LEVEL, MSAA_LEVEL, 1);
	}
	
	ofClear(0,0,0,0);
}

void ofxQTKitThreadedVideoExporter::end() {
	if(useMSAA) glPopMatrix();
	fbo.end();
	ofSetHexColor(0xFFFFFF);
	
	

	ofFbo *theFBO = &fbo;
	if(useMSAA) {
		theFBO = &fbo2;
		fbo2.begin();
		ofClear(0,0,0,0);
		glPushMatrix();
		glScalef(1.f/MSAA_LEVEL, 1.f/MSAA_LEVEL, 1);
		fbo.draw(0, 0);
		glPopMatrix();
		fbo2.end();
		fbo2.draw(0, 0);
	} else {
		fbo.draw(0,0);
	}
	
	// wait for thread to end
	waitForThread(true);
	

	theFBO->bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	
	int glTypeR = GL_RGB;

	if(glType==GL_RGBA) {
		glTypeR = GL_RGBA;
	}
	glReadPixels(0, 0, theFBO->getWidth(), theFBO->getHeight(), glTypeR, GL_UNSIGNED_BYTE, pixels);
	theFBO->unbind();
	
	// start the thread again
	startThread(true, false);
	
}


void ofxQTKitThreadedVideoExporter::threadedFunction() {
	exporter.addFrame(pixels);
}
void ofxQTKitThreadedVideoExporter::saveFile(string file) {
	waitForThread(true);
	exporter.saveFile(file);
}
