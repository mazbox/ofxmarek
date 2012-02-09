/**     ___           ___           ___                         ___           ___     
 *     /__/\         /  /\         /  /\         _____         /  /\         /__/|    
 *    |  |::\       /  /::\       /  /::|       /  /::\       /  /::\       |  |:|    
 *    |  |:|:\     /  /:/\:\     /  /:/:|      /  /:/\:\     /  /:/\:\      |  |:|    
 *  __|__|:|\:\   /  /:/~/::\   /  /:/|:|__   /  /:/~/::\   /  /:/  \:\   __|__|:|    
 * /__/::::| \:\ /__/:/ /:/\:\ /__/:/ |:| /\ /__/:/ /:/\:| /__/:/ \__\:\ /__/::::\____
 * \  \:\~~\__\/ \  \:\/:/__\/ \__\/  |:|/:/ \  \:\/:/~/:/ \  \:\ /  /:/    ~\~~\::::/
 *  \  \:\        \  \::/          |  |:/:/   \  \::/ /:/   \  \:\  /:/      |~~|:|~~ 
 *   \  \:\        \  \:\          |  |::/     \  \:\/:/     \  \:\/:/       |  |:|   
 *    \  \:\        \  \:\         |  |:/       \  \::/       \  \::/        |  |:|   
 *     \__\/         \__\/         |__|/         \__\/         \__\/         |__|/   
 *
 *  Description: Abstracts either a normal camera, a firewire camera or a
 *				 plain video all with the same interface.
 *
 *  Camera.h, created by Marek Bereza on 15/07/2011.
 */

#pragma once

#include "ofMain.h"
#include "ofxVideoGrabber.h"

#define CAMERA_MODE_QUICKTIME	1
#define CAMERA_MODE_FIREWIRE	2
#define CAMERA_MODE_RECORDED	3
#define CAMERA_MODE_IMAGES		4
#ifdef _WIN32
#include "Listener.h"
#endif

namespace tricks {
	namespace vision {

		class Camera 
		#ifdef _WIN32
		: public xmlgui::Listener
		#endif
		{
		public:
		#ifdef _WIN32
			void controlChanged(xmlgui::Control *ctrl);
		#endif
			~Camera() { close(); }
			void setup(int width, int height, string testVideoFile = "", string imageFolder = "no-images");
			void update();
			void draw(float x, float y);
			void draw(float x, float y, float width, float height);
			ofTexture &getTextureReference();
			unsigned char *getPixels();
			int getWidth();
			int getHeight();
			void videoSettings();
			void setPaused(bool paused);
			bool isPaused();
			
			void drawSettings();
			void saveFrame();
			
			void nextImage();
			void prevImage();
			
			void setMode(int mode);
			string getImagesFolder();
			void close();
		private:
			bool canDoFirewire;
			bool canDoCamera;
			
			bool saveNextFrame;
			int mode;
			ofVideoGrabber videoGrabber;
			ofVideoPlayer  testDataPlayer;
			ofxVideoGrabber firewire;
			
			// these are for the images type
			ofImage image;
			string imagesFolder;
			int imagePos;
			
		};
	}
}