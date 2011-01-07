/*
 *  GuiListener.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *  Copyright 2010 Apple Inc. All rights reserved.
 *
 */
#pragma once
#include "GuiControl.h"
class GuiControl;
class GuiListener {
public:
	virtual void controlChanged(GuiControl *control) = 0;
	
};
