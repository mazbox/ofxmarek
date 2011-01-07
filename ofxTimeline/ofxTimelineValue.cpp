#include "ofxTimelineValue.h"
#include "ofMain.h"
#include "ofxXmlSettings.h"


ofxTimelineValue::ofxTimelineValue(string name): ofxTimelineEventListFloat(name) {
	
	
	lastPointLookedAt = 0;
}

void ofxTimelineValue::addTimelineValue(ofxTimelineValue &timelineToAdd) {
	if(points.size()==0) {
		vector<ofxTimelinePoint<float> > *p = timelineToAdd.getPoints();
		for(int i = 0; i < p->size(); i++) {
			addPoint(p->at(i).time, p->at(i).value);
		}
		
	} else {
		for(int i = 0; i < points.size(); i++) {
			points[i].value += timelineToAdd.getValue(points[i].time);
		}
	}
	findMinMax();
}


void ofxTimelineValue::printInfo() {
	if(points.size()==0) {
		printf("timeline '%s' has no points\n", name.c_str());
	} else {
		printf("timeline '%s' min: %f   max: %f   start time: %f,    duration %f\n", name.c_str(), getMinValue(), getMaxValue(), getStartTime(), getDuration());
	}
}



float ofxTimelineValue::getValueInLoop(double time) {
	if(points.size()==0) return 0;
	// go forward or backwards
	double currTime = points[lastPointLookedAt].time;
	if(time>currTime) {
		// do nothing
	} else {
		lastPointLookedAt = 0;
	}
	for(int i = lastPointLookedAt; i < points.size()-1; i++) {
		if(points[i].time<=time && points[i+1].time>=time) {
			// somewhere in between these 2 points
			lastPointLookedAt = i;
			return lerpValue(time, i);
		}
	}
	return 0;
}

float ofxTimelineValue::lerpValue(double time, int i) {
	float totalTime = (points[i+1].time - points[i].time);
	
	float timeIntoFrame = time - points[i].time;
	
	float amt = timeIntoFrame / totalTime;
	amt = 1.f - amt;
	return points[i].value*amt + (1.f-amt)*points[i+1].value;
}
/**
 * Get the value of the timeline at a point in the graph.
 */
float ofxTimelineValue::getValue(double time) {
	if(points.size()==0) return 0;
	//printf("Get value for %f\n", time);
	// find where the value is on the graph
	for(int i = 0; i < points.size()-1; i++) {
		//printf("Is it between %f and %f?\n", points[i].time, points[i+1].time);
		if(points[i].time<=time && points[i+1].time>=time) {
			// somewhere in between these 2 points
			return lerpValue(time, i);
		}
	}
	return 0;
}

float ofxTimelineValue::getNormalizedValue(double time) {
	return ofMap(getValue(time), minValue, maxValue, 0, 1);
}

bool ofxTimelineValue::loadFromXml(string infile) {
	if(infile=="") {
		infile = xmlFile;
	} else {
		xmlFile = infile;
	}
	if(infile=="") {
		printf("No xml file ever specified for timeline '%s'\n", name.c_str());
		infile = name + ".xml";
		xmlFile = infile;
		printf("Creating default: '%s'\n", xmlFile.c_str());
	}

	ofxXmlSettings xml;
	xml.loadFile(infile);
	xml.pushTag("timeline");
	int numTags = xml.getNumTags("point");
	for(int i = 0; i < numTags; i++) {
		double time = xml.getAttribute("point", "time", 0, i);
		float value = xml.getAttribute("point", "value", 0, i);
		addPoint(time, value);
	}
}

bool ofxTimelineValue::saveToXml(string outfile) {
	if(outfile=="") {
		outfile = xmlFile;
	} else {
		xmlFile = outfile;
	}
	if(outfile=="") {
		printf("No xml file ever specified for timeline '%s'\n", name.c_str());
		outfile = name +".xml";
		xmlFile = outfile;
		printf("Creating default: %s\n", xmlFile.c_str());
	}
	
	ofxXmlSettings xml;
	xml.addTag("timeline");
	xml.pushTag("timeline");
	for(int i = 0; i < points.size(); i++) {
		xml.addTag("point");
		xml.addAttribute("point", "time", points[i].time, i);
		xml.addAttribute("point", "value", points[i].value, i);
	}
	xml.saveFile(outfile);
	
	
}

void ofxTimelineValue::print() {
	for(int i = 0; i < points.size(); i++) {
		printf("Time: %f  Value %f\n", points[i].time, points[i].value);
	}
}
