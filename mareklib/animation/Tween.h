/*
 *  Fade.h
 *  VimeoAwards
 *
 *  Created by Marek Bereza on 27/09/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "ofMain.h"
#include "ofxEasing.h"
#include "ofxEasingBack.h"
#include "ofxEasingBounce.h"
#include "ofxEasingCirc.h"
#include "ofxEasingCubic.h"
#include "ofxEasingElastic.h"
#include "ofxEasingExpo.h"
#include "ofxEasingLinear.h"
#include "ofxEasingQuad.h"
#include "ofxEasingQuart.h"
#include "ofxEasingQuint.h"
#include "ofxEasingSine.h"

#define EASE_IN 1
#define EASE_OUT 2
#define EASE_IN_OUT 3


#define EASE_LINEAR 1
#define EASE_BACK 2
#define EASE_CIRC 3
#define EASE_CUBIC 4
#define EASE_ELASTIC 5
#define EASE_EXPO 6
#define EASE_QUAD 7
#define EASE_QUART 8
#define EASE_QUINT 9
#define EASE_SINE 10

#pragma once

class Tween {
public:
	Tween() {
		fadeStartTime = 0;
		fadeDuration = 1;
		fadeFrom = 0;
		fadeTo = 1;
		inited = false;
		value = 0;
		easer = NULL;
		setType(EASE_IN, EASE_LINEAR);
	}
	
	~Tween() {
		if(easer!=NULL) delete easer;
	}
	
	void setType(int type, int direction) {
		
		this->type = type;
		this->direction = direction;
		
		delete easer;
		
		switch(this->type) {
			case EASE_BACK: easer = new ofxEasingBack(); break;
			case EASE_CIRC: easer = new ofxEasingCirc(); break;
			case EASE_CUBIC: easer = new ofxEasingCubic(); break;
			case EASE_ELASTIC: easer = new ofxEasingElastic(); break;
			case EASE_EXPO: easer = new ofxEasingExpo(); break;
			case EASE_QUAD: easer = new ofxEasingQuad(); break;
			case EASE_QUART: easer = new ofxEasingQuart(); break;
			case EASE_QUINT: easer = new ofxEasingQuint(); break;
			case EASE_SINE: easer = new ofxEasingSine(); break;
			case EASE_LINEAR:
			default:
				easer = new ofxEasingLinear();
				break;
		}
	}
	
	void operator=(const float &a) {
		value = a;
	}
	
	bool operator==(const float &a) {
		return value==a;
	}
	
	void tween(float to, float duration) {
		tween(value, to, duration);
	}
	
	void delayThenTween(float delay, float to, float duration) {
		delayTweenDelay(delay, value, to, duration, 0);
	}
	
	void tweenThenDelay(float to, float duration, float delay) {
		delayTweenDelay(0, value, to, duration, delay);
	}
	
	
	
	void tween(float from, float to, float duration) {
		delayTweenDelay(0, from, to, duration, 0);
	}
	
	
	void delayTweenDelay(float preDelay, float from, float to, float duration, float postDelay) {
		preDelayStartTime = ofGetElapsedTimef();
		
		fadeStartTime = preDelayStartTime + preDelay;
		fadeFrom = from;
		fadeTo = to;
		fadeDuration = duration;
		postDelayEndTime = fadeStartTime + fadeDuration+postDelay;
		
		isDone = false;
		inited = true;
		
	}
	
	
	
	float getValue() {
		if(!inited || isDone) return value;
		float currTime = ofGetElapsedTimef();
		//value = ofMap(currTime - fadeStartTime, 0, fadeDuration, fadeFrom, fadeTo, true);
		float t = ofClamp(currTime - fadeStartTime, 0, fadeDuration);
		
		switch(direction) {
			case EASE_IN:
				value = easer->easeIn(t, fadeFrom, fadeTo - fadeFrom, fadeDuration);
				break;
			case EASE_OUT:
				value = easer->easeOut(t, fadeFrom, fadeTo - fadeFrom, fadeDuration);
				break;
			case EASE_IN_OUT:
				value = easer->easeInOut(t, fadeFrom, fadeTo - fadeFrom, fadeDuration);
				break;
		}
		if(currTime>postDelayEndTime) {
			isDone = true;
		}
		return value;
	}
	
	bool done() {
		return isDone;
	}
	
	
	
private:
	ofxEasing *easer;
	float preDelayStartTime;
	float postDelayEndTime;
	float fadeStartTime;
	float fadeFrom;
	float fadeTo;
	float fadeDuration;
	float value;
	bool isDone;
	bool inited;
	int direction;
	int type;
};
