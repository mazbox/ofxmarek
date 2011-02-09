/*
 *  PdGui.h
 *  PdPlayer
 *
 *  Created by Marek Bereza on 13/02/2010.
 *
 */

#include "GuiContainer.h"
#include "ofxPd.h"
#include "ofMain.h"
#pragma once
class PdGui: public GuiContainer, public GuiListener {
public:
	PdGui(): GuiContainer() {
		pd = NULL;
	}
	void load(string guiFile, string pdFile, int bufferSize) {

		loadFromXml(guiFile);
		setListener(this);
		pd = new ofxPd();
		pd->load(ofToDataPath(pdFile), 2,2,44100, bufferSize);
		if(pd->hasInput()) {
			ofSoundStreamSetup(2, 1, ofGetAppPtr(), 44100, 512, 1);
		} else {
			ofSoundStreamSetup(2, 0, ofGetAppPtr(), 44100, 512, 1);
		}
		show();
	}
	
	void process(float *input, float *output, int frames) {
		if(pd!=NULL)
			pd->process(input, output, frames);
	}
	
	
	void controlChanged(GuiControl *control) {

		if(control->type=="button") {
			pd->sendBang(control->controlId);
		} else if(control->type=="slider" || control->type=="knob") {
			pd->sendFloat(control->controlId, fval(control->value));
		} else if(control->type=="slider2d") {
			pd->sendFloat(control->controlId, fval(control->value));
			
		} else if(control->type=="keyboard") {
			pd->sendFloat(control->controlId, ival(control->value));
		} else if(control->type=="toggle") {
			pd->sendFloat(control->controlId, bval(control->value)?1.f:0.f);
		}
	}
	
	void draw() {
		
		if(controls.size()>0) {
			GuiContainer::draw();
		}
	}
	
	
private:
	ofxPd *pd;
};