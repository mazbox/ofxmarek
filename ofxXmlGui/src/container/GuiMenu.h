/*
 *  GuiMenu.h
 *  Gui
 *
 *  Created by Marek Bereza on 09/04/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "GuiContainer.h"
class GuiMenu: public GuiContainer, public GuiListener {
	
public:
	GuiMenu();
	void addMenu(string menuName);
	void addItem(string menuName, string itemName, string controlId, string itemType = "button");
	void controlChanged(GuiControl *control);
	void draw();
private: 
	int xOffset;
	vector<string> menuIds;
};