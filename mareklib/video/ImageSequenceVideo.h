/*
 *  PngSequence.h
 *  IceGenerator
 *
 *  Created by Marek Bereza on 23/07/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#pragma once
#include "ofxDirList.h"
#include "constants.h"

class ImageSequenceVideo { 
public:
	
	
	bool loadMovie(string dir) {
		ofxDirList dirList;
		int numFiles = dirList.listDir(dir);
		
		playing = false;
		for(int i = 0; i < numFiles; i++) {
			imagePaths.push_back(dirList.getPath(i));
		}
		sort(imagePaths.begin(), imagePaths.end());
		
		width = 0;
		height = 0;
		index = 0;
		if(imagePaths.size()>0) {
			image.loadImage(imagePaths[0]);
			width = image.getWidth();
			height = image.getHeight();
		}
		return true;
	}
	
	void rewind() {
		index = 0;
	}

	void stop() {
		playing = false;
	}
	
	
	void play() {
		playing = true;
	}
	
	bool update() {
		if(playing && index<imagePaths.size()) {
			image.loadImage(imagePaths[index]);
			index++;
			return true;
		}
		return true;
	}
	
	void draw(float x, float y, float w, float h) {
		image.draw(x, y, w, h);
	}
	
	float getDuration() {
		return (float)imagePaths.size()/OUTPUT_FRAMERATE;
	}
	
	float getPosition() {
		return (float)index/(float)imagePaths.size();
	}
	
	
	void setPosition(float pos) {
		float frameF = pos * (float)imagePaths.size();
		index = frameF;
		image.loadImage(imagePaths[index]);
	}
	
	int getWidth() { return width; }
	int getHeight() { return height; }
	
private:

	bool playing;
	ofImage image;
	vector<string> imagePaths;
	int width, height;
	int index;
};