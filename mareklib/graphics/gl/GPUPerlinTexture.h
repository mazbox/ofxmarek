/*
 *  GPUPerlinTexture.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 29/12/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include "ofxFbo.h"
#include "ofxShader.h"

class GPUPerlinTexture: public ofxFbo {
public:
	void init(int w, int h) {

		setup(w, h, GL_RGB32F_ARB);
		shader.setup("perlin.vert", "perlin.frag");
	}
	
	// call this every frame in your draw loop
	void generate(float zoom) {
		begin();
		ofClear(0, 0, 0, 0);
		shader.begin();
		shader.setUniform1f("d", ofGetElapsedTimef());
		shader.setUniform1f("dScale",0.2);
		shader.setUniform1f("xyScale", (float)zoom);
		
		//draw(0, 0, getWidth(), getHeight());
		glBegin(GL_QUADS);
		
		glTexCoord2f(0, 0);
		glVertex2f(0, 0);
		
		glTexCoord2f(getWidth(), 0);
		glVertex2f(getWidth(), 0);
		
		glTexCoord2f(getWidth(), getHeight());
		glVertex2f(getWidth(), getHeight());
		
		glTexCoord2f(0, getHeight());
		glVertex2f(0, getHeight());
		
		glEnd();
		shader.end();
		end();
	}
	ofxShader shader;
};

