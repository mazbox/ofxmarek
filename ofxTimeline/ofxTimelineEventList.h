/*
 *  ofxTimelineEventList.h
 *  Snowboard
 *
 *  Use this to create discrete events on a timeline.
 *  The actual event type is specified with templates.
 * 
 *  E.g. 
 *		typedef int EventId;
 *		ofxTimelineEventList<EventId> events;
 *  
 *  will give you a list of events of type EventId
 *  
 *  Created by Marek Bereza on 16/12/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#pragma once

#include "ofxTimelinePoint.h"
#include <vector>
using namespace std;

template <class T>
class ofxTimelineEventList {
public:
	
	
	ofxTimelineEventList(string name = "") {//, ofxTimelineValue *other) {
		this->name = name;
	}
	
	
	void addPoint(double time, T value) {
		preprocessPoint(time, value);
		if(points.size()==0 || points.back().time<time) {
			points.push_back(ofxTimelinePoint<T>(time, value));
			return;
		}
		
		
		for(int i = 0; i < points.size()-1; i++) {
			if(points[i].time<=time && points[i+1].time>=time) {
				points.insert(points.begin()+i+1, ofxTimelinePoint<T>(time, value));
				return;
			}
		}
		
		if(points.size()>0 && points[0].time>value) {
			points.insert(points.begin(), ofxTimelinePoint<T>(time, value));
			return;
		}
		
		
		points.push_back(ofxTimelinePoint<T>(time, value));	
	}
	
	
	/**
	 * This returns a point if it exists
	 * in the timespan provided
	 */
	ofxTimelinePoint<T> *getPointInTimeSpan(double startTime, double endTime) {
		for(int i = 0; i < points.size(); i++) {
			if(points[i].time>=startTime && points[i].time<endTime) {
				return &points[i];
			}
		}
		return NULL;
	}
	
	
	// how many points there are
	int size() {
		return points.size();
	}
	// how long, in seconds
	double getDuration() {
		if(points.size()==0) return 0;
		return points.back().time - points[0].time;
	}
	
	
	double getEndTime() {
		if(points.size()==0) return 0;
		return points.back().time;
	}
	
	// when it starts	
	double getStartTime() {
		if(points.size()==0) return 0;
		return points[0].time;
	}
	
	
	
	vector<ofxTimelinePoint<T> > *getPoints() {
		return &points;
	}
	
	string name;
	
protected:
	
	/**
	 * Override this if you want to look at the points
	 * before adding them to the list - it's used to work
	 * out min and max values on the fly.
	 */
	virtual void preprocessPoint(double time, T value) {}
	vector<ofxTimelinePoint<T> > points;
	
};

class ofxTimelineEventListFloat: public ofxTimelineEventList<float> {
public: 
	ofxTimelineEventListFloat(string name = ""): ofxTimelineEventList<float>(name) {
		minValue = FLT_MAX;
		maxValue = FLT_MIN;
	}
	
	float getMinValue() {
		if(minValue==FLT_MAX) return 0;
		if(size()==1) return 0;
		return minValue;
	}
	
	float getMaxValue() {
		if(maxValue==FLT_MIN) return 0;
		if(size()==1) return 10;
		return maxValue;
	}	
	
	
	
protected:
	void findMinMax() {
		// recalculate min/max
		minValue = FLT_MAX;
		maxValue = FLT_MIN;
		for(int i = 0; i < points.size(); i++) {
			if(points[i].value>maxValue) maxValue = points[i].value;
			else if(points[i].value<minValue) minValue = points[i].value;
		}
	}
	
	float minValue, maxValue;
	void preprocessPoint(double time, float value) {
		if(value>maxValue) maxValue = value;
		if(value<minValue) minValue = value;
	}
};