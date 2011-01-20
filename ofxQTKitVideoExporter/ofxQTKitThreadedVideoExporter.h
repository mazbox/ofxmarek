/*
 *  ofxQTKitThreadedVideoExporter.h
 *  AlphaVideoExporter
 *
 *  This exports alpha videos. This doesn't use the FBO's 
 *  MSAA, it does it explicitly by drawing to a double sized
 *  FBO and downscales it.
 *
 *  Created by Marek Bereza on 07/12/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include "ofxQTKitVideoExporter.h"


class ofxQTKitThreadedVideoExporter: public ofThread {
public:

	void setup(
			   int width, 
			   int height, 
			   int glType, // currently supports GL_RGB and GL_RGBA
			   float fps = 30, 
			   ofxQTKitFormat format = ofxQTKitFormat_MPEG4, 
			   ofxQTKitQuality quality = ofxQTKitQuality_High,
			   bool useMSAA = false
			   );	
	
	// when exporting, you need to use a temporary file to copy the data to.
	// this file is usually the same size as the video itself.
	void setTempFile(string tempFile);
	
	// do drawing in between begin and end
	void begin();
	void end();	
	
	

	// once you've finished rendering, call this to create the movie file
	void saveFile(string file);
	
	
	
	// for threading - you don't need to touch this
	void threadedFunction();
	
private:
	unsigned char *pixels;
	ofxQTKitVideoExporter exporter;
	int glType;
	bool useMSAA;
	ofFbo fbo;
	ofFbo fbo2;
	
};