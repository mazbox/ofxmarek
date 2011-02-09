/*
 *  ofxXmlGui.h
 *  GuiDownloader
 *
 *  Created by Marek Bereza on 28/03/2010.
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
#include "GuiTitle.h"
#include "GuiSlider2D.h"
#include "GuiDrawable.h"

/**
 * This is a simple wrapper for the ofxXmlGui library. It lets you build up
 * tabbed pages of controls, although you don't need to add a page if you don't want to.
 */
class ofxXmlGui: public GuiContainer, public GuiListener {

public:	
	
	/** Position the gui where you want it, and how wide the elements will be */
	void setup(float _x = 20, float _y = 20, float size = 150);
	
	/** Creates a tabbable gui page */
	GuiContainer *addPage(string name);
	
	/** show a different page */
	void setPage(string name);
	void setPage(int pageNumber);
	void nextPage();
	void prevPage();
	
	GuiTitle *addTitle(string label);
	
	/** add an ofBaseDraws to the gui */
	GuiDrawable *addDrawable(string name, ofBaseDraws &drawable);
	
	/** Add a generic slider (for floats) */
	GuiSlider	*addSlider(string name, float &ptr, float minValue, float maxValue);
	
	/** 2D slider, like a trackpad */
	GuiSlider2D *addSlider2D(string name, ofPoint &ptr, float minX, float maxX, float minY, float maxY);
	
	/** Toggle button. */
	GuiToggle *addToggle(string name, bool &ptr);	
	
	
	/** Adds a set of buttons that you can switch between */
	GuiSegmentedControl *addSegmentedControl(string name, int &value, vector<string> &options);
	
	/* alternative version: you give the options as a "|"-delimited string. */
	GuiSegmentedControl *addSegmentedControl(string name, int &value, string options);	
	
	
	/** Creates a scrollable list that users can select from - value is the index */
	GuiList *addList(string name, int &value, vector<string> &options);
	
	/** alternative version, as with addSegmentedControl */
	GuiList *addList(string name, int &value, string options);
	
	/** Adds a button that you can listen for when its pressed */
	GuiButton *addButton(string name);	
	
	/** Adds a floating point number display field (not editable) */
	GuiFloatField *addFloatField(string name, float &value);
	
	/** Adds a double precision version of addFloatField */
	GuiDoubleField *addDoubleField(string name, double &value);
	
	/** Adds a slider that centres around zero */
	GuiSlider *addPanner(string name, float &ptr, float minValue = -1, float maxValue = 1);


	/** If you set an autosave file, ofxXmlGui will save the gui state every time it changes */
	virtual void enableAutoSave(string file);	
	
	virtual void disableAutoSave();	
	
	/** Is the gui showing/active? */
	bool isEnabled();
	
	/** toggle whether gui is enabled. */
	void toggle();
	

	void enable();
	
	
	void disable();
	
	
	void addGui(GuiContainer *c);	

	///////////////////////////////////////////////////////////////////////
	// other useful inherited methods (from GuiContainer)
	
	/**
	 * If you want repoint a gui element to a new bit of data,
	 * use this function - give it the name of the control, and
	 * a pointer to the new data.
	 */
	// void pointToValue(string ctrlId, void *ptr);

	
	
	
private:
	float controlSize;
	string xmlFile;
	bool saving;
	vector<GuiContainer*> pages;
	GuiContainer *currPage;
	GuiSegmentedControl *pageChooser;
	void updatePageList();
	GuiContainer *createPage(string name);
	
	// returns -1 if there's no pages.
	int getCurrPageIndex();
public:
	/** 
	 * These are the callbacks for the ofEvents, should 
	 * be private but need visibility to be called back.
	 */
	void __draw(ofEventArgs &e);	
	void mousePressed(ofMouseEventArgs &e);
	void mouseMoved(ofMouseEventArgs &e);	
	void mouseDragged(ofMouseEventArgs &e);
	void mouseReleased(ofMouseEventArgs &e);
	void controlChanged(GuiControl *ctrl);
};
