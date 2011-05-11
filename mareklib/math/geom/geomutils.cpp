#include "geomutils.h"
#include "ofVec2f.h"

float _sign_(ofVec2f p1, ofVec2f p2, ofVec2f p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool pointIsInsideTriangle(ofVec2f pt, ofVec2f v1, ofVec2f v2, ofVec2f v3)
{
	bool b1, b2, b3;
	
	b1 = _sign_(pt, v1, v2) < 0.0f;
	b2 = _sign_(pt, v2, v3) < 0.0f;
	b3 = _sign_(pt, v3, v1) < 0.0f;
	
	return ((b1 == b2) && (b2 == b3));
}

ofPoint getIntersection(ofPoint a1, ofPoint a2, ofPoint b1, ofPoint b2) {

	// the 2 line equations
	float A1 = a2.y - a1.y;
	float B1 = a1.x - a2.x;
	float C1 = A1*a1.x + B1*a1.y;
	
	float A2 = b2.y - b1.y;
	float B2 = b1.x - b2.x;
	float C2 = A2*b1.x + B2*b1.y;
	
	float det = A1*B2 - A2*B1;
	if(det==0) {
		printf("Error!! Lines are parallel\n");
		return ofPoint(0, 0);
	} else {
		return ofPoint((B2*C1 - B1*C2)/det, (A1*C2 - A2*C1)/det);
	}
}


float pointLineDistance(ofVec2f a, ofVec2f b, ofVec2f p) {
	
	// from http://www.softsurfer.com/Archive/algorithm_0102/algorithm_0102.htm#Distance to 2-Point Line
	return ABS(((a.y - b.y)*p.x + (b.x - a.x)*p.y + (a.x*b.y - b.x*a.y))/
	sqrt((b.x-a.x)*(b.x-a.x) + (b.y - a.y)*(b.y - a.y))
	);
}

