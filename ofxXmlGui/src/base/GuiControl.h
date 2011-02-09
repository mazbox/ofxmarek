/*
 *  Control.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */
#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "GuiListener.h"
#include "GuiResources.h"

#define bval(A) (*((bool*)A))
#define ival(A) (*((int*)A))
#define fval(A) (*((float*)A))
#define dval(A) (*((double*)A))
#define faval(A, B) (*(((float*)A)+B))
#define sval(A) (*((string*)A))

#include "ColorUtils.h"
#include "StringUtils.h"

#import "ParameterInfo.h"

class GuiControl: public ofRectangle {
protected:
	// mouse is over
	bool over;
	
	// mouse is down
	bool down;
	
	// in edit mode
	bool editing;

	// resources for the gui
	GuiResources *resources;
	
	GuiControl *parent;
public:
	
	bool enabled;
	
	bool visible;
	
	// if it's allowed to scale this item in edit mode
	bool scalable;
	
	// human readable name of control
	string	name;
	
	// identifier for events
	string		controlId;
	
	
	
	// the listeners for events
	GuiListener **listeners;
	int numListeners;
	
	// raw data value
	void	*value;
	
	// the computer readable type - corresponds to its class
	string	type;
	
	// whether this element is focused
	bool focus;
	
	
	

	GuiControl();	
	
	
	// these are what you want to implement.
	
	// this is sort of your constructor - set all default values, it's overwritten by the xml
	// you must also allocate memory for the value attribute
	virtual void setup() {};
	
	// this is called after the xml is loaded so if you set anything in the xml that you 
	// need to load, now is the time to do it!
	virtual void load() {};
	
	virtual void draw();
	virtual void touchOver(int _x, int _y, int touchId) { }
	virtual bool touchMoved(int _x, int _y, int touchId){ return true; }
	virtual bool touchDown(int _x, int _y, int touchId){ return true; }
	virtual bool touchUp(int _x, int _y, int touchId){ return true; }
	virtual bool keyPressed(int key) { return false; }
	virtual vector<ParameterInfo> getParameterInfo() { return vector<ParameterInfo>(); }
	
	// this is for loading and saving values.
	virtual string valueToString();
	virtual void valueFromString(string inp);
	
	
	
	// any other loading/saving to xml that's not covered
	// by getParameterInfo() should be done in these methods
	virtual void save(ofxXmlSettings xml, int pos) {};
	virtual void load(ofxXmlSettings xml, int pos) {};

	
	
	// getter and setter for editing
	virtual void	setEditing(bool _editing) { editing = _editing; }
	bool			getEditing() { return editing; }
	
	virtual bool isContainer() { return false; }
	
	
	// Gives you a list of all the possible types an object
	// can be. Used for the gui builder.
	static vector<string> getTypes();
	
	
	// show or hides an element
	void show();
	void hide();
	
	// This is a bit different to hide()
	// it disables the control and makes
	// its width/height 0 so you can hide
	// it from the user like a hidden field
	// but it still turns up in the xml
	// like a hidden input in html forms.
	void makeHidden();
	
	// if you want to use this element without a container
	// you can use this method to provide a resources handler.
	void setResources(GuiResources *res);
	
	
	// layout stuff
	
	// resize and position, returns this
	GuiControl *size(float w, float h);
	GuiControl *position(float x, float y);
	
	// this positions this gui control underneath the 
	// one that's specified in the parameter.
	// (and returns itself)
	GuiControl *under(GuiControl *rel, float padding = 5);
	
	// this positions this control under the param
	// but aligned to the right.
	// (and returns itself)
	GuiControl *underRight(GuiControl *rel, float padding = 5);
	
	
	// position to the right
	GuiControl *right(GuiControl *rel, float padding = 5);
	
///////////////////////////////////////////////////////////////////////////////////////////////
//		MEANT TO BE PRIVATE, THESE THINGS BELOW
///////////////////////////////////////////////////////////////////////////////////////////////	
	
	bool hitTest(int _x, int _y) {
		return (_x>=x && _x<=x+width && _y>=y && _y<=y+height);
	}
	
	// to do with xml saving and loading 
	void saveBaseInfo(string tag, ofxXmlSettings xml, int pos);	
	void loadBaseInfo(string tag, ofxXmlSettings xml, int pos);	
	
	// these are secret methods for the Gui class. Don't use them
	void _draw();
	void _touchOver(int _x, int _y, int touchId);
	bool _touchMoved(int _x, int _y, int touchId);
	bool _touchDown(int _x, int _y, int touchId);
	bool _touchUp(int _x, int _y, int touchId);
	void _keyPressed(int key) {if(!enabled) return; keyPressed(key);}
	
	
	ofPoint getAbsoluteCoords();
	
	
protected:
	// creates a new guicontrol subclass based on the type string
	static GuiControl* instantiator(string _type, string _name, string _controlId, GuiResources *_resources, GuiControl *parent);
	static void addType(string _type);
	static vector<string> types;
	int editState;
	ofPoint editPoint;
};

void maskOn(int _x, int _y, int _w, int _h);
void maskOff();