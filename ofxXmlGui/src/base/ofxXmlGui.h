/*
 *  ofxXmlGui.h
 *  GuiDownloader
 *
 *  Created by Marek Bereza on 28/03/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#pragma once

#include "GuiContainer.h"
#include "GuiSlider.h"
#include "GuiToggle.h"
#include "GuiSegmentedControl.h"
#include "GuiList.h"
#include "GuiButton.h"
#include "GuiFloatField.h"
#include "GuiDoubleField.h"
#include "GuiPanner.h"

#include "GuiSlider2D.h"

class ofxXmlGui: public GuiContainer {

public:	
	void addGui(GuiContainer *c) {
		GuiListener **l = c->listeners;
		int numL = c->numListeners;
		add(c);
		c->listeners = l;
		c->numListeners = numL;
	}
	
	GuiSlider *addSlider(string name, float &ptr, float minValue, float maxValue) {
		GuiSlider *slider = (GuiSlider*)add("slider", name, name);
		slider->min = minValue;
		slider->max = maxValue;
		slider->value = &ptr;
		slider->width = size;
		slider->showValue = true;
		return slider;
	}

	GuiSlider2D *addSlider2D(string name, ofPoint &ptr, float minX, float maxX, float minY, float maxY) {
		GuiSlider2D *slider2d = (GuiSlider2D*)add("slider2d", name, name);
		slider2d->minX = minX;
		slider2d->maxX = maxX;
		slider2d->minY = minY;
		slider2d->maxY = maxY;
		slider2d->showValue = true;
		slider2d->value = &ptr[0];
		return slider2d;
	}
	
	GuiToggle *addToggle(string name, bool &ptr) {
		GuiToggle *slider = (GuiToggle*)add("toggle", name, name);		
		slider->value = &ptr;
		slider->width = size;
		return slider;
	}
	
	GuiSegmentedControl *addSegmentedControl(string name, int &value, string options) {
		GuiSegmentedControl * sc = (GuiSegmentedControl*)add("segmented", name, name);
		sc->options = options;
		sc->value = &value;
		sc->width = size;
		sc->load();
		return sc;
	}
	
	GuiList *addList(string name, int &value, string options) {
		GuiList * gl = (GuiList*) add("list", name, name);
		gl->options = options;
		gl->value = &value;
		gl->width = size;
		gl->load();
		return gl;
	}
	
	GuiButton *addButton(string name) {
		GuiButton *button = (GuiButton*)add("button", name, name);
		button->width = size;
		return button;
	}
	GuiFloatField *addFloatField(string name, float &value) {
		GuiFloatField *ff = (GuiFloatField*)add("floatfield", name, name);
		ff->value = &value;
		ff->width = size;
		return ff;
	}
	
	GuiDoubleField *addDoubleField(string name, double &value) {
		GuiDoubleField *ff = (GuiDoubleField*)add("doublefield", name, name);
		ff->value = &value;
		ff->width = size;
		return ff;
	}
	
	GuiSlider *addPanner(string name, float &ptr, float minValue = -1, float maxValue = 1) {
		GuiPanner *slider = (GuiPanner*)add("panner", name, name);
		slider->min = minValue;
		slider->max = maxValue;
		slider->value = &ptr;
		slider->width = size;
		slider->showValue = true;
		return slider;
	}	

	void __draw(ofEventArgs &e) {
		draw();
	}
	
	void mousePressed(ofMouseEventArgs &e) {
		touchDown(e.x, e.y, e.button);
	}
	
	void mouseMoved(ofMouseEventArgs &e) {
		touchOver(e.x, e.y, e.button);
	}
	
	void mouseDragged(ofMouseEventArgs &e) {
		touchMoved(e.x, e.y, e.button);
	}
	
	void mouseReleased(ofMouseEventArgs &e) {
		if(touchUp(e.x, e.y, e.button) && saving) {
			saveValues(xmlFile);
		}
	}
	
	void setAutoSaveFile(string file) {
		xmlFile = file;
		saving = true;
		// try to load settings for current gui
		loadValues(xmlFile);
	}
	
	
	void toggle() {
		
		if(visible) disable();
		else enable();
	}
	
	bool isEnabled() {
		return visible;
	}
	void enable() {
		ofAddListener(ofEvents.mousePressed, this, &ofxXmlGui::mousePressed);
		ofAddListener(ofEvents.mouseMoved, this, &ofxXmlGui::mouseMoved);
		ofAddListener(ofEvents.mouseDragged, this, &ofxXmlGui::mouseDragged);
		ofAddListener(ofEvents.mouseReleased, this, &ofxXmlGui::mouseReleased);
		ofAddListener(ofEvents.draw, this, &ofxXmlGui::__draw);
		show();
	}
	
	
	void disable() {
		ofRemoveListener(ofEvents.mousePressed, this, &ofxXmlGui::mousePressed);
		ofRemoveListener(ofEvents.mouseMoved, this, &ofxXmlGui::mouseMoved);
		ofRemoveListener(ofEvents.mouseDragged, this, &ofxXmlGui::mouseDragged);
		ofRemoveListener(ofEvents.mouseReleased, this, &ofxXmlGui::mouseReleased);
		ofRemoveListener(ofEvents.draw, this, &ofxXmlGui::__draw);
		hide();
	}
	void setup(float _x = 20, float _y = 20, float size = 150, int layoutType = LAYOUT_SIMPLE_GUI) {
		setLayoutType(layoutType);
		x = _x;
		y = _y;

		width = size;
		this->size = size - paddingX*2;
		xmlFile = "";
		saving = false;
		disable();
	}
private:
	float size;
	string xmlFile;
	bool saving;
};
