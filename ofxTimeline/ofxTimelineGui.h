/*
 *  ofxTimelineGui.h
 *  ofxTimelineExample
 *
 *  Created by Marek Bereza on 29/11/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#pragma once
#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxXmlGui.h"

class ofxTimelineGui: public GuiListener {
public:
	ofxXmlGui gui;
	ofxTimelineGui();
	
	double getCurrentTime();
	double getStartTime();
	double getEndTime();
	void setCurrentTime(double time);
	
	void enable();
	void disable();
	
	bool isEnabled();
	
	string getCurrentTimelineName();
	
	void togglePlaying();

	
	void addTimelineValue(ofxTimelineValue *value);
	
	void addTimelineEventList(ofxTimelineEventListFloat *el);
	void scaleTimeToFit();
	
	// zoom the time to fit between a start time and end time (in seconds)
	void scaleTime(double start, double end);
	
	
	/** Callback for gui stuff */
	void controlChanged(GuiControl *ctrl);	
	
private:
	void update(ofEventArgs &e);
	void draw(ofEventArgs &e);
	void mousePressed(ofMouseEventArgs &e);
	void mouseMoved(ofMouseEventArgs &e);
	void mouseDragged(ofMouseEventArgs &e);
	void mouseReleased(ofMouseEventArgs &e);
	
	int currentTimelineIndex;
	GuiList * graphList;
	
	bool playing;
	double playhead;
	double timeFromStart;
	float currentValue;
	class DRectangle {
	public:
		double x, y, width, height;
		void set(double x, double y, double width, double height) {
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
		}
		void setRange(double minX, double maxX, double minY, double maxY) {
			x = maxX;
			y = maxY;
			width = minX - maxX;
			height = minY - maxY;
		}
		void setVerticalRange(double minY, double maxY) {
			y = maxY;
			height = minY - maxY;
		}
	};
	
	// this is the drawing window, measured in time and value
	DRectangle window;
	
	DRectangle graphRect;
	
	void drawGraphline(float value);
	void drawTimelineValue(ofxTimelineValue *value);
	void drawEventList(ofxTimelineEventListFloat *value);
	
	bool enabled;
	vector<ofxTimelineValue*> timelines;
	vector<ofxTimelineEventListFloat *> eventLists;
	double timelineHeight;
};