/*
 *  Gui.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */
#pragma once

#define OFXXMLGUI_DEFAULT_BG_COLOR 0x1E2832

#include "GuiControl.h"
#include "ofxXmlSettings.h"
#include "GuiResources.h"

/**
 * For use with setLayoutType()
 */
#define LAYOUT_ABSOLUTE 1
#define LAYOUT_VERTICAL 2
#define LAYOUT_HORIZONTAL 3
#define LAYOUT_SIMPLE_GUI 4

/**
 * For vertical and horizontal layouts
 * - how much padding to use.
 */
#define DEFAULT_PADDING 5;


class GuiContainer: public GuiControl {
	
public:
	
	
	
	GuiContainer();
	
	/**
	 * This points a control's value to the parameter
	 */
	void pointToValue(string _controlId, void *ptr);
	
	/**
	 * Returns a control for a given id
	 */
	GuiControl *getControlById(string cid);
	
	/**
	 * If enabled, the gui is draggable
	 */
	void setMovable(bool _movable);
	
	/** 
	 * Set whether you want absolute, vertical or horizontal
	 * layouts - see defines at the top of the file 
	 */
	void setLayoutType(int _t);
	
	/**
	 * Load the gui from an xml file 
	 */
	void loadFromXml(string file = "gui.xml");
	
	/**
	 * Save the gui to an xml file 
	 */
	void saveToXml(string file = "gui.xml");
	
	/**
	 * loads an xml file with the values specified
	 * in the file. If the file doesn't exist, it just
	 * returns.
	 */
	void loadValues(string file = "values.xml");
	
	/**
	 * Saves xml version of the values of all the controls.
	 * this can be loaded again with loadValues().
	 */
	void saveValues(string file = "values.xml");
	/**
	 * Call this to draw the gui and its children.
	 */
	virtual void draw();
	
	/**
	 * This lets you add a control programatically
	 */
	GuiControl *add(string _type, string _name, string _controlId);
	
	/**
	 * Removes a control given its control ID
	 */
	void remove(string cid);
	
	/**
	 * Implement GuiListener and pass the object
	 * to this method to listen to events in this
	 * gui.
	 */
	void addListener(GuiListener *_listener);
	
	/**
	 * Automatically creates xml files of all control values
	 * every time you change them, and loads them on startup if
	 * the file exists.
	 */
	virtual void enableAutoSave(string file = "values.xml");
	
	/** 
	 * Disable auto save (off by default) 
	 */
	virtual void disableAutoSave();
	
	/**
	 * These methods propagate mouse actions and key
	 * presses to the child controls 
	 */
	virtual void touchOver(int _x, int _y, int touchId);
	virtual bool touchMoved(int _x, int _y, int touchId);
	virtual bool touchDown(int _x, int _y, int touchId);
	virtual bool touchUp(int _x, int _y, int touchId);	
	bool keyPressed(int key);
	
	// gui builder functions
	void setEditing(bool _editing);	
	void clear();
	void duplicateFocusedControl();
	void deleteFocusedControl();
	void setAdjustBounds(bool _adjustingBounds);
	void sendToBack();
	void sendToFront();
	void sendBackwards();
	void sendForwards();
	
	virtual bool isContainer() { return true; }
	
	virtual vector<ParameterInfo> getParameterInfo();
	
	
	// calls load on all the children
	void reload();
	
	// for linear (horiz/vert) layouts, this is the spacing.
	int paddingX;
	int paddingY;
	
	// loads a background if there is one.
	// (called internally)
	void load();
	
	
	vector<GuiControl*> getControls();
	
	// adds an actual element to the gui
	void add(GuiControl *c);
	
	int numChildren();
protected:
	

	
	bool moving;
	vector<GuiControl*> controls;
	GuiResources res;
	ofImage *bg;
	string bgImgUrl;
	int layoutType;
	void propagateListeners(GuiListener **listeners, int numListeners);
private:
	bool autoSaving;
	string autoSaveFile;
	bool adjustingBounds;
	
	
	int indexForControl(GuiControl *c);	
	
	bool movable;
	
	int layoutOffsetX;
	int layoutOffsetY;
	
	GuiControl *focusedControl;
};