/**
 * ofGuiEventDispatcher.cpp
 * emptyExample
 *
 * Created by Marek Bereza on 16/06/2011.
 *
 */

#include "ofGuiEventDispatcher.h"
#include "Container.h"
gui::ofGuiEventDispatcher::ofGuiEventDispatcher() {
	this->enabled = false;
}

void gui::ofGuiEventDispatcher::setup(Container *root) {
	this->root = root;
}
void gui::ofGuiEventDispatcher::draw(ofEventArgs &e) {
	root->draw();
}


void gui::ofGuiEventDispatcher::mousePressed(ofMouseEventArgs &e) {

	root->touchDown(e.x, e.y, e.button);
}

void gui::ofGuiEventDispatcher::mouseMoved(ofMouseEventArgs &e) {
	root->touchOver(e.x, e.y, e.button);
}

void gui::ofGuiEventDispatcher::mouseDragged(ofMouseEventArgs &e) {
	root->touchMoved(e.x, e.y, e.button);
}

void gui::ofGuiEventDispatcher::mouseReleased(ofMouseEventArgs &e) {
	root->touchUp(e.x, e.y, e.button);
}

void gui::ofGuiEventDispatcher::keyPressed(ofKeyEventArgs &e) {
	root->keyPressed(e.key);
}

void gui::ofGuiEventDispatcher::keyReleased(ofKeyEventArgs &e) {
	root->keyReleased(e.key);
}


void gui::ofGuiEventDispatcher::touchDown(ofTouchEventArgs &e) {
	root->touchDown(e.x, e.y, e.id);
}

void gui::ofGuiEventDispatcher::touchMoved(ofTouchEventArgs &e) {
	root->touchMoved(e.x, e.y, e.id);
}

void gui::ofGuiEventDispatcher::touchUp(ofTouchEventArgs &e) {
	root->touchUp(e.x, e.y, e.id);
}

void gui::ofGuiEventDispatcher::enableEvents() {

	ofAddListener(ofEvents.mousePressed, this, &gui::ofGuiEventDispatcher::mousePressed);
	ofAddListener(ofEvents.mouseMoved, this, &gui::ofGuiEventDispatcher::mouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &gui::ofGuiEventDispatcher::mouseDragged);
	
	ofAddListener(ofEvents.touchDown, this, &gui::ofGuiEventDispatcher::touchDown);
	ofAddListener(ofEvents.touchUp, this, &gui::ofGuiEventDispatcher::touchUp);
	ofAddListener(ofEvents.touchMoved, this, &gui::ofGuiEventDispatcher::touchMoved);
	
	ofAddListener(ofEvents.mouseReleased, this, &gui::ofGuiEventDispatcher::mouseReleased);
	ofAddListener(ofEvents.draw, this, &gui::ofGuiEventDispatcher::draw);
	
	ofAddListener(ofEvents.keyPressed, this, &gui::ofGuiEventDispatcher::keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &gui::ofGuiEventDispatcher::keyReleased);
	
}

void gui::ofGuiEventDispatcher::disableEvents() {
	ofRemoveListener(ofEvents.mousePressed, this, &gui::ofGuiEventDispatcher::mousePressed);
	ofRemoveListener(ofEvents.mouseMoved, this, &gui::ofGuiEventDispatcher::mouseMoved);
	ofRemoveListener(ofEvents.mouseDragged, this, &gui::ofGuiEventDispatcher::mouseDragged);
	
	ofRemoveListener(ofEvents.touchDown, this, &gui::ofGuiEventDispatcher::touchDown);
	ofRemoveListener(ofEvents.touchUp, this, &gui::ofGuiEventDispatcher::touchUp);
	ofRemoveListener(ofEvents.touchMoved, this, &gui::ofGuiEventDispatcher::touchMoved);
	
	ofRemoveListener(ofEvents.mouseReleased, this, &gui::ofGuiEventDispatcher::mouseReleased);
	ofRemoveListener(ofEvents.draw, this, &gui::ofGuiEventDispatcher::draw);
	
	ofRemoveListener(ofEvents.keyPressed, this, &gui::ofGuiEventDispatcher::keyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &gui::ofGuiEventDispatcher::keyReleased);
}

bool gui::ofGuiEventDispatcher::isEnabled() {
	return enabled;
}
void gui::ofGuiEventDispatcher::setEnabled(bool enabled) {

	if(enabled!=this->enabled) {
		if(enabled) {
			enableEvents();
		} else {
			disableEvents();
		}
	}
	this->enabled = enabled;
}