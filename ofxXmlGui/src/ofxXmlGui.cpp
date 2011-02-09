/*
 *  ofxXmlGui.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 17/01/2011.
 *
 */

#include "ofxXmlGui.h"

void ofxXmlGui::addGui(GuiContainer *c) {
	GuiListener **l = c->listeners;
	int numL = c->numListeners;
	add(c);
	c->listeners = l;
	c->numListeners = numL;
}

#define PAGE_CHOOSER_OFFSET 30



GuiContainer *ofxXmlGui::addPage(string title) {
	
	currPage->hide();
	
	if(pages.size()==0) {
		// create the page chooser
		
		// page is empty, so rename it and use it.
		if(currPage->numChildren()==0) {
			currPage->name = title;
			pages.push_back(currPage);
		} else { // default page is not empty, so call it default and create a new one
			pages.push_back(currPage);
			currPage = createPage(title);
			pages.push_back(currPage);
		}
		pageChooser = (GuiSegmentedControl*)add("segmented", "", "pagechooser");
		pageChooser->x = paddingX;
		pageChooser->y = paddingY;
		pageChooser->width = controlSize;
		
		// push all the pages down
		for(int i = 0; i < pages.size(); i++) {
			pages[i]->y = PAGE_CHOOSER_OFFSET;
		}
	} else { // already have some pages, so just create a new one
		currPage = createPage(title);
		currPage->y = PAGE_CHOOSER_OFFSET;
		pages.push_back(currPage);
		
	}
	updatePageList();
	
	currPage->show();
	
	/*
		pages[0]->name = title;
		pages[0]->controlId = title;
	} else {
		GuiContainer *page = (GuiContainer*)add("container", title, title);
		pages.push_back(page);
	}
	currPage = pages.back();
	currPage->setLayoutType(LAYOUT_SIMPLE_GUI);
	currPage->x = x;
	currPage->y = y+20;
	currPage->show();
	updatePageList();
	 */
	return currPage;
}






////////////////////////////////////////////////////////////////////////////
// ADDING GUI CONTROLS
////////////////////////////////////////////////////////////////////////////



GuiDrawable *ofxXmlGui::addDrawable(string name, ofBaseDraws &drawable) {
	GuiDrawable *el = (GuiDrawable*) currPage->add("drawable", name, name);
	el->drawable = &drawable;
	return el;
}



GuiTitle *ofxXmlGui::addTitle(string label) {
	GuiTitle *title = (GuiTitle*)currPage->add("title", label, label);
	title->width = controlSize;
	return title;
}

GuiSlider *ofxXmlGui::addSlider(string name, float &ptr, float minValue, float maxValue) {
	GuiSlider *slider = (GuiSlider*)currPage->add("slider", name, name);
	slider->min = minValue;
	slider->max = maxValue;
	slider->value = &ptr;
	slider->width = controlSize;
	slider->showValue = true;
	return slider;
}

GuiSlider2D *ofxXmlGui::addSlider2D(string name, ofPoint &ptr, float minX, float maxX, float minY, float maxY) {
	GuiSlider2D *slider2d = (GuiSlider2D*)currPage->add("slider2d", name, name);
	slider2d->minX = minX;
	slider2d->maxX = maxX;
	slider2d->minY = minY;
	slider2d->maxY = maxY;
	slider2d->width = controlSize;
	slider2d->showValue = true;
	slider2d->value = &ptr[0];
	return slider2d;
}

GuiToggle *ofxXmlGui::addToggle(string name, bool &ptr) {
	GuiToggle *slider = (GuiToggle*)currPage->add("toggle", name, name);		
	slider->value = &ptr;
	slider->width = controlSize;
	return slider;
}

string optionsToString(vector<string> &options) {
	string optionsString = "";
	for(int i = 0; i < options.size(); i++) {
		optionsString += options[i];
		if(i+1<options.size()) {
			optionsString += "|";
		}
	}
	return optionsString;
}

GuiSegmentedControl *ofxXmlGui::addSegmentedControl(string name, int &value, vector<string> &options) {
	

	return addSegmentedControl(name, value, optionsToString(options));
}

// alternative version
GuiSegmentedControl *ofxXmlGui::addSegmentedControl(string name, int &value, string options) {
	GuiSegmentedControl * sc = (GuiSegmentedControl*)currPage->add("segmented", name, name);
	sc->options = options;
	sc->value = &value;
	sc->width = controlSize;
	sc->load();
	return sc;
}


GuiList *ofxXmlGui::addList(string name, int &value, vector<string> &options) {
	return addList(name, value, optionsToString(options));
}

GuiList *ofxXmlGui::addList(string name, int &value, string options) {
	GuiList * gl = (GuiList*) currPage->add("list", name, name);
	gl->options = options;
	gl->value = &value;
	gl->width = controlSize;
	gl->load();
	return gl;
}

GuiButton *ofxXmlGui::addButton(string name) {
	GuiButton *button = (GuiButton*)currPage->add("button", name, name);
	button->width = controlSize;
	return button;
}
GuiFloatField *ofxXmlGui::addFloatField(string name, float &value) {
	GuiFloatField *ff = (GuiFloatField*)currPage->add("floatfield", name, name);
	ff->value = &value;
	ff->width = controlSize;
	return ff;
}

GuiDoubleField *ofxXmlGui::addDoubleField(string name, double &value) {
	GuiDoubleField *ff = (GuiDoubleField*)currPage->add("doublefield", name, name);
	ff->value = &value;
	ff->width = controlSize;
	return ff;
}

GuiSlider *ofxXmlGui::addPanner(string name, float &ptr, float minValue, float maxValue) {
	GuiPanner *slider = (GuiPanner*)currPage->add("panner", name, name);
	slider->min = minValue;
	slider->max = maxValue;
	slider->value = &ptr;
	slider->width = controlSize;
	slider->showValue = true;
	return slider;
}	






////////////////////////////////////////////////////////////////////////////
// EVENT CALLBACKS
////////////////////////////////////////////////////////////////////////////

void ofxXmlGui::__draw(ofEventArgs &e) {
	draw();
}

void ofxXmlGui::mousePressed(ofMouseEventArgs &e) {
	touchDown(e.x, e.y, e.button);
}

void ofxXmlGui::mouseMoved(ofMouseEventArgs &e) {
	touchOver(e.x, e.y, e.button);
}

void ofxXmlGui::mouseDragged(ofMouseEventArgs &e) {
	touchMoved(e.x, e.y, e.button);
}

void ofxXmlGui::mouseReleased(ofMouseEventArgs &e) {
	touchUp(e.x, e.y, e.button);
}


void ofxXmlGui::enableAutoSave(string file = "values.xml") {
	if(pages.size()==0) {
		currPage->enableAutoSave(file);
	} else {
		// find basename of xml
		int dotPos = file.rfind(".");
		string first;
		string second;
		if(dotPos!=-1) {
			first = file.substr(0, dotPos);
			second = file.substr(dotPos+1);
		}
		for(int i = 0; i < pages.size(); i++) {
			pages[i]->enableAutoSave(first + "."+pageChooser->opts[i]+"."+second);
		}
	}
}


void ofxXmlGui::disableAutoSave() {
	if(pages.size()==0) {
		currPage->disableAutoSave();
	} else {
		for(int i = 0; i < pages.size(); i++) {
			pages[i]->disableAutoSave();
		}
	}
	
}

////////////////////////////////////////////////////////////////////////////
// VISIBILITY
////////////////////////////////////////////////////////////////////////////

void ofxXmlGui::toggle() {
	
	if(visible) disable();
	else enable();
}

bool ofxXmlGui::isEnabled() {
	return visible;
}
void ofxXmlGui::enable() {
	ofAddListener(ofEvents.mousePressed, this, &ofxXmlGui::mousePressed);
	ofAddListener(ofEvents.mouseMoved, this, &ofxXmlGui::mouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &ofxXmlGui::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &ofxXmlGui::mouseReleased);
	ofAddListener(ofEvents.draw, this, &ofxXmlGui::__draw);
	show();
}


void ofxXmlGui::disable() {
	ofRemoveListener(ofEvents.mousePressed, this, &ofxXmlGui::mousePressed);
	ofRemoveListener(ofEvents.mouseMoved, this, &ofxXmlGui::mouseMoved);
	ofRemoveListener(ofEvents.mouseDragged, this, &ofxXmlGui::mouseDragged);
	ofRemoveListener(ofEvents.mouseReleased, this, &ofxXmlGui::mouseReleased);
	ofRemoveListener(ofEvents.draw, this, &ofxXmlGui::__draw);
	hide();
}

void ofxXmlGui::setup(float _x, float _y, float size) {
	
	name = "ofxXmlGui";
	pageChooser = NULL;
	
	setLayoutType(LAYOUT_ABSOLUTE);
	x = _x;
	y = _y;
	
	width = size;
	this->controlSize = size - paddingX*2;
	
	currPage = createPage("default");
	addListener((GuiListener*)this);
	
	xmlFile = "";
	saving = false;
	disable();
}

void ofxXmlGui::updatePageList() {
	vector<string> pageNames;
	for(int i = 0; i < pages.size(); i++) {
		pageNames.push_back(pages[i]->name);
	}
	
	pageChooser->options = optionsToString(pageNames);
	pageChooser->load();
	int currPageIndex = getCurrPageIndex();
	if(currPageIndex!=-1) {
		ival(pageChooser->value) = currPageIndex;
	}
	
}
void ofxXmlGui::controlChanged(GuiControl *ctrl) {
	if(ctrl==pageChooser) {
		currPage->hide();
		currPage = pages[ival(ctrl->value)];
		currPage->show();
	} else {
		printf("%s\n", ctrl->controlId.c_str());
	}
}

GuiContainer *ofxXmlGui::createPage(string name) {
	GuiContainer *p = (GuiContainer*)add("container", name, name);
	p->setLayoutType(LAYOUT_SIMPLE_GUI);
	p->x = 0;
	p->y = 0;
	p->width = controlSize+paddingX*2;
	return p;
}


////////////////////////////////////////////////////////////////////////////
// PAGE STUFF
////////////////////////////////////////////////////////////////////////////
/** show a different page */
void ofxXmlGui::setPage(string name) {
	for(int i = 0; i < pages.size(); i++) {
		if(pages[i]->name==name) {
			currPage->hide();
			currPage = pages[i];
			ival(pageChooser->value) = i;
			currPage->show();
			return;
		}
	}
}
void ofxXmlGui::setPage(int pageNumber) {
	
	// check bounds - ignore if out of bounds.
	if(pageNumber>=0 && pageNumber<pages.size()) {
		currPage->hide();
		currPage = pages[pageNumber];
		ival(pageChooser->value) = pageNumber;
		currPage->show();
	} else {
		printf("ofxXmlGui: Warning setPage(int) parameter out of bounds\n");
	}
}

int ofxXmlGui::getCurrPageIndex() {
	// find what page we're on.
	for(int i = 0; i < pages.size(); i++) {
		if(pages[i]==currPage) {
			return i;
		}
	}
	return -1;
}

void ofxXmlGui::nextPage() {
	int currentIndex = getCurrPageIndex();
	// increment and wrap
	if(currentIndex!=-1) {
		currentIndex++;
		currentIndex %= pages.size();
		currPage->hide();
		currPage = pages[currentIndex];
		currPage->show();
		ival(pageChooser->value) = currentIndex;
	}
}
void ofxXmlGui::prevPage() {
	int currentIndex = getCurrPageIndex();
	// decrement and wrap
	if(currentIndex!=-1) {
		currentIndex--;
		if(currentIndex<0) currentIndex += pages.size();
		currPage->hide();
		currPage = pages[currentIndex];
		currPage->show();
		ival(pageChooser->value) = currentIndex;
	}
}