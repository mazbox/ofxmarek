/**
 * VideoSelector.h
 * VideoBackdrop
 *
 * Created by Marek Bereza on 29/06/2011.
 *
 */

#include "ofMain.h"
class VideoSelector {
public:
	void setup(string folder) {
		pos = 0;
		
		getPaths(folder);
		video.setUseTexture(true);
		if(paths.size()>0) {
			video.closeMovie();
			video.loadMovie(paths[pos]);
			video.play();
		}
	}
	
	ofVideoPlayer &getVideo() {
		return video;
	}
	void nextVideo() {
		/*pos++;
		pos %= paths.size();
		if(paths.size()>0) {
			video.closeMovie();
			video.loadMovie(paths[pos]);
			video.play();
		}*/
	}
	
	void prevVideo() {
		/*pos--;
		if(pos<0) pos = paths.size()-1;
		if(paths.size()>0) {
			video.closeMovie();
			video.loadMovie(paths[pos]);
			video.play();
		}*/
	}
	
	void setVideo(int imageId) {
		/*if(pos!=imageId) {
			pos = imageId % paths.size();
			if(paths.size()>0) {
				string path = paths[pos];
				video.closeMovie();
				video.loadMovie(path);
				video.play();
			}
		}*/
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
	int pos;
	ofVideoPlayer video;
};
