/**
 * Container.cpp
 * emptyExample
 *
 * Created by Marek Bereza on 16/06/2011.
 *
 */

#include "Container.h"
#include "Gui.h"
#include "Instantiator.h"
#define AUTO_LAYOUT_PADDING 5

gui::Container::Container(): Control() {
	type = "container";
	parent = NULL;
	layoutType = LayoutType_absolute;
	keyboardFocusedControl = NULL;

}

void gui::Container::clear() {
	deque<Control*>::iterator it;
	for(it = children.begin(); it != children.end(); it++) {
		delete (*it);
	}
	children.clear();
}
gui::Container::~Container() {
	clear();
}
void gui::Container::draw() {
	drawChildren();
}

void gui::Container::setLayoutType(LayoutType layoutType) {
	this->layoutType = layoutType;

}
void gui::Container::addChild(Control *child) {
	child->parent = this;
	if(layoutType==LayoutType_vertical) {
		if(children.size()>0) {
			child->x = children.back()->x;
			child->y = children.back()->y + children.back()->height + AUTO_LAYOUT_PADDING;
		}
	}
	//printf("Adding child %s, %d\n", child->name.c_str(), child->isContainer());
	children.push_back(child);
	if(child->isContainer()) {
		//printf("Cascading listeners to %s\n", child->name.c_str());
		for(int i = 0; i < listeners.size(); i++) {
			((Container*)child)->addListener(listeners[i]);
		}
	}
}

bool gui::Container::removeChild(Control *child) {
	deque<Control*>::iterator it;
	for(it = children.begin(); it != children.end(); it++) {
		if((*it)==child) {
			children.erase(it);
			if(child==keyboardFocusedControl) keyboardFocusedControl = NULL;
			return true;
		}
	}
	return false;
}


void gui::Container::setKeyboardFocus(Control *keyboardFocusedControl) {

	
	if(parent!=NULL) { // propagate down
		parent->setKeyboardFocus(keyboardFocusedControl);
		
	} else { // we're the root so act upon it
		if(this->keyboardFocusedControl!=NULL) this->keyboardFocusedControl->focus = false;
		keyboardFocusedControl->focus = true;
		this->keyboardFocusedControl = keyboardFocusedControl;
	}
}

void gui::Container::touchOver(int x, int y, int id) {
	x -= this->x;
	y -= this->y;
	deque<Control*>::iterator it;
	for(it = children.begin(); it != children.end(); it++) {
		(*it)->over = (*it)->inside(x, y);
		(*it)->touchOver(x, y, id);
	}
}

bool gui::Container::touchDown(int x, int y, int id) {
	x -= this->x;
	y -= this->y;
	deque<Control*>::iterator it;
	for(it = children.begin(); it != children.end(); it++) {
		(*it)->over = (*it)->down = (*it)->inside(x, y);
		// if you're a container, pass the touch down, if you're a control only pass the touch down if you're in the box

		if(((*it)->isContainer() || (*it)->inside(x, y)) && (*it)->touchDown(x, y, id)) {

			for(int i = 0; i < listeners.size(); i++) {
				listeners[i]->controlChanged((*it));
			}
			return true;
		}
	}
	return false;
}

bool gui::Container::touchMoved(int x, int y, int id) {
	x -= this->x;
	y -= this->y;
	deque<Control*>::iterator it;
	for(it = children.begin(); it != children.end(); it++) {
		(*it)->over = (*it)->down = (*it)->inside(x, y);
		if(((*it)->isContainer() || (*it)->inside(x, y)) && (*it)->touchMoved(x, y, id)) {
			for(int i = 0; i < listeners.size(); i++) {
				listeners[i]->controlChanged((*it));
			}
			return true;
		}
	}
	return false;
}

bool gui::Container::touchUp(int x, int y, int id) {
	x -= this->x;
	y -= this->y;
	deque<Control*>::iterator it;
	for(it = children.begin(); it != children.end(); it++) {
		(*it)->over = (*it)->down = false;
		if(((*it)->isContainer() || (*it)->inside(x, y)) && (*it)->touchUp(x, y, id)) {
			for(int i = 0; i < listeners.size(); i++) {
				listeners[i]->controlChanged((*it));
			}
			return true;
		}
	}
	return false;
}

void gui::Container::drawChildren() {
	glPushMatrix();
	glTranslatef(x, y, 0);
	deque<Control*>::iterator it;
	for(it = children.begin(); it != children.end(); it++) {
		(*it)->draw();
	}
	glPopMatrix();
}


gui::Control *gui::Container::hitTest(float x, float y) {
	x -= this->x;
	y -= this->y;
	deque<Control*>::iterator it;
	for(it = children.begin(); it != children.end(); it++) {
		Control *hit = (*it)->hitTest(x, y);
		if(hit!=NULL) return hit;
	}
	// if no children hit-test successfully try this actual container
	if(inside(x+this->x, y+this->y)) return this;
	return NULL;
}

void gui::Container::saveToXmlObject(ofxXmlSettings &xml) {
	printf("Saving a %s with %d children\n", type.c_str(), children.size());
	// this adds a tag of name 'type'
	Control::saveToXmlObject(xml);
	xml.pushTag(type);

	deque<Control*>::iterator it;
	for(it = children.begin(); it != children.end(); it++) {
		(*it)->saveToXmlObject(xml);
	}

}

void gui::Container::saveToXml(string file) {
	ofxXmlSettings xml;
	saveToXmlObject(xml);
	xml.saveFile(file);
}
/*
void gui::Container::loadFromXml(string file) {
	ofxXmlSettingsHelper xml;
	xml.loadFile(file);
	x = xml.getAttribute(type, "x", 0, 0);
	y = xml.getAttribute(type, "y", 0, 0);
	width = xml.getAttribute(type, "width", 0, 0);
	height = xml.getAttribute(type, "height", 0, 0);
	name = xml.getAttribute(type, "name", "", 0);
	vector<ParameterInfo> parameterInfo;
	getParameterInfo(parameterInfo);
	for(int i = 0; i < parameterInfo.size(); i++) {
		
	}
	
	xml.pushTag(type);
	loadFromXmlObject(xml);
	
}*/

void gui::Container::loadFromXmlObject(TiXmlElement *xml) {
	Control::loadFromXmlObject(xml);
	
	// now we've loaded this container, load the children
	TiXmlNode *childNode = xml->FirstChild();
	if(childNode!=NULL) {
		TiXmlElement *child = childNode->ToElement();
		for(child; child; child = child->NextSiblingElement()) {
			string tagName = child->ValueStr();
			Control *ctrl = INSTANTIATE(tagName);
			ctrl->loadFromXmlObject(child);
			addChild(ctrl);
		}
	}
}

bool gui::Container::keyPressed(int key) {
	if(keyboardFocusedControl!=NULL) {
		return keyboardFocusedControl->keyPressed(key);
	} else {
		return false;
	}
	
	/*
	deque<Control*>::iterator it;
	for(it = children.begin(); it != children.end(); it++) {
		if((*it)->keyPressed(key)) {
			return true;
		}
	}
	return false;
	 */
}
void gui::Container::addListener(Listener *listener) {
	deque<Control*>::iterator it;
	for(it = children.begin(); it != children.end(); it++) {
		if((*it)->isContainer()) {
			Container *c = (Container*)(*it);
			c->addListener(listener);
		}	
	}
	listeners.push_back(listener);
}

gui::Control *gui::Container::getControlById(string id) {
	deque<Control*>::iterator it;
	for(it = children.begin(); it != children.end(); it++) {
		if((*it)->id==id) {
			return (*it);
		}	
	}
	return NULL;
}

float gui::Container::getFloat(string controlId) {
}
int gui::Container::getInt(string controlId) {
}
bool gui::Container::getBool(string controlId) {
}
string gui::Container::getString(string controlId) {
	getControlById(controlId);
}