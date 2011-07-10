/**
 * ImageSelector.h
 * Box2dContour
 *
 * Created by Marek Bereza on 27/06/2011.
 *
 */
#include "ofMain.h"
class ImageSelector {
public:
	void setup(string folder) {
		pos = 0;
		
		getPaths(folder);
		
		for(int i = 0; i < paths.size(); i++) {
			images.push_back(ofImage());
			images.back().loadImage(paths[i]);
		}
	}
	
	ofImage *getImage() {
		return &images[pos];
	}
	void nextImage() {
		pos++;
		pos %= paths.size();



	}
	
	void prevImage() {
		pos--;
		if(pos<0) pos = paths.size()-1;

	}

	void setImage(int imageId) {
		if(pos!=imageId) {
			pos = imageId % paths.size();
		}
	}
	
	int getPos() {
		return pos;
	}
	int size() {
		return paths.size();
	}
	
private:
	void getPaths(string folder) {
		paths.clear();
		ofDirectory dir;

		int numItems = dir.listDir(folder);

		for(int i = 0; i < numItems; i++) {
			paths.push_back(dir.getPath(i));
		}
		dir.close();
		
	}
	vector<string> paths;
	vector<ofImage> images;
	int pos;
	ofImage *image;
};
