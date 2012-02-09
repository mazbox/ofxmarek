#include "ofxQTKitVideoTexture.h"
#include "ofMain.h"
#import "Cocoa/Cocoa.h"
#import "QTKit/QTKit.h"

int ofxQTKitVideoTexture::videoCount = 0;
ofxQTKitVideoTexture::ofxQTKitVideoTexture() {
	videoCount++;
	qtVisualContext = NULL;
	currentFrame = NULL;
	movie = NULL;
	bLoaded = false;
	frameData = NULL;
	speed = 1;
	bPlaying = false;
	width = 0;
	height = 0;
	duration = 0;
	movieIsDone = false;
}

ofxQTKitVideoTexture::~ofxQTKitVideoTexture() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	videoCount--;
	if(movie==NULL) return;
	QTMovie *mov = (QTMovie*)movie;
	[mov release];
	[pool release];
	if(frameData!=NULL) delete [] frameData;
	
}
	
bool ofxQTKitVideoTexture::getMovieIsPlaying() {
	return bPlaying;
}
void ofxQTKitVideoTexture::play() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	if(movie==NULL) return;
	QTMovie *mov = (QTMovie*)movie;
	[mov play];
	bPlaying = true;
	movieIsDone = false;
	[pool release];
}
void ofxQTKitVideoTexture::stop() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	if(movie==NULL) return;
	QTMovie *mov = (QTMovie*)movie;
	[mov stop];
	bPlaying = false;
	[pool release];
}


void ofxQTKitVideoTexture::setLoopState(int looping) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	if(movie==NULL) return;
	QTMovie *mov = (QTMovie*)movie;
	bool loop = false;
	if(looping!=OF_LOOP_NONE) {
		loop = true;
	}
	[mov setAttribute:[NSNumber numberWithBool:loop] forKey:QTMovieLoopsAttribute];
	[pool release];
}

float ofxQTKitVideoTexture::getPosition() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	if(movie==NULL) return 0;
	QTMovie *mov = (QTMovie*)movie;
	QTTime dur = [mov currentTime];
	float pos = (float)dur.timeValue/dur.timeScale;
	[pool release];
	return pos/duration;
}


float ofxQTKitVideoTexture::getDuration() {
	return duration;
}


void ofxQTKitVideoTexture::setPosition(float pct) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	if(movie==NULL) return;
	QTMovie *mov = (QTMovie*)movie;
	float ms = pct*duration*1000.f;
	QTTime time = QTMakeTime(ms, 1000);
	[mov setCurrentTime:time];
	[pool release];
}

float ofxQTKitVideoTexture::getSpeed() {
	return speed;
}

void ofxQTKitVideoTexture::rewind() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	if(movie==NULL) return;
	QTMovie *mov = (QTMovie*)movie;
	[mov stop];
	[mov gotoBeginning];
	bPlaying = false;
	[pool release];
}

void ofxQTKitVideoTexture::setSpeed(float _speed) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	if(movie==NULL) return;
	speed = _speed;
	QTMovie *mov = (QTMovie*)movie;
	[mov setRate:speed];
	[pool release];
}


void ofxQTKitVideoTexture::setPaused(bool bPause) {
	if(bPlaying && !bPause) play();
	else if(!bPlaying && bPause) stop();
	bPlaying = !bPause;
}



float ofxQTKitVideoTexture::getHeight() { return height; }
float ofxQTKitVideoTexture::getWidth() { return width; }



@interface MovieListener : NSObject {
    ofxQTKitVideoTexture *video;

}
@end



@implementation MovieListener

- (id) init {
	if(self = [super init]) {

	}
    return self;
}


- (void)movieRateChanged:(NSNotification *)aNotification
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	id mov = [aNotification object];
	if(mov!=nil) {
		//printf("Rate %f\n", [mov rate]);
		if([mov rate]==0) {
			//printf("\n\nMOVIE ENDED!!!\n\n\n");
			video->movieEnded();
		}
	}
	[pool release];
	//video->movieEnded();
	//printf("\n\n\nMOVIE RATE CHANGED !!! \n\n\n");
}

@end



bool ofxQTKitVideoTexture::getIsMovieDone() {
	return movieIsDone;
}

void ofxQTKitVideoTexture::movieEnded() {	
	movieIsDone = true;
	
}

bool ofxQTKitVideoTexture::loadMovie(string name) {
	fileName = name;
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	if(movie!=NULL) {
		QTMovie *mov = (QTMovie*)movie;
		[mov release];
		[pool release];
		movie = NULL;
	}
	
	
	string path = ofToDataPath(name, true);
	NSString *nsPath = [NSString stringWithCString:path.c_str() encoding: NSASCIIStringEncoding];
	
	
	// check quicktime file
	if([QTMovie canInitWithFile:nsPath] == false) {
		NSLog(@"Couldn't load %@\n", nsPath);
		
		return false;
	}
	
	
	// load quicktime file
	QTMovie *qt = [[QTMovie alloc] initWithFile:nsPath error:NULL];

	NSError *err;

	MovieListener *ml = [[MovieListener alloc] init];
	ml->video = this;
	
	
	
	[
	 [NSNotificationCenter defaultCenter] 
	 addObserver:ml
	 selector:@selector(movieRateChanged:) 
	 name:QTMovieRateDidChangeNotification 
	 object:qt
	 ];
	
	
	
	
	movie = qt;
	listener = ml;
	
	if(movie == NULL) {
		NSLog(@"File %@ could not be loaded\n", nsPath);
		return false;
	}
	
	
//	NSLog(@"Loaded %@\n", nsPath);	
	width = 640;
	height = 480;
	
	


	NSArray* vtracks = [movie tracksOfMediaType:QTMediaTypeVideo];
	if([vtracks count]>0) {
		QTTrack* track = [vtracks objectAtIndex:0];
		NSSize size = [track apertureModeDimensionsForMode:QTMovieApertureModeClean];
		width = size.width;
		height = size.height;
	}
	
	
	
	NSDictionary	    *attributes = nil;
    attributes = [NSDictionary dictionaryWithObjectsAndKeys:
                  [NSDictionary dictionaryWithObjectsAndKeys:
                   [NSNumber numberWithFloat:width],
                   kQTVisualContextTargetDimensions_WidthKey,
                   [NSNumber numberWithFloat:height],
                   kQTVisualContextTargetDimensions_HeightKey, nil], 
                  kQTVisualContextTargetDimensionsKey, 
                 
				  
				  [NSDictionary dictionaryWithObjectsAndKeys:
                   [NSNumber numberWithFloat:width], 
                   kCVPixelBufferWidthKey, 
                   [NSNumber numberWithFloat:height], 
                   kCVPixelBufferHeightKey, nil], 
				  
				  
				  
                  kQTVisualContextPixelBufferAttributesKey,
                  nil];
	
	
	OSStatus error = QTOpenGLTextureContextCreate(NULL,
                                         CGLGetCurrentContext(),
										CGLGetPixelFormat(CGLGetCurrentContext()),
                                         (CFDictionaryRef)attributes,
                                         &qtVisualContext);
	if(error!=noErr) printf("Got error %d\n", error);
	QTMovie *mov = (QTMovie*)movie;
    // Associate it with our movie.
    SetMovieVisualContext([mov quickTimeMovie],qtVisualContext);
	
	
	QTTime dur = [mov duration];
	duration = (float)dur.timeValue/dur.timeScale;
	printf("Duration: %f\n", duration);
	
	//SetMoviePlayHints([mov quickTimeMovie],hintsHighQuality, hintsHighQuality);	
	[mov gotoBeginning];
	MoviesTask([mov quickTimeMovie], 0); 
	[pool release];
	bLoaded = true;
	return true;
}


bool ofxQTKitVideoTexture::update() {
	
	int start = ofGetElapsedTimeMillis();
	if(QTVisualContextIsNewImageAvailable(qtVisualContext,NULL)) {
		// Release the previous frame
		CVOpenGLTextureRelease(currentFrame);
		
		// Copy the current frame into our image buffer
		OSStatus s = QTVisualContextCopyImageForTime(qtVisualContext, NULL, NULL, &currentFrame);
	/*	if(s!=noErr) {
			printf("Error %d\n", s);
		} else {
			printf("Success\n");
		}*/
//		printf("Time: %d\n", ofGetElapsedTimeMillis() - start);

		return true;
		
	} else {
		//printf("No frame\n");

		return false;
	}
}

void ofxQTKitVideoTexture::setVolume(float volume) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	QTMovie *mov = (QTMovie*)movie;
	[mov setVolume:volume];
	[pool release];
}

void ofxQTKitVideoTexture::idle() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	QTMovie *mov = (QTMovie*)movie;
	[mov setIdling:YES];
	[pool release];
}


void ofxQTKitVideoTexture::bind() {
	if(currentFrame==NULL) {
		update();
		if(currentFrame==NULL) {
			printf("Current Frame is null\n");
			return;
		}
	}
	glEnable(CVOpenGLTextureGetTarget(currentFrame));
	glBindTexture(CVOpenGLTextureGetTarget(currentFrame), 
                  CVOpenGLTextureGetName(currentFrame));
}

void ofxQTKitVideoTexture::unbind() {
	if(currentFrame==NULL) return;
	QTVisualContextTask(qtVisualContext);
	glDisable(CVOpenGLTextureGetTarget(currentFrame));
}

void ofxQTKitVideoTexture::draw(float x, float y, float w, float h) {

	if(w==0) w = width;
	if(h==0) h = height;
	if(currentFrame==NULL) return;
	bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(x, y);
	glTexCoord2f(width, 0);
	glVertex2f(x+w, y);
	glTexCoord2f(width, height);
	glVertex2f(x+w, y+h);
	glTexCoord2f(0, height);
	glVertex2f(x, y+h);
	glEnd();
	unbind();
}