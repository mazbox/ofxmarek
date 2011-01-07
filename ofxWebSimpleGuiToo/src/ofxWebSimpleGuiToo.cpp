/*
 *  ofxWebSimpleGuiToo.cpp
 *  ofxWebSimpleGuiTooExample
 *
 *  Created by Marek Bereza on 27/03/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "ofxWebSimpleGuiToo.h"


ofxWebSimpleGuiToo webGui;

ofxWebSimpleGuiToo::ofxWebSimpleGuiToo() {
}
void ofxWebSimpleGuiToo::startServer(int port) {
	server.start("ofxWebSimpleGuiToo", port);
	server.addHandler(this, "/control");
}

void ofxWebSimpleGuiToo::stopServer(){
	server.stop();
}

void ofxWebSimpleGuiToo::replaceAll(string &str, const string &find_what, const string &replace_with) {
	string::size_type pos=0;
	while((pos=str.find(find_what, pos))!=string::npos)
	{
		str.erase(pos, find_what.length());
		str.insert(pos, replace_with);
		pos+=replace_with.length();
	}
}


void ofxWebSimpleGuiToo::httpGet(string url) {
	
	string key = getRequestParameter("key");
	replaceAll(key, "%20", " ");
	if(key=="") {
		string guiDesc = "[";
		for(int i = 0; i < pages.size(); i++) {
			guiDesc += describePage(pages[i]);
			if(i+1<pages.size()) {
				guiDesc += ",";
			}
		}
		guiDesc += "]";
		httpResponse(guiDesc);
	} else {

		string value = getRequestParameter("value");
		printf("value received: %s = %s\n", key.c_str(), value.c_str());
		ofxSimpleGuiControl *ctrl = &control(key);
		if(ctrl!=NULL) {
			if(ctrl->controlType=="Toggle") {
				ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle*)ctrl;
				*(t->value) = (value=="true");
			} else if(ctrl->controlType=="SliderInt") {
				ofxSimpleGuiSliderInt *t = (ofxSimpleGuiSliderInt*)ctrl;
				*(t->value) = atoi(value.c_str());
			} else if(ctrl->controlType=="SliderFloat") {
				ofxSimpleGuiSliderFloat *t = (ofxSimpleGuiSliderFloat*)ctrl;
				*(t->value) = atof(value.c_str());
			} else if(ctrl->controlType=="Button") {
				ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle*)ctrl;
//				t->onPress(0, 0, 0);
//				t->onRelease(0, 0, 0);
				*(t->value) = true;
// 				printf("Hello here!!\n");
			//} else if(ctrl->controlType=="ColorPicker") {
			//	ofxSimpleGuiColorPicker *t = (ofxSimpleGuiColorPicker*) ctrl;
			//	vector<string> ss = ofSplitString(value, ",");
			//	for(int i = 0; i < 4; i++) 
			//		t->values[i] = atof(ss[i].c_str());

			} else if(ctrl->controlType=="Slider2D") {
				ofxSimpleGuiSlider2d *t = (ofxSimpleGuiSlider2d*) ctrl;
				vector<string> ss = ofSplitString(value, ",");
				
				t->value->x = atof(ss[0].c_str());
				t->value->y = atof(ss[1].c_str());
				
			/*} else if(ctrl->controlType=="Slider3D") {
				ofxSimpleGuiSlider3d *t = (ofxSimpleGuiSlider3d*) ctrl;
				vector<string> ss = ofSplitString(value, ",");
				
				t->value->x = atof(ss[0].c_str());
				t->value->y = atof(ss[1].c_str());
				t->value->z = atof(ss[2].c_str());
			*/
			}
			
		}
		httpResponse("thanks");
	}
}

string ofxWebSimpleGuiToo::describePage(ofxSimpleGuiPage *page) {
	string desc = "{\"name\": \""+page->name+"\", \"controls\":[";
	for(int i = 0; i < page->getControls().size(); i++) {
		desc += describeControl(page->getControls()[i]);
		if(i+1<page->getControls().size()) {
			desc += ",";
		}
	}
	desc += "]}";
	return desc;
}
string ofxWebSimpleGuiToo::describeControl(ofxSimpleGuiControl *ctrl) {
	string desc = "{\"name\":\""+ctrl->name + "\",";
	desc += "\"type\": \""+ctrl->controlType+"\"";
	
	if(ctrl->controlType=="Toggle") {
		ofxSimpleGuiToggle *t = (ofxSimpleGuiToggle*)ctrl;
		if(*t->value) desc += ", \"value\":\"true\"";
		else desc += ", \"value\":\"false\"";
	} else if(ctrl->controlType=="SliderInt") {
		ofxSimpleGuiSliderInt *t = (ofxSimpleGuiSliderInt*)ctrl;
		desc += ", \"value\":\""+ofToString(*(t->value))+"\"";
		desc += ", \"min\":\""+ofToString(t->min)+"\"";
		desc += ", \"max\":\""+ofToString(t->max)+"\"";
		
	} else if(ctrl->controlType=="SliderFloat") {
		ofxSimpleGuiSliderFloat *t = (ofxSimpleGuiSliderFloat*)ctrl;
		desc += ", \"value\":\""+ofToString(*(t->value))+"\"";
		desc += ", \"min\":\""+ofToString(t->min)+"\"";
		desc += ", \"max\":\""+ofToString(t->max)+"\"";
		
	} else if(ctrl->controlType=="Slider2D") {
		ofxSimpleGuiSlider2d *t = (ofxSimpleGuiSlider2d*)ctrl;
		
		
		desc += ", \"value\":\""+ofToString(t->value->x)+","+ofToString(t->value->y)+"\"";
		desc += ", \"minX\":\""+ofToString(t->min.x)+"\"";
		desc += ", \"maxX\":\""+ofToString(t->max.x)+"\"";
		desc += ", \"minY\":\""+ofToString(t->min.y)+"\"";
		desc += ", \"maxY\":\""+ofToString(t->max.y)+"\"";
	/*} else if(ctrl->controlType=="Slider3D") {
		ofxSimpleGuiSlider3d *t = (ofxSimpleGuiSlider3d*)ctrl;
		
		
		desc += ", \"value\":\""+ofToString(t->value->x)+","+ofToString(t->value->y)+","+ofToString(t->value->z)+"\"";
		desc += ", \"minX\":\""+ofToString(t->min.x)+"\"";
		desc += ", \"maxX\":\""+ofToString(t->max.x)+"\"";
		desc += ", \"minY\":\""+ofToString(t->min.y)+"\"";
		desc += ", \"maxY\":\""+ofToString(t->max.y)+"\"";
		desc += ", \"minZ\":\""+ofToString(t->min.z)+"\"";
		desc += ", \"maxZ\":\""+ofToString(t->max.z)+"\"";*/
	//} else if(ctrl->controlType=="ColorPicker") {
	//	ofxSimpleGuiColorPicker *t = (ofxSimpleGuiColorPicker*)ctrl;
		
		
	//	desc += ", \"value\":\""
	//	+ofToString(t->values[0])+","+ofToString(t->values[1])+","+ofToString(t->values[2])+","+ofToString(t->values[3])+"\"";
		
	}
	desc += "}";
	return desc;
}

