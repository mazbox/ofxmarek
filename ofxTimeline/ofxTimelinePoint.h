#pragma once


/**
 * Types of timeline point you can have.
 * For now, it's just linear.
 */
enum ofxTimelineType {
	ofxTimelineLinear
};


/**
 * Represents a point on the timeline.
 * Linear for now.
 */


template <class T>
class ofxTimelinePoint {
public:
	ofxTimelinePoint(double time, T value, ofxTimelineType = ofxTimelineLinear) {
		this->time = time;
		this->value = value;
		this->type = type;
	}

	double time;
	T value;
	ofxTimelineType type;
};