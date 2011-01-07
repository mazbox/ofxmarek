#include "ofxFbo.h"
#include "Blur.h"

class Glow {
public:
	void setup(float width, float height) {
		blur.setup(width, height);
		fbo1.setup(width, height, GL_RGBA);
		fbo2.setup(width, height, GL_RGBA);
		amt = 1.0;

	}
	void begin() {
		fbo1.begin();
		ofClear(0, 0, 0, 0);
	}
	
	void end() {
		fbo1.end();
		
		fbo2.begin();
		ofClear(0, 0, 0, 0);
		blur.begin();
		fbo1.draw(0, 0);
		blur.end();
		fbo2.end();
		
		
		ofClear(0, 0, 0, 0);
		ofEnableAlphaBlending();		
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glColor3f(1,1,1);
		fbo1.draw(0, 0);
		glColor4f(1, 1, 1, amt);
		fbo2.draw(0, 0);
		
	}
	float amt;
	ofxFbo fbo1, fbo2;
	Blur blur;
};
