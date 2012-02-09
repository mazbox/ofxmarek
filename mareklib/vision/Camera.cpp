/**
 *  Camera.cpp
 *
 *  Created by Marek Bereza on 15/07/2011.
 */

#include "Camera.h"
//#include "constants.h"
// doesn\t work0
//#define CAM_GREYSCALE

#ifdef CAM_GREYSCALE
#endif
#ifdef _WIN32
#include "FlyCaptureGrabber/FlyCaptureGrabber.h"
FlyCaptureGrabber *sdk;

#include "SimpleGui.h"
xmlgui::SimpleGui myGui;
map<string,int> cameraPropsIds;
#endif


void tricks::vision::Camera::setup(int width, int height, string testVideoFile, string imagesFolder) {
	
	saveNextFrame = false;
	
	
#ifndef TEST_WITH_VIDEO
#ifdef _WIN32
	--
	firewire.setDeviceID(0);
	sdk = new FlyCaptureGrabber;
	sdk->listDevices();

	

#else
    Libdc1394Grabber *sdk = new Libdc1394Grabber;
	
	sdk->listDevices();
	sdk->setDiscardFrames(true);
	sdk->set1394bMode(false);
	char path[512];
	getcwd(path, 512);
	printf("Working directory: %s\n", ofToDataPath("", true).c_str());
	
#endif

	
	ofxIIDCSettings *settings = new ofxIIDCSettings;
	settings->setXMLFilename("mySettingsFile.xml");

	firewire.setVerbose(true);
	
#ifdef CAM_GREYSCALE
	bool result = firewire.initGrabber( width, height, VID_FORMAT_GREYSCALE, VID_FORMAT_GREYSCALE, 30, true, sdk, settings );
#else
	bool result = firewire.initGrabber( width, height, VID_FORMAT_YUV422, VID_FORMAT_RGB, 30, true, sdk, settings );
#endif
    canDoFirewire = result;

#ifdef _WIN32
	// setup the gui
	printf("Setted up gui\n");
	myGui.setup();
	myGui.addListener(this);
	
	int numProps = sdk->getNumProperties();
	for(int i = 0; i < numProps; i++) {
		string name;
		float value = 0;
		float min = 0;
		float max = 0;
		sdk->getPropertyInfo(i, name, value, min, max);
		float *f = new float;
		*f = value;
		printf("Value %f\n", value);
		//if(name=="BRIGHTNESS") max = 1024;
		myGui.addSlider(name, *f, min, max);
		cameraPropsIds[name] = i;
	}
#endif

	
#endif
	if(imagesFolder!="no-images") {
		this->imagesFolder = imagesFolder;
		imagePos = -1;
		nextImage();
	}
	canDoCamera = false;
#ifndef TEST_WITH_VIDEO
	if(result) {
		mode = CAMERA_MODE_FIREWIRE;
	} else if(videoGrabber.initGrabber(width, height)) {
		mode = CAMERA_MODE_QUICKTIME;
        canDoCamera = true;
	} else 
#endif
	{
		if(testVideoFile=="") {
			ofLogError("Video file path was empty, not gonna do a whole lot now");
		} else {
			printf("Trying to load %s\n", ofToDataPath(testVideoFile).c_str());
		}
		if(!testDataPlayer.loadMovie(testVideoFile)) {
			ofLogError("Couldn't load the movie, not gonna do shit now! %s", testVideoFile.c_str());
		}
		else {
			testDataPlayer.play();
			mode = CAMERA_MODE_RECORDED;
		}
	}
	
}


#ifdef _WIN32
void tricks::vision::Camera::controlChanged(xmlgui::Control *control) {
	printf("%s\n", control->name.c_str());
	sdk->setProperty(cameraPropsIds[control->name], control->getFloat());
}
#endif


ofTexture &tricks::vision::Camera::getTextureReference() {
    if(mode==CAMERA_MODE_FIREWIRE) return firewire.getTextureReference();
	else if(mode==CAMERA_MODE_QUICKTIME) return videoGrabber.getTextureReference();
	else if(mode==CAMERA_MODE_IMAGES) return image.getTextureReference();
	else return testDataPlayer.getTextureReference();
}
void tricks::vision::Camera::update() {
	if(mode==CAMERA_MODE_FIREWIRE) firewire.update();
	else if(mode==CAMERA_MODE_QUICKTIME) videoGrabber.update();
	else if(mode==CAMERA_MODE_RECORDED) testDataPlayer.update();
}

void tricks::vision::Camera::draw(float x, float y) {
	if(mode==CAMERA_MODE_FIREWIRE) return firewire.draw(x,y);
	else if(mode==CAMERA_MODE_QUICKTIME) return videoGrabber.draw(x,y);
	else if(mode==CAMERA_MODE_IMAGES) return image.draw(x,y);
	else return testDataPlayer.draw(x,y);
}

void tricks::vision::Camera::draw(float x, float y, float width, float height) {
	if(mode==CAMERA_MODE_FIREWIRE) firewire.draw(x,y,width,height);
	else if(mode==CAMERA_MODE_QUICKTIME) videoGrabber.draw(x,y,width,height);
	else if(mode==CAMERA_MODE_IMAGES) image.draw(x,y,width,height);
	else testDataPlayer.draw(x,y,width,height);

}

unsigned char *tricks::vision::Camera::getPixels() {
	unsigned char *pixels = NULL;

	if(mode==CAMERA_MODE_FIREWIRE) pixels = firewire.getPixels();
	else if(mode==CAMERA_MODE_QUICKTIME) pixels = videoGrabber.getPixels();
	else if(mode==CAMERA_MODE_IMAGES) pixels = image.getPixels();
	else pixels = testDataPlayer.getPixels();

	if(saveNextFrame && pixels!=NULL) {
		printf("Saved a frame\n");
		ofImage img;
		img.allocate(getWidth(), getHeight(), OF_IMAGE_COLOR);
		img.setFromPixels(pixels, getWidth(), getHeight(), OF_IMAGE_COLOR);
		string root = imagesFolder;
		if(root!="") root += "/";
		img.saveImage(root + string("frame-")+ofToString(ofGetFrameNum())+".png");
		saveNextFrame = false;
	}
	
	return pixels;
}

int tricks::vision::Camera::getWidth() {
	if(mode==CAMERA_MODE_FIREWIRE) return firewire.getWidth();
	else if(mode==CAMERA_MODE_QUICKTIME) return videoGrabber.getWidth();
	else if(mode==CAMERA_MODE_IMAGES) return image.getWidth();
	else return testDataPlayer.getWidth();
}

int tricks::vision::Camera::getHeight() {
	if(mode==CAMERA_MODE_FIREWIRE) return firewire.getHeight();
	else if(mode==CAMERA_MODE_QUICKTIME) return videoGrabber.getHeight();
	else if(mode==CAMERA_MODE_IMAGES) return image.getHeight();
	else return testDataPlayer.getHeight();
}

void tricks::vision::Camera::setPaused(bool paused) {
	if(mode==CAMERA_MODE_RECORDED) {
		testDataPlayer.setPaused(paused);
	}
}

bool tricks::vision::Camera::isPaused() {
	if(mode==CAMERA_MODE_RECORDED) {
		return testDataPlayer.isPaused();
	}
	
	return false;
}

void tricks::vision::Camera::videoSettings() {
	if(mode==CAMERA_MODE_FIREWIRE) {
		printf("trying to show settings\n");
#ifdef _WIN32
		myGui.toggle();
#else
		firewire.videoSettings();
#endif
	}
}

void tricks::vision::Camera::saveFrame() {
	saveNextFrame = true;
}
void tricks::vision::Camera::drawSettings() {
	if(mode==CAMERA_MODE_FIREWIRE) firewire.drawSettings();
}


void tricks::vision::Camera::nextImage() {
	ofDirectory imgFolder(imagesFolder);
	imgFolder.allowExt("png");
	imgFolder.allowExt("jpg");
	
	int numEntries = imgFolder.listDir();
	
	if(imagePos==-1) { // initial state
		imagePos = 0;
	} else {
		imagePos++;
		if(imagePos>=numEntries) {
			imagePos = 0;
		}
	}
	
	if(imagePos<numEntries && imagePos>=0) {
		image.loadImage(imgFolder.getPath(imagePos));
	} else {
		printf("Couldn't load pic\n");
	}

}
void tricks::vision::Camera::prevImage() {
	
	ofDirectory imgFolder(imagesFolder);
	imgFolder.allowExt("png");
	imgFolder.allowExt("jpg");
	
	int numEntries = imgFolder.listDir();
	
	if(imagePos==-1) {
		imagePos = 0;
	} else {
		imagePos--;
		if(imagePos<0) {
			imagePos = numEntries-1;
		}
	}
	
	if(imagePos<numEntries && imagePos>=0) {
		image.loadImage(imgFolder.getPath(imagePos));
	}
}



void tricks::vision::Camera::setMode(int mode) {
 
    this->mode = mode;
    // check to see if we can actually flip to the desired mode.
    if(this->mode==CAMERA_MODE_FIREWIRE && this->canDoFirewire==false) {
        this->mode = CAMERA_MODE_QUICKTIME;
        if(!canDoCamera) {
            this->mode = CAMERA_MODE_IMAGES;
        }
    }
#ifdef TEST_WITH_VIDEO
	if(this->mode==CAMERA_MODE_FIREWIRE || this->mode==CAMERA_MODE_QUICKTIME) {
		this->mode = CAMERA_MODE_RECORDED;
	}
#endif
	
}
string tricks::vision::Camera::getImagesFolder() {
	return imagesFolder;
}

void tricks::vision::Camera::close() {
#ifdef _WIN32
#ifndef TEST_WITH_VIDEO
	sdk->close();
#endif
#endif
}