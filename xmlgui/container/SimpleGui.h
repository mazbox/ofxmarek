#pragma once
#include "Container.h"
#include "Instantiator.h"
#include "Slider.h"
#include "Toggle.h"
#include "SegmentedControl.h"
#include "List.h"
#include "PushButton.h"
#include "ofGuiEventDispatcher.h"
#include "Drawable.h"
#include "Panner.h"
#include "IntSlider.h"

//#define SIMPLE_GUI_WIDTH 150

namespace xmlgui {
	
	class SimpleGui: public xmlgui::Container, public xmlgui::Listener {
	public:
		
		SimpleGui(): xmlgui::Container() {
			addListener(this);
			setLayoutType(xmlgui::LayoutType_vertical);
			x = 10;
			y = 20;
			mustAddNewColumn = false;
			isSetup = false;
			SIMPLE_GUI_WIDTH = 150;
		}
		virtual void setup() {
			events.setup(this);
			isSetup = true;
			enabled = false;
			setEnabled(true);
			setEnabled(false);
					}
		void setEnabled(bool enabled) {
			if(!isSetup) this->setup();
			if(this->enabled!=enabled) {
				events.setEnabled(enabled);
				this->enabled = enabled;
			}
		}
		
		// implement this method if you want to receive
		// control changed stuff in your superclass as
		// you can't implement an xmlgui::Listener
		virtual void ctrlChanged(xmlgui::Control *ctrl) {
			
		}
		void controlChanged(xmlgui::Control *ctrl) {
			this->ctrlChanged(ctrl);
			if(settingsFile!="") {	
				saveSettings();
			}
		}
		
		Drawable *addDrawable(string name, ofBaseDraws &baseDraws) {
			Drawable *drawable = (Drawable*)INSTANTIATE_WITH_ID("drawable", name);
			drawable->drawable = &baseDraws;
			drawable->width = SIMPLE_GUI_WIDTH;
			drawable->height = baseDraws.getHeight()*SIMPLE_GUI_WIDTH/baseDraws.getWidth();
			addChild(drawable);
			columnCheck();
			return drawable;
			
		}
		IntSlider *addSlider(string name, int &value, int min, int max) {
			IntSlider *slider = (IntSlider*)INSTANTIATE_WITH_ID("intslider", name);
			slider->pointToValue(&value);
			slider->min = min;
			slider->max = max;
			slider->width = SIMPLE_GUI_WIDTH;
			slider->showValue = true;
			addChild(slider);
			columnCheck();
			return slider;
			
		}
		Slider *addSlider(string name, float &value, float min, float max) {
			Slider *slider = (Slider*)INSTANTIATE_WITH_ID("slider", name);
			slider->pointToValue(&value);
			slider->min = min;
			slider->max = max;
			slider->width = SIMPLE_GUI_WIDTH;
			slider->showValue = true;
			addChild(slider);
			columnCheck();
			return slider;

		}
		
		Panner *addPanner(string name, float &value, float min, float max) {
			Panner *slider = (Panner*)INSTANTIATE_WITH_ID("panner", name);
			slider->min = min;
			slider->max = max;
			slider->width = SIMPLE_GUI_WIDTH;
			slider->pointToValue(&value);
			slider->showValue = true;
			addChild(slider);
			columnCheck();

			return slider;
		}
		
		Toggle *addToggle(string name, bool &value) {
			Toggle *tog = (Toggle*)INSTANTIATE_WITH_ID("toggle", name);
			tog->pointToValue(&value);
			tog->width = tog->height; // make it square
			addChild(tog);
			columnCheck();
			return tog;
		}
		
		PushButton *addPushButton(string name) {
			PushButton *tog = (PushButton*)INSTANTIATE_WITH_ID("pushbutton", name);
			tog->width = 80;
			tog->height = 20;
			addChild(tog);
			columnCheck();
			return tog;
			
		}
		SegmentedControl *addSegmented(string name, int &value, string options) {
			SegmentedControl *seg = (SegmentedControl*)INSTANTIATE_WITH_ID("segmented", name);
			seg->pointToValue(&value);
			seg->width = SIMPLE_GUI_WIDTH;
			seg->options = options;
			seg->load();
			addChild(seg);
			columnCheck();
			return seg;
		}
		
		SegmentedControl *addSegmented(string name, int &value, vector<string> options) {
			
			SegmentedControl *seg = (SegmentedControl*)INSTANTIATE_WITH_ID("segmented", name);
			seg->pointToValue(&value);
			seg->width = SIMPLE_GUI_WIDTH;
			seg->opts = options;
			addChild(seg);
			columnCheck();
			return seg;
		}
		
		List *addList(string name, int &value, vector<string> options) {
			List *list = (List*)INSTANTIATE_WITH_ID("list", name);
			list->pointToValue(&value);
			list->items = options;
			list->width = SIMPLE_GUI_WIDTH;
			addChild(list);
			columnCheck();
			return list;
		}

		
		void saveSettings(string file = "") {
			if(file=="") {
				if(this->settingsFile=="") {
					ofLogError() << "No settings file specified, will not save gui settings";
					return;
				}
			} else {
				this->settingsFile = file;
			}
			ofxXmlSettings xml;
			xml.addTag("settings");
			xml.pushTag("settings");
			for(int i = 0; i < children.size(); i++) {
				xml.addTag("setting");
				xml.addAttribute("setting", "id", children[i]->id, i);
				xml.addAttribute("setting", "value", children[i]->valueToString(), i);
			}
			xml.saveFile(settingsFile);
		}
		
		void loadSettings(string file) {
			this->settingsFile = file;
			ofxXmlSettings xml;
			xml.loadFile(file);
			
			xml.pushTag("settings");
			int numTags = xml.getNumTags("setting");
			for(int i = 0; i < numTags; i++) {
				string id = xml.getAttribute("setting", "id", "", i);
				string value = xml.getAttribute("setting", "value", "", i);
				xmlgui::Control *c = getControlById(id);
				if(c!=NULL) {
					c->valueFromString(value);
				} else {
				
					ofLogError() << "Could not find control named '" << id << "'";
				}
				
			}
		}
		
		void columnCheck() {
			if(mustAddNewColumn) {
				// move the last added item onto a new column
				if(children.size()>0) {
					children.back()->y = 0;
					children.back()->x = children[children.size()-2]->x + SIMPLE_GUI_WIDTH + AUTO_LAYOUT_PADDING;
				}
				mustAddNewColumn = false;
			}
		}
		void addColumn() {
			mustAddNewColumn = true;
		}
		xmlgui::ofGuiEventDispatcher events;
		
		
			
		void toggle() {
			setEnabled(!enabled);
		}
		
		// this must be called before adding controls!
		void setWidth(float width) {
			SIMPLE_GUI_WIDTH = width;
		}
		bool isEnabled() { return enabled; }
		
	private:
		bool enabled;
		bool mustAddNewColumn;
		bool isSetup;
		string settingsFile;
		float SIMPLE_GUI_WIDTH;
	};
};