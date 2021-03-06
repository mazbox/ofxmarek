/**
 * LabeledControl.h
 * PdGui
 *
 * Created by Marek Bereza on 06/07/2011.
 *
 */
#include "Control.h"
#pragma once
class LabeledControl: public gui::Control {
public:
	LabeledControl(): Control() {
		labelColor = 0xFFFFFF;
		drawingLabel = true;

	}
	void drawLabel(float x = -1, float y = -1) {
		if(drawingLabel) {
			if(x==-1) {
				x = this->x;
				y = this->y;
			}
			setRGBA(labelColor);
			gui::Resources::drawString(name, x, y);
		}
	}
	
	void drawCustomLabel(string label, float x, float y) {
		if(drawingLabel) {
			setRGBA(labelColor);
			gui::Resources::drawString(name, x, y);
		}
	}
	
	
	void getParameterInfo(vector<ParameterInfo> &params) {
		params.push_back(ParameterInfo("Draw Label", "drawlabel", "toggle", &drawingLabel));
		params.push_back(ParameterInfo("Label Color", "labelColor", "colorpicker", &labelColor));
	}
	
	bool drawingLabel;
	int labelColor;
};