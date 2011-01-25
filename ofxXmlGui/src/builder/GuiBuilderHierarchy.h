/*
 *  GuiBuilderHierarchy.h
 *  Gui
 *
 *  Created by Marek Bereza on 05/05/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "GuiList.h"
#include "GuiBuilderMenu.h"

class GuiBuilderHierarchy: public GuiContainer, public GuiListener {
public:
	
	GuiList *list;
	GuiContainer *gui;
	GuiBuilderMenu *menu;
	map<string,GuiContainer*> pointers;
	GuiBuilderHierarchy() {
		height = 200;
		width = 200;
		x = ofGetWidth()-width;
		y = 30;
		
		setLayoutType(LAYOUT_VERTICAL);
		
//		setListener(this);
		paddingY = 14;
		addListener(this);
		setMovable(true);
		
		list = (GuiList*)GuiControl::instantiator("list", "list", "list", resources, this);
		add(list);
	
		add("button", "refresh", "refresh");
		
		gui = NULL;
	}
	
	void controlChanged(GuiControl *control) {
		if(control->controlId=="refresh") {
			refresh();
		} else if(control->controlId=="list") {
			string ctrlId = list->getSelectedString();
			//if(ctrlId.find("_ ")!=-1) {
			//	ctrlId = ctrlId.substr(ctrlId.find("_ ")+2);
			//}
			menu->setGui(pointers[ctrlId]);
			printf("Selected %s\n", ctrlId.c_str());
		}
	}
	
	void refresh() {
		if(gui!=NULL) {
			list->clearItems();
			pointers.clear();
			buildHierarchy(gui, 0);
		}
	}
	
	void buildHierarchy(GuiContainer *cont,int level) {
		string ding = "";
		for(int i = 0; i < level; i++) {
			ding += "_";
		}
		ding += " ";
		
		list->addItem(ding+cont->controlId);
		pointers[ding+cont->controlId] = cont;
		vector<GuiControl*> c = cont->getControls();
		for(int i = 0; i < c.size(); i++) {
			if(c[i]->isContainer()) {
				buildHierarchy((GuiContainer*)c[i], level+1);
			}
		}
	}
	
	void setGui(GuiContainer *_gui, GuiBuilderMenu *_menu) {
		gui = _gui;
		menu = _menu;
		refresh();
	}
	
};
