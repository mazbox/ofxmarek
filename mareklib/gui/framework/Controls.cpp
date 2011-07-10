/**
 * Controls.cpp
 * emptyExample
 *
 * Created by Marek Bereza on 20/06/2011.
 *
 */

#include "Instantiator.h"
#include "Gui.h"
#include "Slider.h"
#include "PushButton.h"
#include "Menu.h"
#include "MenuItem.h"
#include "MenuBar.h"
#include "ColorPicker.h"
#include "Knob.h"
#include "Toggle.h"
#include "Contour.h"
#include "VU.h"
#include "Meter.h"
#include "Keyboard.h"

#include "Panner.h"
#include "SegmentedControl.h"
#include "Title.h"
#include "Slider2D.h"
#include "Drawable.h"
#include "List.h"
#include "DoubleField.h"
#include "TextField.h"
#include "Text.h"
#include "FloatField.h"
#include "FPSCounter.h"
#include "IntField.h"
#include "Grid.h"
#include "FileField.h"
#include "Pane.h"

#define REGISTER_CONTROL(A, B) addControlConstructor(ControlInstantiator<A>, B);




void gui::Instantiator::registerControls() {
	REGISTER_CONTROL(Control,		"control");

	REGISTER_CONTROL(Container,		"container");
	REGISTER_CONTROL(Pane,			"pane");
	
	REGISTER_CONTROL(PushButton,	"pushbutton");
	REGISTER_CONTROL(Slider,		"slider");
	REGISTER_CONTROL(ColorPicker,	"colorpicker");
	REGISTER_CONTROL(Knob,			"knob");
	REGISTER_CONTROL(Toggle,		"toggle");
	REGISTER_CONTROL(Contour,		"contour");
	REGISTER_CONTROL(Keyboard,		"keyboard");
	REGISTER_CONTROL(VU,			"vu");
	REGISTER_CONTROL(Meter,			"meter");
	REGISTER_CONTROL(Panner,		"panner");
	REGISTER_CONTROL(SegmentedControl, "segmented");
	REGISTER_CONTROL(Title,				"title");
	REGISTER_CONTROL(Slider2D,		"slider2d");
	REGISTER_CONTROL(Drawable,		"drawable");
	REGISTER_CONTROL(List,			"list");
	
	REGISTER_CONTROL(DoubleField,	"doublefield");
	REGISTER_CONTROL(TextField,		"textfield");
	REGISTER_CONTROL(Text,			"text");
	REGISTER_CONTROL(FloatField,	"floatfield");
	REGISTER_CONTROL(FPSCounter,	"fps");
	REGISTER_CONTROL(IntField,		"intfield");

	REGISTER_CONTROL(FileField,		"file");
	
	REGISTER_CONTROL(Grid,			"grid");
	
	// menu stuff for gui editor
	REGISTER_CONTROL(Menu,			"menu");
	REGISTER_CONTROL(MenuBar,		"menubar");
	REGISTER_CONTROL(MenuItem,		"menuitem");
}

