/*
 *  ofxQTKitVideoExporter.cpp
 *  AlphaVideoExporter
 *
 *  Created by Marek Bereza on 07/12/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include "ofxQTKitVideoExporter.h"
#include <Cocoa/Cocoa.h>
#include <QTKit/QTKit.h>



NSString *getFormatStringForEnum(ofxQTKitFormat format) {
	switch(format) {
		case ofxQTKitFormat_Cinepak: return @"cvid";
		case ofxQTKitFormat_JPEG: return @"jpeg";
		case ofxQTKitFormat_Graphics: return @"smc ";
		case ofxQTKitFormat_Animation: return @"rle ";
		case ofxQTKitFormat_AppleVideo: return @"rpza";
		case ofxQTKitFormat_KodakPhotoCD: return @"kpcd";
		case ofxQTKitFormat_PNG: return @"png ";
		case ofxQTKitFormat_MotionJPEG_A: return @"mjpa";
		case ofxQTKitFormat_MotionJPEG_B: return @"mjpb";
		case ofxQTKitFormat_SorensonV1: return @"SVQ1";
		case ofxQTKitFormat_SorensonV3: return @"SVQ3";
		case ofxQTKitFormat_MPEG4: return @"mp4v";
		case ofxQTKitFormat_NTSC_DV25: return @"dvc ";
		case ofxQTKitFormat_PAL_DV25: return @"dvcp";
		case ofxQTKitFormat_GIF: return @"gif ";
		case ofxQTKitFormat_H263: return @"h263";
		case ofxQTKitFormat_TIFF: return @"tiff";
		case ofxQTKitFormat_RGB: return @"raw ";
		case ofxQTKitFormat_2vuy: return @"2vuY";
		case ofxQTKitFormat_yuv2: return @"yuv2";
		case ofxQTKitFormat_v308: return @"v308";
		case ofxQTKitFormat_v408: return @"v408";
		case ofxQTKitFormat_v216: return @"v216";
		case ofxQTKitFormat_v410: return @"v410";
		case ofxQTKitFormat_v210: return @"v210";
		default: return nil;
	}
}









ofxQTKitVideoExporter::ofxQTKitVideoExporter() {
	tempFile = "temp.mov";
}







void ofxQTKitVideoExporter::setup(int width, int height, int glType, float fps, ofxQTKitFormat format, ofxQTKitQuality quality) {
	this->width = width;
	this->height = height;
	this->fps = fps;
	this->glType = glType;
	this->format = format;
	this->quality = quality;

	if(glType!=GL_RGB && glType!=GL_RGBA) {
		printf("Error: ofxQTKitVideoExporter must use either GL_RGB or GL_RGBA format.\n");
	}
	if(glType==GL_RGBA && format!=ofxQTKitFormat_Animation) {
		printf("Warning: ofxQTkitVideoExporter in GL_RGBA mode is only known to work with the Animation codec.\n");
	}
	string filePath = tempFile;
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	imageRep = nil;
	filePath = ofToDataPath(filePath, true);
	// generate a name for our movie file
	NSString *name = [NSString stringWithCString:filePath.c_str()
											encoding:[NSString defaultCStringEncoding]];
	
	// Create a QTMovie with a writable data reference
	movie = [[QTMovie alloc] initToWritableFile:name error:NULL];
	
	// mark the movie as editable
	[movie setAttribute:[NSNumber numberWithBool:YES] forKey:QTMovieEditableAttribute];
	
	NSSize imageSize = NSMakeSize(width, height);
	printf("Inited movie maker with %d %d\n", width, height);
	
	image = [[NSImage alloc] initWithSize:imageSize];
	
	[image retain];
	// keep it around until we are done with it...
	[movie retain];
	
	videoFormatDict = [NSDictionary dictionaryWithObjectsAndKeys:getFormatStringForEnum(format),
			  QTAddImageCodecType,
			  [NSNumber numberWithLong:quality],
			  QTAddImageCodecQuality,
			  nil];
	[videoFormatDict retain];
	
	
	[pool release];
}


bool ofxQTKitVideoExporter::saveFile(string filePath) {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	filePath = ofToDataPath(filePath, true);
	NSString *name = [NSString stringWithCString:filePath.c_str()
										encoding:[NSString defaultCStringEncoding]];
	// create a dict. with the movie flatten attribute (QTMovieFlatten)
	// which we'll use to flatten the movie to a file below
	NSDictionary	*dict = nil;
	dict = [NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES] 
									   forKey:QTMovieFlatten];
	bool success = false;
	if (dict)
	{
		// create a new movie file and flatten the movie to the file

		success = [movie writeToFile:name withAttributes:dict];
	}
	[pool release];
	return success;
}


void ofxQTKitVideoExporter::addFrameFromNSImage(NSImage *img) {

	
	
	double timeVal = 100000.0/fps;
	long long timeValue = timeVal;
	long timeScale      = 100000;
	QTTime duration     = QTMakeTime(timeValue, timeScale);
	
	if (img)
	{
		// Adds an image for the specified duration to the QTMovie
		[movie addImage:img 
			forDuration:duration
		 withAttributes:videoFormatDict];
	}
}
int frame = 1;

void ofxQTKitVideoExporter::addFrame(unsigned char *data) {

	BOOL hasAlpha = glType==GL_RGBA || glType==GL_RGBA16;
/*
	ofImage img;
	img.setUseTexture(false);
	img.allocate(width, height, OF_IMAGE_COLOR);
	img.setFromPixels(data, width, height, OF_IMAGE_COLOR, true);
	img.saveImage("output/000"+ofToString(frame++)+".tiff");
	*/

	int bitsPerSample = 8;
	if(glType==GL_RGBA16 || glType==GL_RGB16) {
		bitsPerSample = 16;
	}
	if(glType==GL_RGB32F_ARB) {
		bitsPerSample = 32;
	}
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	if(imageRep!=nil) [image removeRepresentation:imageRep];

	imageRep =  [[NSBitmapImageRep alloc]
								initWithBitmapDataPlanes:(unsigned char **)&data
								pixelsWide:width pixelsHigh:height
								bitsPerSample:bitsPerSample
								samplesPerPixel:hasAlpha?4:3  // or 4 with alpha
								hasAlpha:glType==hasAlpha
								isPlanar:NO
								colorSpaceName:NSCalibratedRGBColorSpace
								bitmapFormat:0
								bytesPerRow:0  // 0 == determine automatically
								bitsPerPixel:0];  // 0 == determine automatically
//	NSCalibratedRGBColorSpace
//NSDeviceRGBColorSpace
	
	

	[image addRepresentation:imageRep];
	addFrameFromNSImage(image);
	
	//[image release];
	//[imageRep release];
	//[byteData release];
	[pool release];
 
}


void ofxQTKitVideoExporter::addFrame(string pathToImage) {
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	string path = ofToDataPath(pathToImage, true);
	
	NSString *name = [NSString stringWithCString:path.c_str()
										encoding:[NSString defaultCStringEncoding]];
	NSURL *fileUrl = [NSURL fileURLWithPath:name];        
	NSImage *anImage = [[NSImage alloc] initWithContentsOfURL:fileUrl];    
	
	addFrameFromNSImage(anImage);
	if(anImage!=nil) [anImage release];
	
	[pool release];
	
}