/**
 * Editor.cpp
 * emptyExample
 *
 * Created by Marek Bereza on 16/06/2011.
 *
 */

#include "Editor.h"

gui::Editor::Editor(): Container() {
	enabled = false;
	resizing = false;
	focusedControl = NULL;
	rolledOverControl = NULL;
	addListener(this);
}
void gui::Editor::controlChanged(Control *c) {
	string n = c->id.c_str();
	if(n.find("new ")==0) {
		string ctrlType = n.substr(4);
		Control *ctrl = INSTANTIATE(ctrlType);
		ctrl->x = 100;
		ctrl->y = 100;
		Control *con = root->getControlById("root");
		if(con!=NULL) {
			((Container*)con)->addChild(ctrl);
		} else {
			root->addChild(ctrl);
		}
		
	} else if(n=="Save") {
		root->saveToXml("gui.xml");
	} else if(n=="Save As...") {
		ofFileDialogResult result = ofSystemSaveDialog("default.xml", "Save As...");
		if(result.bSuccess) {
			root->saveToXml(result.filePath);
		}
		
	} else if(n=="Delete") {
		
		if(focusedControl != NULL && focusedControl->parent!=NULL) {
			focusedControl->parent->removeChild(focusedControl);
			if(focusedControl==rolledOverControl) {
				rolledOverControl = NULL;
			}
			
			// let the inspector know we're deleting a control in case
			// its gui is pointing to it.
			inspector.setControl(NULL);
			delete focusedControl;
			focusedControl = NULL;
		}
	} else if(n=="Duplicate") {
		if(focusedControl != NULL && focusedControl->parent!=NULL) {
			Control *newControl = focusedControl->clone();
			newControl->x += 10;
			newControl->y += 10;
			newControl->id += "1";
			focusedControl->parent->addChild(newControl);
			focusedControl = newControl;
		}
	}
}
void gui::Editor::setup(Container *root) {
	addChild(root);
	this->root = root;
	eventDispatcher.setup(this);
	
	setEnabled(true);
	setupMenus();
	addChild(&inspector);
}
/*
void gui::Editor::setupInspector() {
	inspector = new Inspector();
	addChild(inspector);
}*/
void gui::Editor::setupMenus() {
	menuBar = (MenuBar*)INSTANTIATE("menubar");
	
	menuBar->addMenu("File");
	menuBar->addItem("File", "Save");
	menuBar->addItem("File", "Save As...");
	menuBar->addMenu("Edit");
	//menuBar->addItem("Edit", "Duplicate");
	menuBar->addItem("Edit", "Delete");
	menuBar->addItem("Edit", "Duplicate");
	
	menuBar->addMenu("Create");
	
	for(int i = 0; i < Instantiator::getInstance()->size(); i++) {
		menuBar->addItem("Create", string("new ") + Instantiator::getInstance()->at(i));
	}
	
	
	addChild(menuBar);
}


void gui::Editor::setEnabled(bool enable) {
	this->enabled = enable;
	if(this->enabled) {
		eventDispatcher.enableEvents();
	} else {
		eventDispatcher.disableEvents();
	}
}

bool gui::Editor::isEnabled() { return enabled; }


void gui::Editor::touchOver(int x, int y, int id) {
	menuBar->touchOver(x, y,id);
	inspector.touchOver(x, y,id);
	Control *ctrl = root->hitTest(x, y);

	if(ctrl!=NULL) {
		rolledOverControl = ctrl;
		ofVec2f pos = ctrl->getAbsolutePosition();
		// find out if the mouse is over the bottom right hand corner
	
		// bottom right
		pos += ofVec2f(ctrl->width, ctrl->height);
		pos -= ofVec2f(x, y);
		if(pos.x<5 && pos.y<5) {
			printf("Corner\n");
		}
		
	} else {
		rolledOverControl = NULL;
	}
}


bool gui::Editor::touchDown(int x, int y, int id) {

	if(menuBar->touchDown(x, y,id)) return true;
	if(inspector.touchDown(x, y,id)) return true;
	rolledOverControl = NULL;
	Control *ctrl = root->hitTest(x, y);
	focusedControl = ctrl;
	if(focusedControl!=NULL) {
		printf("Hit control: %s\n", ctrl->id.c_str());
		inspector.setControl(focusedControl);
		lastMousePosition = ofVec2f(x, y);
		
		
		ofVec2f pos = focusedControl->getAbsolutePosition();
		// find out if the mouse is over the bottom right hand corner
		
		// bottom right
		pos += ofVec2f(focusedControl->width, focusedControl->height);
		pos -= ofVec2f(x, y);
		if(focusedControl->scalable && pos.x<5 && pos.y<5) {
			resizing = true;
		} else {
			resizing = false;
		}
		
	}
	return false;
}

bool gui::Editor::touchMoved(int x, int y, int id) {
	if(menuBar->touchMoved(x, y,id)) return true;
	if(inspector.touchMoved(x, y,id)) return true;
	// do dragging
	if(focusedControl!=NULL) {
		ofVec2f delta = ofVec2f(x, y) - lastMousePosition;
		if(resizing) {
			focusedControl->width += delta.x;
			focusedControl->height += delta.y;
		} else {
			focusedControl->x += delta.x;
			focusedControl->y += delta.y;
		}
		lastMousePosition = ofVec2f(x, y);
		
	}
	return false;
}
bool gui::Editor::touchUp(int x, int y, int id) {
	if(menuBar->touchUp(x, y,id)) return true;
	if(inspector.touchUp(x, y,id)) return true;
	return false;
}

void gui::Editor::draw() {
	Container::draw();
	if(focusedControl!=NULL) {
		ofNoFill();
		ofSetHexColor(0xFFFFFF);
		ofVec2f pos = focusedControl->getAbsolutePosition();
		ofRect(pos.x, pos.y, focusedControl->width, focusedControl->height);
		ofFill();
	}
	
	if(rolledOverControl!=NULL) {
		ofSetHexColor(0xFFFFFF);
		ofVec2f pos = rolledOverControl->getAbsolutePosition();
		ofRect(pos.x + rolledOverControl->width-5, pos.y+rolledOverControl->height-5, 5, 5);

		ofSetHexColor(0x002244);
		ofRect(0, ofGetHeight()-20, ofGetWidth(), 20);
		string path = "";
		Control *c = rolledOverControl;
		while(c!=NULL && c->parent!=NULL && c->parent->parent!=NULL) {
			if(path!="") path = "  >>  " + path;
			string idStr = "";
			if(c->id!="") idStr = "['" +c->id+ "']";
			path = c->type + idStr + path;
			c = c->parent;
		}
		ofSetHexColor(0xFFFFFF);
		gui::Resources::drawString(path, 3, ofGetHeight()-4);
	}
}