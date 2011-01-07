/*
 *  ofxQTKitVideoExporter.h
 *  AlphaVideoExporter
 *
 *  Created by Marek Bereza on 07/12/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include "ofMain.h"
#import <QuickTime/QuickTime.h>

#include "ofxQTKitFormatOptions.h"

#ifdef __OBJC__
@class QTMovie;
#include <Cocoa/Cocoa.h>
#endif

class ofxQTKitVideoExporter {
public:
	ofxQTKitVideoExporter();
	
	// call this at the beginning of your app
	void setup(
			   int width, 
			   int height, 
			   int glType, // currently supports GL_RGB and GL_RGBA
			   float fps = 30, 
			   ofxQTKitFormat format = ofxQTKitFormat_MPEG4, 
			   ofxQTKitQuality quality = ofxQTKitQuality_High);
	
	// call this to add an image to the movie
	void addFrame(string image);
	
	// or call this and give it raw pixel data.
	void addFrame(unsigned char *data);
	
	// call this when you're done adding frames
	bool saveFile(string filePath);
	void setTempFile(string td) { tempFile = td; }
private:
	string filePath;
	int width;
	int height;
	int glType;
	float fps;
	ofxQTKitFormat format;
	ofxQTKitQuality quality;
	string tempFile;
#ifdef __OBJC__
    QTMovie*				movie;
	void addFrameFromNSImage(NSImage *img);
	NSDictionary *videoFormatDict;

	NSImage * image;
	NSBitmapImageRep * imageRep;

#else
	void*					movie;
	void addFrameFromNSImage(void *img);
	void *videoFormatDict;
	void *image;
	void *imageRep;
#endif
};