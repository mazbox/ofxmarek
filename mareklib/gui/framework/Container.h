/**
 * Container.h
 * emptyExample
 *
 * Created by Marek Bereza on 16/06/2011.
 *
 */
#pragma once
#include "Control.h"
#include "Listener.h"

namespace gui {
	
	enum LayoutType {
		
		// do nothing
		LayoutType_absolute,
		// stack the elements on top of eachother
		LayoutType_vertical
	};
	
	class Container: public Control {
	public:
		Container();
		~Container();
		virtual void draw() ;
		void clear();
		void addChild(Control *child);
		bool removeChild(Control *child);
		virtual void touchOver(int x, int y, int id);
		virtual bool touchDown(int x, int y, int id);
		virtual bool touchMoved(int x, int y, int id);
		virtual bool touchUp(int x, int y, int id);
		
		virtual bool keyPressed(int key);
		virtual Control *hitTest(float x, float y);
		void saveToXmlObject(ofxXmlSettings &xml);
		virtual void saveToXml(string file = "");
		
		void loadFromXmlObject(TiXmlElement *xml);
		virtual bool isContainer() { return true; };
		
		void addListener(Listener *listener);
		Control *getControlById(string id);
		void setLayoutType(LayoutType layoutType);
		void pointToValue(string controlName, void *pointer);
		
		float getFloat(string controlName);
		int getInt(string controlName);
		bool getBool(string controlName);
		string getString(string controlName);
		
		
		// if you want a control to have keyboard focus, you call
		// this and it receives keyboard events directly from the 
		// root container
		void setKeyboardFocus(Control *keyboardFocusedControl);
			
	protected:
		
		// the control currently receiving keyboard events
		Control *keyboardFocusedControl;
		LayoutType layoutType;
		void drawChildren() ;
		vector<Listener*> listeners;
		deque<Control*> children;
	};

};
