/*
 *  RandomTexture.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 29/12/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

class RandomTexture: public ofTexture {
public:
	RandomTexture(): ofTexture() {
		
		
	}
	void init(int w, int h) {
		bool floatingPoint = true;
		if(floatingPoint) {
			allocate(w, h, GL_RGB32F_ARB);
			float *myData = new float[w*h*3];
			for(int i = 0; i < w*h*3; i++) {
				myData[i] = ofRandom(-0.02, 0.02);
			}
			loadData(myData, w, h, GL_RGB);
			delete [] myData;
		} else {
			allocate(w, h, GL_RGB);
			unsigned char *myData = new unsigned char[w*h*3];
			for(int i = 0; i < w*h*3; i++) {
				myData[i] = ofRandom(0, 255);
			}
			loadData(myData, w, h, GL_RGB);
			delete [] myData;
		}
	}
};
