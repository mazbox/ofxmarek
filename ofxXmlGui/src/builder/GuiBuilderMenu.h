/*
 *  GuiMenu.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *  Copyright 2010 Apple Inc. All rights reserved.
 *
 */
#pragma once

//#define GLUT_API_VERSION

#include "GuiMenu.h"
#include "GuiBuilderInspector.h"

#define NEW_BUTTON "NEW_BUTTON"
#define NEW_CONTROL "NEW_CONTROL"
#define NEW_SLIDER "NEW_SLIDER"
#define TEST_GUI "TEST_GUI"
#define LOAD "LOAD"
#define SAVE "SAVE"
#define SAVE_AS "SAVE_AS"

#define SAVE_GUI "SAVE_GUI"
#define LOAD_GUI "LOAD_GUI"
#define NEW_TOGGLE "NEW_TOGGLE"
#define NEW_TEXTFIELD "NEW_TEXTFIELD"
#define DUPLICATE "DUPLICATE"
#define DELETE "DELETE"
#define NEW_FPS "NEW_FPS"
#define CLEAR "CLEAR"
#define NEW_2DSLIDER "NEW_2DSLIDER"
#define NEW_INTFIELD "NEW_INTFIELD"
#define NEW_IMAGE "NEW_IMAGE"
#define SEND_TO_BACK "SEND_TO_BACK"
#define SEND_TO_FRONT "SEND_TO_FRONT"
#define NEW_IMAGE_BUTTON "NEW_IMAGE_BUTTON"
#define NEW_IMAGE_TOGGLE "NEW_IMAGE_TOGGLE"
#define NEW_SEGMENTED "NEW_SEGMENTED"
#define XML_FILE "XML_FILE"
#define ADJUST_BOUNDS "ADJUST_BOUNDS"


/** 
 * GuiBuilderMenu can't be a listener because GuiMenus are. 
 * So this weird bit of code gets round it.
 */
class EventProxy {
public: 
	virtual void controlHasChanged(GuiControl *control) = 0;
};

class BuilderListener: public GuiListener {
public:
	EventProxy *proxy;
	virtual void controlChanged(GuiControl *control) {
		proxy->controlHasChanged(control);
	}
};
class GuiBuilderMenu;
#ifdef GLUT_API_VERSION
void processMenuEvents(int eventId);
void addGlutMenus(GuiBuilderMenu *menu);
#endif

#ifdef TARGET_OSX
enum kDialogType {
	kDialogFile,
	kDialogFolder
};
string getStringFromDialog(kDialogType type, char* windowTitle, char* windowMessage);
string getStringFromSaveDialog (char* windowTitle, char* windowMessage);
#endif

class GuiBuilderMenu: public GuiMenu, public EventProxy {
public:
	
	
	bool editMode;
	bool adjustBounds;
	ofPoint lastMousePress;
	string file;
	GuiContainer *gui;
	
	GuiBuilderInspector *inspector;
	BuilderListener _listener;
	GuiBuilderMenu() {
		lastMousePress = ofPoint(0, 0);
		x = 0;
		y = 0;
		_listener.proxy = this;
		
		
		editMode = true;
		adjustBounds = false;
		file = "gui.xml";

		
		addMenu("File");
		{
			
			addItem("File", "Open...", LOAD);
			addItem("File", "Save", SAVE);
			addItem("File", "Save As...", SAVE_AS);

#ifndef TARGET_OSX			
			addItem("File", "XML File", XML_FILE, "textfield");
			addItem("File", "Save GUI", SAVE_GUI);
			addItem("File", "Load GUI", LOAD_GUI);
#endif
		}
		
		addMenu("Controls");
		addMenu("Containers");
		{
			
			
			vector<string> types = GuiControl::getTypes();
			for(int  i = 0; i < types.size(); i++) {
				GuiControl *ctrl = add(types[i], "deleteMeStraightAway!","deleteMeStraightAway!");
				if(ctrl->isContainer()) {
					addItem("Containers", types[i], "new_"+types[i]);
				} else {
					addItem("Controls", types[i], "new_"+types[i]);
				}
				remove(ctrl->controlId);
				
				
			}
		}
		
		
				
		addMenu("Edit");
		{
			addItem("Edit", "Edit mode", TEST_GUI, "toggle");
			addItem("Edit", "Adjust bounds", ADJUST_BOUNDS, "toggle");
			addItem("Edit", "Duplicate", DUPLICATE);
			addItem("Edit", "Delete", DELETE);
			addItem("Edit", "Clear", CLEAR);
		}
		
		
		addMenu("Arrange");
		{
			addItem("Arrange", "Send to back", SEND_TO_BACK);
			addItem("Arrange", "Send to front", SEND_TO_FRONT);
		}
		
#ifdef GLUT_API_VERSION
		addGlutMenus(this);
		printf("Generated GLUT Contextual menus\n");
#endif


		height += 20;
		
		saveToXml("guigui.xml");
		
		
		//guigui->load("guigui->xml");
		setMovable(true);
		addListener(&_listener);
		
		pointToValue(TEST_GUI, &editMode);
		
#ifndef TARGET_OSX
		pointToValue(XML_FILE, &file);
#endif
		pointToValue(ADJUST_BOUNDS, &adjustBounds);
		
	}
	
	
	void touchOver(int _x, int _y, int tid) {
		GuiContainer::touchOver(_x, _y, tid);
		lastMousePress = ofPoint(_x, _y);
	}

	void controlHasChanged(GuiControl *control) {
		string createNewType = "";
		string controlId = control->controlId;
		if(controlId==NEW_BUTTON) {
			
			createNewType = "button";
			
		} else if(controlId==SAVE_GUI) {
			gui->saveToXml(file);
		} else if(controlId==LOAD_GUI) {
			gui->loadFromXml(file);
			gui->x = 100;
		
		
			
			
#ifdef TARGET_OSX
			

			//string getStringFromSaveDialog (char* windowTitle, char* windowMessage);

		} else if(controlId==LOAD) {
			string path = getStringFromDialog(kDialogFile, "Open...", "Select the gui xml file");
			if(path!="" && path.find(".xml")!=-1) {
				gui->loadFromXml(path);
				file = path;
			}
		} else if(controlId==SAVE) {
			gui->saveToXml(file);

		} else if(controlId==SAVE_AS) {
			string path = getStringFromSaveDialog ("Save...", "Save your gui xml file");
			if(path!="" && path.find(".xml")!=-1) {
				gui->saveToXml(file);
			}
	#endif		
			
		} else if(controlId==ADJUST_BOUNDS) {
			gui->setAdjustBounds(adjustBounds);
		} else if(controlId.find("new_")==0) {
			createNewType = controlId.substr(4);
		} else if(controlId==DUPLICATE) {
			gui->duplicateFocusedControl();
		} else if(controlId==DELETE) {
			inspector->clear();
			gui->deleteFocusedControl();
		} else if(controlId==CLEAR) {
			gui->clear();
		} else if(controlId==SEND_TO_BACK) {
			gui->sendToBack();
		} else if(controlId==SEND_TO_FRONT) {
			gui->sendToFront();
		
		} else if(controlId==TEST_GUI) {
			gui->setEditing(editMode);
		}
		
		
		if(createNewType!="") {
			addElement(createNewType, "new " + createNewType, 100, 100);
			
			
		}
	}
	
	void addElement(string type, string cid, int _x, int _y) {
		GuiControl *ctrl = gui->add(type, cid, cid);
		ctrl->setEditing(editMode);
		ctrl->x = _x;
		ctrl->y = _y;
	}
	
	void setGui(GuiContainer *g) {
		gui = g;
		gui->loadFromXml(file);
		gui->setMovable(true);
		gui->x = 120;
		gui->y = 40;
		if(gui->width<50) gui->width = 50;
		if(gui->height<50) gui->height = 50;
		gui->setEditing(editMode);
	}
};