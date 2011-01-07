/*
 *  Fade.h
 *  VimeoAwards
 *
 *  Created by Marek Bereza on 27/09/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "ofMain.h"

class Fade {
public:
	Fade() {
		fadeStartTime = 0;
		fadeDuration = 1;
		fadeFrom = 0;
		fadeTo = 1;
		inited = false;
	}
	
	
	void fade(float to, float duration) {
		fade(getAlpha(), to, duration);
	}
	
	float fade(float from, float to, float duration) {
		fadeStartTime = ofGetElapsedTimef();
		fadeFrom = from;
		fadeTo = to;
		fadeDuration = duration;
		isDone = false;
		inited = true;
	}
	float getAlpha() {
		if(!inited) return 0;
		value = ofMap(ofGetElapsedTimef() - fadeStartTime, 0, fadeDuration, fadeFrom, fadeTo, true);
		if(fadeTo==value) {
			isDone = true;
		}
		return value;
	}
	
	bool done() {
		return isDone;
	}
private:
	float fadeStartTime;
	float fadeFrom;
	float fadeTo;
	float fadeDuration;
	float value;
	bool isDone;
	bool inited;
};
