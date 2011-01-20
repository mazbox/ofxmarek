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

double ofxTimelineValue::ofMapd(double value, double inputMin, double inputMax, double outputMin, double outputMax) {
	
	if (ABS(inputMin - inputMax) < FLT_EPSILON){
		ofLog(OF_LOG_WARNING, "ofMap: avoiding possible divide by zero, check inputMin and inputMax\n");
		return outputMin;
	} else {
		double outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
		
		return outVal;
	}
	
}


void ofxTimelineValue::exportSVG(string file) {
	string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	xml += "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" \"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n";
	
	int docWidth = 1024;
	int docHeight = 768;
	
	float minVal = getMinValue();
	float maxVal = getMaxValue();
	double minTime = getStartTime();
	double maxTime = getEndTime();
	string dat = "";
	int verticalPadding = 30;
	for(int i = 0; i < docWidth; i++) {
		double time = ofMapd(i, 0, docWidth, minTime, maxTime);
		float val = ofMapd(getValueInLoop(time), minVal, maxVal, docHeight-verticalPadding*2, verticalPadding);
		dat += ofToString(i) + "," + ofToString(val) + " ";
	}
	
	// comma separate x and y, space separate coords

	
	xml += "<svg version=\"1.0\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\" width=\""+ofToString(docWidth)+"px\" height=\""+ofToString(docHeight)+"px\" viewBox=\"0 0 "+ofToString(docWidth)+" "+ofToString(docHeight)+"\" enable-background=\"new 0 0 "+ofToString(docWidth)+" "+ofToString(docHeight)+"\" xml:space=\"preserve\">\n";

	// label
	xml += "<text transform=\"matrix(1 0 0 1 100, 20)\" font-family=\"'courier'\" font-size=\"14\">"+name+"</text>\n";
	
	// min max labels
	xml += "<text transform=\"matrix(1 0 0 1 20, "+ofToString(verticalPadding+5)+")\" font-family=\"'courier'\" font-size=\"14\">"+ofToString(maxVal,3)+"</text>\n";
	xml += "<text transform=\"matrix(1 0 0 1 20, "+ofToString(5+ docHeight - 2*verticalPadding)+")\" font-family=\"'courier'\" font-size=\"14\">"+ofToString(minVal,3)+"</text>\n";
	
	// top and bottom lines
	string topLineCoords = "90,"+ofToString(verticalPadding)+" "+ofToString(docWidth)+","+ofToString(verticalPadding);
	xml += "<polyline fill=\"none\" stroke=\"#000000\" stroke-miterlimit=\"10\" points=\""+topLineCoords+"\" />\n";
	
	string bottomLineCoords = "90,"+ofToString(docHeight - 2*verticalPadding)+" "+ofToString(docWidth)+","+ofToString(docHeight  - 2*verticalPadding);
	xml += "<polyline fill=\"none\" stroke=\"#000000\" stroke-miterlimit=\"10\" points=\""+bottomLineCoords+"\" />\n";
	
	xml += "<polyline fill=\"none\" stroke=\"#000000\" stroke-miterlimit=\"10\" points=\""+dat+"\" />\n";
	
	xml += "</svg>";
	
	
	ofstream out; 
	out.open(ofToDataPath(file).c_str(), ios::out); 
	out << xml << endl; 
	out.close();
	
	
	
	
}