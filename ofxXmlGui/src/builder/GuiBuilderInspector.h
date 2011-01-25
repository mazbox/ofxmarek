/*
 *  GuiInspector.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *  Copyright 2010 Apple Inc. All rights reserved.
 *
 */
#pragma once

#include "GuiContainer.h"
#include "GuiListener.h"


class GuiBuilderInspector: public GuiContainer, public GuiListener {

public:
	GuiBuilderInspector() {
		setLayoutType(LAYOUT_VERTICAL);
		
		addListener(this);
		paddingY = 14;
		gui = NULL;
		setMovable(true);
	}
	
	void setGui(GuiContainer *_gui) {
		gui = _gui;
	}
	void controlChanged(GuiControl *control) {
		if(control->controlId=="1234reload") {
			if(gui!=NULL) {
				gui->reload();
			}
		} else if(control->getEditing()){
			
			clear();
			add("textfield", "Name", "Name");
			add("textfield", "Control ID", "Control ID");
			add("floatfield", "Width", "Width");
			add("floatfield", "Height", "Height");
			
			pointToValue("Name", &control->name);
			pointToValue("Control ID", &control->controlId);
			pointToValue("Width", &control->width);
			pointToValue("Height", &control->height);
			
			
			vector<ParameterInfo> params = control->getParameterInfo();
			bool needsReloadButton = false;
			for(int i = 0; i < params.size(); i++) {
				string ID = ""+ofToString(i+8345);
				add(params[i].type, params[i].name, ID);
				if(params[i].type=="file") needsReloadButton = true;
				pointToValue(ID, params[i].value);
			}
			if(control->type=="segmented") needsReloadButton = true;
			if(control->type=="list") needsReloadButton = true;
			if(needsReloadButton) add("button", "reload", "1234reload");
			
			height += 20;
			
			
		} 
	}
private:
	GuiContainer *gui;
};