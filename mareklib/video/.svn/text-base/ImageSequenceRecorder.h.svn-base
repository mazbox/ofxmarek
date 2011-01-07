/*
 *  ImageSequenceRecorder.h
 *  IceGenerator
 *
 *  Created by Marek Bereza on 23/07/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include "ofMain.h"
#include "Poco/File.h"
using Poco::File;

class ImageSequenceRecorder {
public:
	
	// if output dir is blank we create a new folder with date and time
	void setup(int _width, int _height, string _outputDir = "") {
		width = _width;
		height = _height;
		if(_outputDir=="") {
			_outputDir = ofToString(ofGetYear()) + "-"+ofToString(ofGetMonth()) + "-" + 
				ofToString(ofGetDay()) + "-"+ofToString(ofGetHours())+"-"+ofToString(ofGetMinutes())
			+"-" +ofToString(ofGetSeconds());
			
		}
		
		outputDir = _outputDir;
		outputDir = ofToDataPath(outputDir, true);
		File path(outputDir);
		printf("Creating directory %s\n", outputDir.c_str());
		path.createDirectories();
		frame = 0;
		
	}
	
	string toFrameNo(int fn) {
		if(fn<10) return "0000"+ofToString(fn);
		else if(fn<100) return "000"+ofToString(fn);
		else if(fn<1000) return "00"+ofToString(fn);
		else if(fn<10000) return "0"+ofToString(fn);
		else if(fn<100000) return ofToString(fn);
		else return "out of range!!";
	}
	
	void recordFrame() {
		image.grabScreen(0, 0, width, height);
		image.saveImage(outputDir + "/image"+toFrameNo(frame)+".tif");
		frame++;
	}
	
private:
	int		width;
	int		height;
	int		frame;
	ofImage image;
	string	outputDir;
};