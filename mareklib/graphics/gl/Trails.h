#include "ofxFbo.h"

class Trails {
public:
	void setup(float width, float height) {
		
		bigFbo.setup(width, height, GL_RGBA);
		float multiplier = 0.5;
		littleFbo.setup(width*multiplier, height*multiplier, GL_RGBA);
		
	}
	void begin() {
		ofBackground(0, 0, 0);
		bigFbo.begin();

		ofClear(0, 0, 0, 0);
		littleFbo.getTexture(0).draw(0, 0, bigFbo.getWidth(), bigFbo.getHeight());
	}
	
	void end() {
		
		bigFbo.end();
		
		drawBigIntoLittle();
		
		glColor4f(1, 1, 1, 1);
		bigFbo.draw(0, 0);
		
		
	}
	
	void drawBigIntoLittle() {
		littleFbo.begin();
		ofClear(0, 0, 0, 0);
		bigFbo.getTexture(0).draw(0, 0, littleFbo.getWidth(), littleFbo.getHeight());
		littleFbo.end();
	}
	
	
private:
	ofxFbo			bigFbo;
	ofxFbo			littleFbo;
};
