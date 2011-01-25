/*
 *  GuiMenu.cpp
 *  Gui
 *
 *  Created by Marek Bereza on 09/04/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include "GuiMenu.h"
#include "GuiButton.h"

GuiMenu::GuiMenu() {
	xOffset = 0;
	name = "menu";
	width = 0;
	height = 0;
	addListener(this);
}
void GuiMenu::draw() {
	ofSetHexColor(0xFFFFFF);
	ofRect(0,0, ofGetWidth(), 20);
	GuiContainer::draw();
	ofSetHexColor(0xCCCCCC);
	ofLine(0, 20, ofGetWidth(), 20);
	
}


void GuiMenu::addMenu(string menuName) {
	// this isn't how we want to do it - we want protected instantiation


	GuiContainer *c = new GuiContainer();
	c->paddingX = 0;
	c->paddingY = 1;
	c->name = menuName;
	c->controlId = menuName;
	c->setLayoutType(LAYOUT_VERTICAL);
	c->width = 110;
	c->addListener(this);
	c->visible = false;
	c->enabled = false;
	add(c);
	c->y = 20;
	c->x = xOffset;

	GuiButton *b = (GuiButton*)add("button", menuName, "menu"+menuName);
	b->x = xOffset;
	b->y = 0;
	xOffset += c->width;
	//addListener(this);
	menuIds.push_back(c->controlId);
}

void GuiMenu::addItem(string menuName, string itemName, string controlId, string itemType) {
	GuiContainer *m = (GuiContainer*)getControlById(menuName);
	if(m!=NULL) {
		m->add(itemType, itemName, controlId);
	} else {
		printf("GuiMenu: Error: couldn't find menu %s\n", menuName.c_str());
	}
}

void GuiMenu::controlChanged(GuiControl *control) {
	printf("Control %s\n", control->name.c_str());
	if(control->controlId.find("menu")!=-1) {
		// show hide menu
		GuiControl *m = getControlById(control->controlId.substr(4));
		if(m!=NULL) {
			for(int i = 0; i < menuIds.size(); i++) {
				GuiControl *mm = getControlById(menuIds[i]);
				if(mm!=NULL && mm!=m) mm->hide();
			}
			m->visible = m->enabled = !m->enabled;
		}
	} else {
		if(control->type=="button" || control->type=="toggle") {
			for(int i = 0; i < menuIds.size(); i++) {
				GuiControl *mm = getControlById(menuIds[i]);
				if(mm!=NULL) mm->visible = mm->enabled = false;
			}
		}
		for(int i = 0; i < numListeners; i++) {
			if(listeners[i]!=this) {
				listeners[i]->controlChanged(control);
			}
		}
	}
}
