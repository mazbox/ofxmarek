/*
 *  GuiTypes.cpp
 *  Gui
 *
 *  Created by Marek Bereza on 12/02/2010.
 *
 */


#include "GuiControl.h"
#include "GuiTitle.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiToggle.h"
#include "GuiTextField.h"
#include "GuiFPSCounter.h"
#include "GuiIntField.h"
#include "GuiFloatField.h"
#include "GuiDoubleField.h"
#include "GuiSlider2D.h"
#include "GuiImage.h"
#include "GuiImageButton.h"
#include "GuiImageToggle.h"
#include "GuiNiceButton.h"
#include "GuiSegmentedControl.h"
#include "GuiText.h"
#include "GuiColorPicker.h"
#include "GuiKeyboard.h"
#include "GuiKnob.h"
#include "GuiContour.h"
#include "GuiMeter.h"
#include "GuiVU.h"
#include "GuiList.h"
#include "GuiGrid.h"
#include "GuiPanner.h"
#include "GuiContainer.h"
#include "GuiDrawable.h"


#define BEGIN_REGISTRY() static bool firstCall = true; if(0) {}
#define REGISTER(TYPE,CLASS) if(_type==TYPE) { \
instance = new CLASS(); \
} \
if(firstCall) addType(TYPE); 
#define END_REGISTRY() if(instance==NULL) instance = new GuiControl(); \
firstCall = false; 

GuiControl* GuiControl::instantiator(string _type, string _name, string _controlId, GuiResources *_resources, GuiControl *_parent) {
	GuiControl *instance = NULL;
	
BEGIN_REGISTRY()
	
	REGISTER("container", GuiContainer)
	REGISTER("button", GuiButton)
	REGISTER("drawable", GuiDrawable)
	REGISTER("title", GuiTitle)
	REGISTER("slider", GuiSlider)
	REGISTER("toggle", GuiToggle)
	REGISTER("knob", GuiKnob)
	REGISTER("text", GuiText)
	REGISTER("textfield", GuiTextField)
	REGISTER("intfield", GuiIntField)
	REGISTER("floatfield", GuiFloatField)
	REGISTER("doublefield", GuiDoubleField)
	REGISTER("fps", GuiFPSCounter)
	REGISTER("slider2d", GuiSlider2D)
	REGISTER("segmented", GuiSegmentedControl)
	REGISTER("colorpicker", GuiColorPicker)
	REGISTER("keyboard", GuiKeyboard)
	REGISTER("file", GuiTextField)
	REGISTER("contour", GuiContour)
	REGISTER("meter", GuiMeter)
	REGISTER("vu", GuiVU)
	REGISTER("list", GuiList)
	REGISTER("grid", GuiGrid)
	REGISTER("panner", GuiPanner)

END_REGISTRY()
	
	
		
	instance->resources = _resources;
	instance->type = _type;
	instance->controlId = _controlId;
	instance->name = _name;
	instance->parent = _parent;
	instance->setup();
	return instance;
}


