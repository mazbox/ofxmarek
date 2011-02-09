/*
 *  GuiControl.cpp
 *  Gui
 *
 *  Created by Marek Bereza on 12/02/2010.
 *
 */



#include "GuiControl.h"


// how big the triangle in the bottom
// right hand corner is for scaling in
// edit mode.
#define SCALE_TRIANGLE_SIZE 10

// what kind of edit are we doing now?
#define EDIT_NONE 0
#define EDIT_MOVE 1
#define EDIT_SCALE 2
#define EDIT_CONSTRAINED_SCALE 2



GuiControl::GuiControl() {
	value = NULL;
	focus = false;
	listeners = NULL;
	numListeners = 0;
	controlId = "";
	
	editing = false;
	over = false;
	down = false;
	visible = true;
	enabled = true;
	editState = EDIT_NONE;
	type = "control";
	scalable = true;
	x = y = 0;
	width = height = 100;
	parent = NULL;
}


ofPoint GuiControl::getAbsoluteCoords() {
	if(parent==NULL) {
		return ofPoint(x, y);
	} else {
		return ofPoint(x, y) + parent->getAbsoluteCoords();
	}
}
void GuiControl::show() {
	visible = true;
	enabled = true;
}

void GuiControl::hide() {
	visible = false;
	enabled = false;
}

void GuiControl::saveBaseInfo(string tag, ofxXmlSettings xml, int pos) {
	
	
	xml.addAttribute(tag, "type", type, pos);
	xml.addAttribute(tag, "width", width, pos);
	xml.addAttribute(tag, "height", height, pos);
	xml.addAttribute(tag, "x", x, pos);
	xml.addAttribute(tag, "y", y, pos);
	xml.addAttribute(tag, "id", controlId, pos);
	xml.addAttribute(tag, "name", name, pos);
	xml.addAttribute(tag, "value", this->valueToString(), pos);
	vector<ParameterInfo> params = this->getParameterInfo();
	for(int i = 0; i < params.size(); i++) {
		GuiControl *g = instantiator(params[i].type, "", "", resources, this);
		g->value = params[i].value;
		xml.addAttribute(tag, params[i].xmlName, g->valueToString(), pos);
	}
}

void GuiControl::loadBaseInfo(string tag, ofxXmlSettings xml, int pos) {
	x = xml.getAttribute(tag, "x", 0.f, pos);
	y = xml.getAttribute(tag, "y", 0.f, pos);
	width = xml.getAttribute(tag, "width", 0.f, pos);
	height = xml.getAttribute(tag, "height", 0.f, pos);
	controlId = xml.getAttribute(tag, "id", "", pos);
	name = xml.getAttribute(tag, "name", "", pos);
	

	vector<ParameterInfo> params = this->getParameterInfo();

	for(int i = 0; i < params.size(); i++) {
		GuiControl *g = instantiator(params[i].type, "", "", resources, this);

		g->valueFromString(xml.getAttribute(tag, params[i].xmlName, "", pos));
		// this is re-pointing a pointer - what we actually want to do is copy
		memcpy(params[i].value, g->value, sizeof(void*));
		
	}
}


void GuiControl::draw() {
	if(down) ofSetColor(0, 100, 100);
	else if(over) ofSetColor(200, 100, 100);
	else ofSetColor(100, 0, 0);
	ofRect(x, y, width, height);
}

// default behaviour is to save data as an int
string GuiControl::valueToString() {
	if(value==NULL) return "";
	return ofToString(ival(value));
	
}
void GuiControl::valueFromString(string inp) {
	if(value==NULL) value = new int[1];
	ival(value) = atoi(inp.c_str());
}



vector<string> GuiControl::getTypes() {
	return types;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// SECRET METHODS HERE!!!
/////////////////////////////////////////////////////////////////////////////////////////////////////////


vector<string> GuiControl::types;
void GuiControl::addType(string _type) {
	for(int i = 0; i < types.size(); i++) {
		if(_type==types[i]) return;
	}
	types.push_back(_type);
}


void GuiControl::_draw() {
	if(!visible) return;
	if(focus && editing) {
		ofNoFill();
		//glEnable (GL_LINE_STIPPLE);
		ofSetColor(255, 255, 255);
		//glLineStipple(1, 0x0F0F);
		ofRect(x-1, y-1, width+2, height+2);
		//glDisable (GL_LINE_STIPPLE);
		ofFill();
	}
	

	this->draw();
	if(focus && editing && scalable) {
		ofSetColor(255, 255, 255);
		ofBeginShape();
		ofVertex(x+width, y+height);
		ofVertex(x+width-SCALE_TRIANGLE_SIZE, y+height);
		ofVertex(x+width, y+height-SCALE_TRIANGLE_SIZE);
		ofEndShape(true);
	}
}

void GuiControl::_touchOver(int _x, int _y, int touchId){
	if(!enabled) return;
	over = hitTest(_x, _y);
	touchOver(_x, _y, touchId);
}
//--------------------------------------------------------------
bool GuiControl::_touchMoved(int _x, int _y, int touchId){
	if(!enabled) return false;
	over = hitTest(_x, _y);
	if(over || (down && editing)) {
		if(editing) {
			if(editState==EDIT_MOVE) {
				x += _x - editPoint.x;
				y += _y - editPoint.y;
			} else if(editState==EDIT_SCALE) {
				width  += _x - editPoint.x;
				height += _y - editPoint.y;
				if(width<5) width = 5;
				if(height<5) height = 5;
			}
			editPoint = ofPoint(_x, _y);
		} else {
			down = over;
			touchMoved(_x, _y, touchId);
			if(listeners!=NULL) {

				for(int i = 0; i < numListeners; i++) {
					listeners[i]->controlChanged(this);
				}
			}
		}
	}
	return over;
}

//--------------------------------------------------------------
bool GuiControl::_touchDown(int _x, int _y, int touchId){
	if(!enabled) return false;
	

	down = over = hitTest(_x, _y);
	if(down) {
		if(editing) {
			editPoint = ofPoint(_x, _y);
			
			// todo this should be pixels not percent
			
			if(scalable && _x-x>width-SCALE_TRIANGLE_SIZE && _y-y>height-SCALE_TRIANGLE_SIZE) {
				editState = EDIT_SCALE;
			} else {
				editState = EDIT_MOVE;
			}
		} else {
			touchDown(_x, _y, touchId);
			
		}
		
		if(listeners!=NULL) {
			for(int i = 0; i < numListeners; i++) {
				listeners[i]->controlChanged(this);
			}
			//GuiControl * l = (GuiControl*) listener;
			//printf("%s\n",	l->name.c_str());
		}
	}
	return over;
}


//--------------------------------------------------------------
bool GuiControl::_touchUp(int _x, int _y, int button){
	if(!enabled) return false;
	over = hitTest(_x, _y);
	down = false;
	if(editing) {
		_touchMoved (_x, _y, button);
	} else {
		if(over) touchUp(_x, _y, button);
	}
	editState = EDIT_NONE;
	return over;
}



void maskOn(int _x, int _y, int _w, int _h) {
	glEnable(GL_SCISSOR_TEST);
	glScissor(_x, ofGetHeight()-(_y+_h), _w, _h);
}
void maskOff() {
	glDisable(GL_SCISSOR_TEST);
}

void GuiControl::makeHidden() {
	width = height = 0;
	x = y = 200;
	hide();
}
void GuiControl::setResources(GuiResources *res) {
	resources = res;
}

GuiControl *GuiControl::under(GuiControl *rel, float padding) {
	this->x = rel->x;
	this->y = rel->y + rel->height + padding;
	return this;
}

GuiControl *GuiControl::underRight(GuiControl *rel, float padding) {
	this->x = rel->x + rel->width - this->width;
	this->y = rel->y + rel->height + padding;
	return this;
}

GuiControl *GuiControl::size(float w, float h) {
	this->width = w;
	this->height = h;
	return this;
}

GuiControl *GuiControl::position(float x, float y) {
	this->x = x;
	this->y = y;
	return this;
}

// position to the right
GuiControl *GuiControl::right(GuiControl *rel, float padding) {
	this->y = rel->y;
	this->x = rel->x + rel->width + padding;
	return this;
}