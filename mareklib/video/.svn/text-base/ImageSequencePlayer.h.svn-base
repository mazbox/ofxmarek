/*
 *  PngSequence.h
 *  IceGenerator
 *
 *  Created by Marek Bereza on 23/07/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "ofxDirList.h"
#include "FrameProvider.h"

class ImageSequencePlayer: public FrameProvider<ofImage*> {
public:
	
	
	bool load(string dir) {
		ofxDirList dirList;
		int numFiles = dirList.listDir(dir);
		
		
		for(int i = 0; i < numFiles; i++) {
			imagePaths.push_back(dirList.getPath(i));
		}
		sort(imagePaths.begin(), imagePaths.end());
		
		width = 0;
		height = 0;
		if(imagePaths.size()>0) {
			image.loadImage(imagePaths[0]);
			width = image.getWidth();
			height = image.getHeight();
		}
		return true;
	}
	

	
	
	/**
	 * If you give this a frame that you want
	 */
	ofImage *getFrame(int requestedFrame = -1) {
		if(requestedFrame!=-1) {
			frame = requestedFrame;
		} else {
			if(looping && frame>=imagePaths.size()-1) frame = 0;
			if(frame<=getLength()-1) {
				image.loadImage(imagePaths[frame]);
				frame++;
			}
		}
		return &image;
	}
	
	
	int getLength() { return imagePaths.size(); }
	int getWidth() { return width; }
	int getHeight() { return height; }
	
private:

	ofImage image;
	vector<string> imagePaths;
	int width, height;
};