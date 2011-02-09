/*
 *  Gui.cpp
 *  Gui
 *
 *  Created by Marek Bereza on 12/02/2010.
 *
 */


#include "GuiContainer.h"
#include <sys/stat.h>

GuiContainer::GuiContainer() {
	focusedControl = NULL;
	layoutType = LAYOUT_ABSOLUTE;
	paddingX = paddingY = DEFAULT_PADDING;
	layoutOffsetX = layoutOffsetY = 0;
	adjustingBounds = false;
	movable = false;
	moving = false;
	listeners = NULL;
	numListeners = 0;
	resources = &res;
	bg = NULL;
	bgImgUrl = "";
	autoSaving = false;
}
/**
 * This points a control's value to the parameter
 */
void GuiContainer::pointToValue(string _controlId, void *ptr) {
	getControlById(_controlId)->value = ptr;
}

GuiControl *GuiContainer::getControlById(string cid) {
	
	for(int i = 0; i < controls.size(); i++) {
		
		if(controls[i]->controlId==cid) return controls[i];
		
		
		// if there's nested controls, dive into the next level
		if(controls[i]->isContainer()) {
			GuiContainer *c = (GuiContainer*)controls[i];
			GuiControl *ctrl = c->getControlById(cid);
			if(ctrl!=NULL) return ctrl;
		}
	}
	printf("Error, could not find control for id: %s\n", cid.c_str());
	return NULL;
}

void GuiContainer::setAdjustBounds(bool _adjustingBounds) {
	if(adjustingBounds==_adjustingBounds) return;
	adjustingBounds = _adjustingBounds;
	for(int i = 0; i < controls.size(); i++) {
		controls[i]->enabled = !adjustingBounds;
	}
}

void GuiContainer::setMovable(bool _movable) {
	movable = _movable;
}

void GuiContainer::setLayoutType(int _t) {
	layoutType = _t;
}


bool guiFileExists(string filename) { 
	struct stat stFileInfo; 
	
	// Attempt to get the file attributes 
	return stat(filename.c_str(),&stFileInfo)==0; 
}


void GuiContainer::enableAutoSave(string file) {
	autoSaving = true;
	autoSaveFile = file;
	loadValues(file);
}

void GuiContainer::disableAutoSave() {
	autoSaving = false;
}
/**
 * loads an xml file with the values specified
 * in the file. If the file doesn't exist, it just
 * returns.
 */
void GuiContainer::loadValues(string file) {
	
	
	if(!guiFileExists(ofToDataPath(file))) return;
	ofxXmlSettings xml;
	xml.loadFile(file);
	xml.pushTag("values");
	int numVals = xml.getNumTags("value");
	for(int i = 0; i < numVals; i++) {
		string controlId = xml.getAttribute("value", "controlId", "", i);
		if(controlId!="") {
			GuiControl *ctrl = getControlById(controlId);
			if(ctrl!=NULL) {
				ctrl->valueFromString(xml.getAttribute("value", "value", "", i));
			}
		}
	}
}

/**
 * Saves xml version of the values of all the controls.
 * this can be loaded again with loadValues().
 */
void GuiContainer::saveValues(string file) {
	ofxXmlSettings xml;
	xml.addTag("values");
	xml.pushTag("values");
	for(int i = 0; i < controls.size(); i++) {
		xml.addTag("value");
		xml.addAttribute("value", "controlId", controls[i]->controlId, i);
		xml.addAttribute("value", "value", controls[i]->valueToString(), i);
	}
	
	xml.saveFile(file);
	
	
}

void GuiContainer::loadFromXml(string file) {
	controls.clear();
	ofxXmlSettings xml;
	res.init(file);
	xml.loadFile(file);
	loadBaseInfo("view", xml, 0);
	x = 0;
	y = 0;
	load();
	//movable = xml.getAttribute("view", "movable", 0, 0);
	xml.pushTag("view");
	
	int numControls = xml.getNumTags("control");
	for(int i = 0; i < numControls; i++) {
		GuiControl *c = instantiator(
				xml.getAttribute("control", "type", "", i), 
				xml.getAttribute("control", "name", "", i), 
									 xml.getAttribute("control", "id", "", i), &res, this);
		
		string valueStr = xml.getAttribute("control", "value", "", i);
		if(valueStr!="") {
			c->valueFromString(valueStr);
		}
		c->loadBaseInfo("control", xml, i);
		c->load();
		c->listeners = listeners;
		c->numListeners = numListeners;
		c->load(xml, i);
		
		add(c);
	}
}

void GuiContainer::saveToXml(string file) {
	ofxXmlSettings xml;
	xml.addTag("view");
	
	saveBaseInfo("view", xml, 0);
	xml.addAttribute("view", "movable", (int)movable, 0);
	xml.pushTag("view");
	for(int i = 0; i < controls.size(); i++) {
		xml.addTag("control");
		controls[i]->saveBaseInfo("control", xml, i);
		controls[i]->save(xml, i);
	}
	xml.saveFile(file);
}


void GuiContainer::remove(string cid) {
	for(int i = 0; i < controls.size(); i++) {
		if(controls[i]->controlId==cid) {
			delete controls[i];
			controls.erase(controls.begin()+i);
			return;
		}
	}
}
vector<GuiControl*> GuiContainer::getControls() {
	return controls;
}
void GuiContainer::draw() {
	if(!visible) return;
	ofFill();
	glPushMatrix();
	glTranslatef(x, y, 0);
	if(bg!=NULL) {
		ofSetHexColor(0xFFFFFF);
		bg->draw(0, 0);
	} else {

		ofSetHexColor(OFXXMLGUI_DEFAULT_BG_COLOR);
		ofRect(0, 0, width, height);
	}
	for(int i = 0; i < controls.size(); i++) {
		if(controls[i]->isContainer()) {
			controls[i]->draw();
		} else {
			controls[i]->_draw();
		}
	}
	glPopMatrix();
}


GuiControl *GuiContainer::add(string _type, string _name, string _controlId) {
	GuiControl *ctrl = instantiator(_type, _name, _controlId, &res, this);
	
	
	
	ctrl->load();
	add(ctrl);
	return ctrl;
}



void GuiContainer::addListener(GuiListener *_listener) {
	
	
	GuiListener **nl = new GuiListener*[numListeners+1];
	int l;
	
	for(l = 0; l < numListeners; l++) {
		nl[l] = listeners[l];
		
	}
	nl[numListeners] = _listener;
	numListeners++;
	delete listeners;
	listeners = nl;
	
	propagateListeners(listeners, numListeners);
	
}

// this copies all the listeners coming in to this and all children
void GuiContainer::propagateListeners(GuiListener **listeners, int numListeners) {
	
	this->listeners = listeners;
	this->numListeners = numListeners;
	for(int l = 0; l < controls.size(); l++) {
		if(controls[l]->isContainer()) {
			((GuiContainer*)controls[l])->propagateListeners(listeners, numListeners);
		} else {
			controls[l]->listeners = listeners;
			controls[l]->numListeners = numListeners;
		}
	}
}

void GuiContainer::touchOver(int _x, int _y, int touchId) {
	if(!enabled) return;
	for(int i = controls.size()-1; i >= 0; i--) {
		if(controls[i]->isContainer()) { // if we're a container, call touchOver to propagate to children.
			controls[i]->touchOver(_x-x, _y-y, touchId);
		} else {
			controls[i]->_touchOver(_x-x, _y-y, touchId);
		}
	}
}

//--------------------------------------------------------------
bool GuiContainer::touchMoved(int _x, int _y, int button) {
	if(!enabled) return false;
	if(!moving) {
		bool foundFocus = false;
		for(int i = controls.size()-1; i >= 0; i--) {
			if(controls[i]->isContainer()) { // if we're a container, call touchMoved to propagate to children.
				foundFocus = controls[i]->touchMoved(_x-x, _y-y, button);
			} else {
				foundFocus = controls[i]->_touchMoved(_x-x, _y-y, button);
			}
			if(foundFocus) return true;
		}
	}
	
	// we found no focus, so run edit
	if((editing || movable) && moving) {
		if(adjustingBounds) {
			width += _x - editPoint.x;
			height += _y - editPoint.y;
		} else {
			x += _x - editPoint.x;
			y += _y - editPoint.y;
		}
		editPoint = ofPoint(_x, _y);
	}
	return false;
}


//--------------------------------------------------------------
bool GuiContainer::touchDown(int _x, int _y, int button) {
	if(!enabled) return false;
	bool foundFocus = false;

	for(int i = controls.size()-1; i >= 0; i--) {
		
		
		
		
		if(controls[i]->isContainer()) { // if we're a container, call touchDown to propagate to children.
			foundFocus = controls[i]->touchDown(_x-x, _y-y, button);
			
		} else {
			foundFocus = controls[i]->_touchDown(_x-x, _y-y, button);

		}
		if(focusedControl!=NULL) focusedControl->focus = false;
		focusedControl = controls[i];
		focusedControl->focus = true;
		if(foundFocus) return true;
	}
	// we found no focus, so run edit
	if((editing || movable) && hitTest(_x, _y)) {
		editPoint = ofPoint(_x, _y);
		moving = true;
		if(listeners!=NULL) {
			for(int i = 0; i < numListeners; i++) {
				listeners[i]->controlChanged(this);
			}
		}
		
	}
	return false;
}

//--------------------------------------------------------------
bool GuiContainer::touchUp(int _x, int _y, int button) {
	if(!enabled) return false;
	moving = false;
	bool foundFocus = false;
	
	for(int i = controls.size()-1; i >= 0; i--) {
		if(controls[i]->isContainer()) { // if we're a container, call touchUp to propagate to children.
			foundFocus = controls[i]->touchUp(_x-x, _y-y, button);
		} else {
			foundFocus = controls[i]->_touchUp(_x-x, _y-y, button);
		} 
		if(foundFocus) {
			if(autoSaving) {
				saveValues(autoSaveFile);
			}
			return true;
		}
	}
	return false;
}


void GuiContainer::setEditing(bool _editing) {
	if(editing==_editing) return;
	editing = _editing;
	for(int i = 0; i < controls.size(); i++) {
		controls[i]->setEditing(editing);
	}
}


vector<ParameterInfo> GuiContainer::getParameterInfo() {
	vector<ParameterInfo> params;
	params.push_back(ParameterInfo("BG Image", "bg","file", &bgImgUrl));
	return params;
	
}

void GuiContainer::reload() {
	load();
	for(int i = 0; i < controls.size(); i++) {
		controls[i]->load();
	}
}

void GuiContainer::load() {
	bg = resources->getImage(bgImgUrl);
	if(bg!=NULL) {
		printf("There is an image: %s\n", bgImgUrl.c_str());
		width = bg->getWidth();
		height = bg->getHeight();
	}
}
void GuiContainer::clear() {
	controls.clear();
	layoutOffsetX = 0;
	layoutOffsetY = 0;
}
void GuiContainer::duplicateFocusedControl() {
	if(focusedControl!=NULL) {
		
		ofxXmlSettings xml;
		xml.addTag("control");
		focusedControl->saveBaseInfo("control", xml, 0);
		focusedControl->save(xml, 0);
		
		GuiControl *ctrl = instantiator(focusedControl->type, focusedControl->name, focusedControl->controlId+"1", &res, this);
		
		ctrl->loadBaseInfo("control", xml, 0);
		ctrl->load();
		ctrl->load(xml, 0);
		ctrl->x += 20;
		ctrl->y += 20;
		ctrl->setEditing(editing);
		
		focusedControl->focus = false;
		focusedControl = ctrl;
		focusedControl->focus = true;
		add(focusedControl);
	}
}

int GuiContainer::indexForControl(GuiControl *c) {
	for(int i = 0; i < controls.size(); i++) {
		if(controls[i]==focusedControl) {
			return i;
		}
	}
	return -1;
}


void GuiContainer::deleteFocusedControl() {
	if(focusedControl==NULL) return;
	for(int i = 0; i < controls.size(); i++) {
		if(controls[i]==focusedControl) {
			controls.erase(controls.begin()+i);
			delete focusedControl;
			focusedControl = NULL;
			return;
		}
	}
}


void GuiContainer::sendToBack() {
	if(focusedControl!=NULL) {
		int ctrlI = indexForControl(focusedControl);
		if(ctrlI!=-1) {
			swap(controls[0], controls[ctrlI]);
		}
	}
}

void GuiContainer::sendToFront() {
	if(focusedControl!=NULL) {
		int ctrlI = indexForControl(focusedControl);
		if(ctrlI!=-1) {
			swap(controls[controls.size()-1], controls[ctrlI]);
		}
	}
}

void GuiContainer::sendBackwards() {
	if(focusedControl!=NULL) {
		int ctrlI = indexForControl(focusedControl);
		if(ctrlI>0) {
			swap(controls[ctrlI-1], controls[ctrlI]);
		}
	}
}

void GuiContainer::sendForwards() {
	if(focusedControl!=NULL) {
		int ctrlI = indexForControl(focusedControl);
		if(ctrlI<controls.size()-1 && ctrlI>-1) {
			swap(controls[ctrlI+1], controls[ctrlI]);
		}
	}
}





bool GuiContainer::keyPressed(int key) {
	if(focusedControl!=NULL) {
		
	
		if(key==9) { // tab
			for(int i = 0; i < controls.size(); i++) {
				if(controls[i]==focusedControl) {
					
#ifdef GLUT_ACTIVE_SHIFT
					if(glutGetModifiers() == 1) {
						if(i>0) {
							focusedControl = controls[i-1];
						} else {
							focusedControl = controls[controls.size()-1];
						}
					} else 
#endif
					{
						if(i+1<controls.size()) {
							focusedControl = controls[i+1];
						} else {
							focusedControl = controls[0];
						}
					}
				}
			}
			return true;
		}
		
		
		
		
		if(editing) {
			
			int inc = 1;
			
#ifdef GLUT_ACTIVE_SHIFT
			if(glutGetModifiers() == 1) inc = 10;
#endif
			switch(key) {
				case 357: // up
					focusedControl->y-=inc;
					break;
				case 359: // down
					focusedControl->y+=inc;
					break;
				case 356: // left
					focusedControl->x-=inc;
					break;
				case 358: // right
					focusedControl->x+=inc;
					break;
				case 127: // backspace
					deleteFocusedControl();
					break;
				case 'd':
				case 'D':
					//if(glutGetModifiers() == 2) { // control character
					//	duplicateFocusedControl();
					//}
					break;
			}
		} else {
			return focusedControl->keyPressed(key);
			
		}
	}
	return false;
}



void GuiContainer::add(GuiControl *c) {
	
	if(layoutType==LAYOUT_VERTICAL) {
		layoutOffsetY += paddingY;
		c->x = paddingX;
		c->y = layoutOffsetY;
		if(c->scalable) c->width = 110;
		layoutOffsetY += c->height;
		
		height = layoutOffsetY + paddingY;
		width = c->width + paddingX * 2;
	} else if(layoutType==LAYOUT_HORIZONTAL) {
		layoutOffsetX += paddingX;
		c->y = paddingY;
		c->x = layoutOffsetX;
		layoutOffsetX += c->width;
		width = layoutOffsetX + paddingX;
		height = c->height + paddingY * 2;
	} else if(layoutType==LAYOUT_SIMPLE_GUI) {
		if(layoutOffsetX==0) layoutOffsetX = paddingX;
		
		if(controls.size()>0) layoutOffsetY += controls.back()->height;
		// if we're about to fall off the bottom of the page,
		// wrap around to the next line.
		if(layoutOffsetY+y+c->height+paddingY>ofGetHeight()) { 
			float newWidth = controls.back()->width;
			if(newWidth==0) {
				for(int i = controls.size()-1; i>=0; i--) {
					if(controls[i]->width>0) {
						newWidth = controls[i]->width;
						break;
					}
				}
			}
			layoutOffsetX += newWidth+paddingX;
			layoutOffsetY = 0;
		}
		if((controls.size()>0 && controls.back()->height>0) || controls.size()==0)	
		layoutOffsetY += paddingY;
		c->x = layoutOffsetX;
		c->y = layoutOffsetY;
		if(c->scalable) c->width = 150;
		
		
		height = MAX(height, c->y+c->height+paddingY);
		width = MAX(width, c->x+c->width+paddingX);
	}
	if(c->listeners==NULL &&listeners!=NULL) {
		c->listeners = listeners;
		c->numListeners = numListeners;
	}
	controls.push_back(c);
}
int GuiContainer::numChildren() {
	return controls.size();
}