/*
 *  ofxTimelineGui.cpp
 *  ofxTimelineExample
 *
 *  Created by Marek Bereza on 29/11/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */


#include "ofxTimelineGui.h"

ofxTimelineGui::ofxTimelineGui() {
	currentTimelineIndex = 0;
	
	currentValue = false;
	playing = false;
	playhead = 0;
	timelineHeight = 600;
	window.setRange(0, 10, -10, 10);//0, -1, 10, 2);
	gui.setup(0, timelineHeight, 250, LAYOUT_ABSOLUTE);
	graphList = gui.addList("Graph", currentTimelineIndex, "");
	graphList->set(0, 0, 300, ofGetHeight() - timelineHeight);

	// 10 40 70 100 130
	//gui.addToggle("Play", playing)					->set(310, 10, 80, 20);
	//gui.addButton("Rewind")							->set(310, 40, 80, 20);
	gui.addDoubleField("Playhead", playhead)			->set(310, 10, 80, 20);
	gui.addDoubleField("Time from start", timeFromStart)->set(310, 40, 80, 20);
	gui.addFloatField("Current value", currentValue)	->set(310, 70, 80, 20);
	gui.addListener(this);
	enable();
}
void ofxTimelineGui::togglePlaying() {
	playing ^= true;
}
string ofxTimelineGui::getCurrentTimelineName() {
	if(currentTimelineIndex!=-1 && currentTimelineIndex<graphList->items.size()) {
		return graphList->items[currentTimelineIndex];
	} else {
		return "";
	}
}
void ofxTimelineGui::draw(ofEventArgs &e) {
	
	ofNoFill();
	ofSetHexColor(0xFFFFFF);
	ofRect(0, 0, ofGetWidth(), timelineHeight);
	
	
	string graphName = graphList->items[currentTimelineIndex];
	bool foundGraph = false;
	for(int i = 0; i < timelines.size(); i++) {
		if(timelines[i]->name==graphName) {
			drawTimelineValue(timelines[i]);
			foundGraph = true;
		}
	}
	
	if(!foundGraph) {
		for(int i = 0; i < eventLists.size(); i++) {
			if(eventLists[i]->name==graphName) {
				drawEventList(eventLists[i]);
				foundGraph = true;
			}
		}
	}
	

	
	// draw playhead
	ofSetHexColor(0xFFFFFF);
	
	
	// this is in seconds, need it in pixels
	double timelinePos = playhead - window.x;
	timelinePos *= (double)ofGetWidth()/window.width;
	ofLine(timelinePos, 0, timelinePos, timelineHeight);
	ofFill();
	gui.draw();

}

void ofxTimelineGui::scaleTimeToFit() {
	double start = FLT_MAX;
	double end = FLT_MIN;
	for(int i = 0; i==0 && i < timelines.size(); i++) {
		double currStart = timelines[i]->getStartTime();
		double currEnd = timelines[i]->getEndTime();
		if(currStart<start) start = currStart;
		if(currEnd>end) end = currEnd;
	}
	
	if(start!=FLT_MAX && end!=FLT_MIN) {
		scaleTime(start, end);
	}
	
}

void ofxTimelineGui::scaleTime(double start, double end) {
	window.x = start;
	window.width = end - start;
	playhead = start;
}


void ofxTimelineGui::drawEventList(ofxTimelineEventListFloat *value) {
	
	glPushMatrix();
	
	window.setVerticalRange(value->getMinValue(), value->getMaxValue());
	glColor3f(1, 0, 0);
	glScalef((double)ofGetWidth()/window.width, timelineHeight/window.height, 1);
	glTranslatef(0, -window.y, 0);
	
	
	glPointSize(5);
	glBegin(GL_POINTS);
	glEnable(GL_POINT_SMOOTH);
	double increment = window.width/ofGetWidth();
	
	for(double i = 0; i < window.width; i+=increment) {
		double currPos = i+window.x;
		ofxTimelinePoint<float> *p = value->getPointInTimeSpan(currPos, currPos+increment);
		if(p!=NULL) {
			glVertex2d(i, p->value);
		}
	}
	glEnd();
	glPointSize(1);
	
	// draw some graph lines.
	glColor4f(1, 1, 1, 0.3);
	if(
	   (window.y>0 && window.height+window.y<0)
	   ||
	   (window.y<0 && window.height+window.y>0)
	   
	   ) {
		
		drawGraphline(0);
	}
	
	
	float from = window.y;
	float to = window.y + window.height;
	if(to<from) {
		float temp = to;
		to = from;
		from = temp;
	}
	
	if(ABS(window.height)<1) {
		// draw a graphline every 0.1
		from = round(from*10.f)/10.f;
		for(float f = from; f < to; f += 0.1) {
			drawGraphline(f);
		}
	} else if(ABS(window.height)<10) {
		// draw a graphline every 1
		from = round(from);
		for(float f = from; f < to; f += 1.0) {
			drawGraphline(f);
		}
	} else if(ABS(window.height)<100) {
		// draw a graphline every 10
		from = round(from/10.f)*10.f;
		for(float f = from; f < to; f += 10) {
			drawGraphline(f);
		}
	} else if(ABS(window.height)<1000) {
		// draw a graphline every 100
		from = round(from/100.f)*100.f;
		for(float f = from; f < to; f += 100) {
			drawGraphline(f);
		}
	}
	
	
	glPopMatrix();
	glColor3f(1, 1, 1);
	ofDrawBitmapString(value->name, 10, 20);
	
}

void ofxTimelineGui::drawTimelineValue(ofxTimelineValue *value) {
	glPushMatrix();
		
	window.setVerticalRange(value->getMinValue(), value->getMaxValue());
	glColor3f(1, 0, 0);
	glScalef((double)ofGetWidth()/window.width, timelineHeight/window.height, 1);
	glTranslatef(0, -window.y, 0);
	
	
	glBegin(GL_LINE_STRIP);
	double increment = window.width/ofGetWidth();

	for(double i = 0; i < window.width; i+=increment) {
		double y = value->getValueInLoop(i+window.x);
		glVertex2d(i, y);
	}
	glEnd();
	
	
	// draw some graph lines.
	glColor4f(1, 1, 1, 0.3);
	if(
	   (window.y>0 && window.height+window.y<0)
	   ||
	   (window.y<0 && window.height+window.y>0)
	   
	   ) {
		
		drawGraphline(0);
	}
	
	
	float from = window.y;
	float to = window.y + window.height;
	if(to<from) {
		float temp = to;
		to = from;
		from = temp;
	}
	
	if(ABS(window.height)<1) {
		// draw a graphline every 0.1
		from = round(from*10.f)/10.f;
		for(float f = from; f < to; f += 0.1) {
			drawGraphline(f);
		}
	} else if(ABS(window.height)<10) {
		// draw a graphline every 1
		from = round(from);
		for(float f = from; f < to; f += 1.0) {
			drawGraphline(f);
		}
	} else if(ABS(window.height)<100) {
		// draw a graphline every 10
		from = round(from/10.f)*10.f;
		for(float f = from; f < to; f += 10) {
			drawGraphline(f);
		}
	} else if(ABS(window.height)<1000) {
		// draw a graphline every 100
		from = round(from/100.f)*100.f;
		for(float f = from; f < to; f += 100) {
			drawGraphline(f);
		}
	}
	
	
	glPopMatrix();
	glColor3f(1, 1, 1);
	ofDrawBitmapString(value->name, 10, 20);
}

void ofxTimelineGui::drawGraphline(float value) {
	ofLine(0, value, window.width, value);
	ofDrawBitmapString(ofToString(value, 2), 0, value);
}

void ofxTimelineGui::addTimelineValue(ofxTimelineValue *value) {
	timelines.push_back(value);
	graphList->addItem(value->name);
}

void ofxTimelineGui::addTimelineEventList(ofxTimelineEventListFloat *el) {
	eventLists.push_back(el);
	graphList->addItem(el->name);
}

bool ofxTimelineGui::isEnabled() {
	return enabled;
}

void ofxTimelineGui::enable() {
	ofAddListener(ofEvents.mousePressed, this, &ofxTimelineGui::mousePressed);
	ofAddListener(ofEvents.mouseMoved, this, &ofxTimelineGui::mouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &ofxTimelineGui::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &ofxTimelineGui::mouseReleased);
	ofAddListener(ofEvents.draw, this, &ofxTimelineGui::draw);
	ofAddListener(ofEvents.update, this, &ofxTimelineGui::update);
	gui.enable();
	enabled = true;
}

void ofxTimelineGui::disable() {
	ofRemoveListener(ofEvents.mousePressed, this, &ofxTimelineGui::mousePressed);
	ofRemoveListener(ofEvents.mouseMoved, this, &ofxTimelineGui::mouseMoved);
	ofRemoveListener(ofEvents.mouseDragged, this, &ofxTimelineGui::mouseDragged);
	ofRemoveListener(ofEvents.mouseReleased, this, &ofxTimelineGui::mouseReleased);
	ofRemoveListener(ofEvents.draw, this, &ofxTimelineGui::draw);
	ofRemoveListener(ofEvents.update, this, &ofxTimelineGui::update);
	gui.disable();
	enabled = false;
}

void ofxTimelineGui::update(ofEventArgs &e) {
	if(playing) playhead += 1.f/30.f;
	timeFromStart = playhead - window.x;
	if(timelines.size()>0) {
		
		string graphName = graphList->items[currentTimelineIndex];
		for(int i = 0; i < timelines.size(); i++) {
			if(timelines[i]->name==graphName) {
				currentValue = timelines[currentTimelineIndex]->getValue(playhead);
			}
		}
		for(int i = 0; i < eventLists.size(); i++) {
			if(eventLists[i]->name==graphName) {
				// do nothing for now lazy
				currentValue = 0;
			}
		}
	}
}
void ofxTimelineGui::mousePressed(ofMouseEventArgs &e) {
	gui.touchDown(e.x, e.y, e.button);
	bool editing = false;
	if(editing) {
		int timelineIndex = e.y/timelineHeight;
		if(timelineIndex>=0 && timelineIndex<timelines.size()) {
			float time = ofMap(e.x, 
								0, ofGetWidth(), 
								window.x, window.x+window.width
								);
			float value = ofMap(e.y, 
							   
							   timelineIndex*timelineHeight, (timelineIndex+1)*timelineHeight,
							   window.y, window.y + window.height);
			timelines[timelineIndex]->addPoint(time, value);
		}
	} else {
		// scrub
		if(e.y<timelineHeight) {
			playhead = window.x + window.width*(double)e.x/(double)ofGetWidth();
		}
	}
}

void ofxTimelineGui::mouseMoved(ofMouseEventArgs &e) {
	gui.touchOver(e.x, e.y, e.button);
}

void ofxTimelineGui::mouseDragged(ofMouseEventArgs &e) {
	gui.touchMoved(e.x, e.y, e.button);
	if(e.y<timelineHeight) {
		playhead = window.x + window.width*(double)e.x/(double)ofGetWidth();
	}
}

void ofxTimelineGui::mouseReleased(ofMouseEventArgs &e) {
	gui.touchUp(e.x, e.y, e.button);
}


void ofxTimelineGui::controlChanged(GuiControl *ctrl) {
	if(ctrl->controlId=="Rewind") {
		playhead = window.x;
	}

}

double ofxTimelineGui::getCurrentTime() {
	return playhead;
}





double ofxTimelineGui::getStartTime() {
	return window.x;
}
double ofxTimelineGui::getEndTime() {
	return window.x + window.width;
}
void   ofxTimelineGui::setCurrentTime(double time) {
	playing = false;
	playhead = time;
	
}