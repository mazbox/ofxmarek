/*
 *  LevelList.h
 *  VimeoAwards
 *
 *  Created by Marek Bereza on 29/09/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include "ofMain.h"
#pragma once
class FadeList {
public:
	FadeList() {
		started = false; 
		totalDuration = 0;
	}
	
	// start fading
	void start() {
		started = true;
		startTime = ofGetElapsedTimef();
	}
	
	void clear() {
		started = false;
		totalDuration = 0;
		items.clear();
	}
	
	float getValue() {
		
		// if the fade hasn't started, send zero
		if(!started) return 0;
		
		// if the fade is already finished, return the last value
		if(done()) return items.back().to;
		
		
		// otherwise, find where we are
		float currTime = ofGetElapsedTimef() - startTime;

		for(int i = 0; i < items.size(); i++) {
			if(currTime<items[i].duration) {
				return ofMap(currTime, 0, items[i].duration, items[i].from, items[i].to);
			} else {
				currTime -= items[i].duration;
			}
		}
		printf("There must be a bug in the FadeList code\n");
		return 0;
	}
	
	/**
	 * Add a fade to the list, specifying value to and from
	 */
	void addFade(float from, float to, float duration) {
		totalDuration += duration;
		items.push_back(FadeListItem(from, to, duration));
	}
	
	/**
	 * Add a fade to the list, only specifying where the value goes to.
	 * It will take the last value to start from.
	 */
	void addFade(float to, float duration) {
		if(items.size()==0) {
			addFade(0, to, duration);
		} else {
			addFade(items.back().to, to, duration);
		}
	}
	
	/**
	 * Adds a pause
	 */
	void addDelay(float duration) {
		if(items.size()==0) {
			addFade(0, 0, duration);
		} else {
			float lastValue = items.back().to;
			addFade(lastValue, lastValue, duration);
		}
	}
	
	
	bool done() {
		if(!started) return false;
		return ofGetElapsedTimef() > startTime+totalDuration;
	}
	
private:
	float startTime;
	bool started;
		 
	float totalDuration;
	
	class FadeListItem {
	public:
		float from;
		float to;
		float duration;
		FadeListItem(float from, float to, float duration) {
			this->from = from;
			this->to = to;
			this->duration = duration;
		}
	};
	vector<FadeListItem> items;
};