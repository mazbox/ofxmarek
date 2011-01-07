#include "geomutils.h"

float _sign_(ofPoint p1, ofPoint p2, ofPoint p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool pointIsInsideTriangle(ofPoint pt, ofPoint v1, ofPoint v2, ofPoint v3)
{
	bool b1, b2, b3;
	
	b1 = _sign_(pt, v1, v2) < 0.0f;
	b2 = _sign_(pt, v2, v3) < 0.0f;
	b3 = _sign_(pt, v3, v1) < 0.0f;
	
	return ((b1 == b2) && (b2 == b3));
}