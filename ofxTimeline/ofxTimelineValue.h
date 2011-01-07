#pragma once

#include <string>
#include <vector>
#include "ofxTimelineEventList.h"
using namespace std;

class ofxTimelineValue: public ofxTimelineEventListFloat {
public:

	/**
	 * Creates a new timeline. Optionally give it a name, and further optionally
	 * copy the contents of another timeline into this one.
	 */
	ofxTimelineValue(string name = "");//, ofxTimelineValue *other = NULL);
	
	
	
	/**
	 * Get the value of the timeline at a point in the graph.
	 */
	float getValue(double time);
	
	
	/**
	 * This version is more efficient if you want to get 
	 * a segment of the timeline out (i.e. for drawing)
	 * It remembers where you last looked and first checks
	 * after that point rather than starting from the beginning.
	 */
	float getValueInLoop(double time);
	
	/**
	 * Gives you the result of getValue, but normalized 
	 * between min and max values of the data set.
	 */
	float getNormalizedValue(double time);
	
	bool loadFromXml(string infile = "");
	
	bool saveToXml(string outfile = "");
	
	
	void print();

	void printInfo();
	
	
	

	/** 
	 * Adds the values of another timeline to this one.
	 */
	void addTimelineValue(ofxTimelineValue &timelineToAdd);
	
	
	
protected:
	
	//vector<ofxTimelinePoint<float> > points;
	
	
private:
	float lerpValue(double time, int i);
	int lastPointLookedAt;
	string xmlFile;
};