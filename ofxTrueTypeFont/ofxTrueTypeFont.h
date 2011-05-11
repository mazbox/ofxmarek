#ifndef _OFX_TTF_FONT_H_
#define _OFX_TTF_FONT_H_


#include "ofTrueTypeFont.h"
#include "ofMain.h"


class ofxTrueTypeFont{

public:


	ofxTrueTypeFont();
	virtual ~ofxTrueTypeFont();
		
	// 			-- default, non-full char set, anti aliased:
	void 		loadFont(string filename, int fontsize);
	void 		loadFont(string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours = false);

	bool		bLoadedOk;
	bool 		bAntiAlised;
	bool 		bFullCharacterSet;

  	float 		getLineHeight();
  	void 		setLineHeight(float height);
	float 		stringWidth(string s);
	float 		stringHeight(string s);
	
	ofRectangle    getStringBoundingBox(string s, float x, float y);
	
	void 		drawString(string s, float x, float y);
	void		drawStringAsShapes(string s, float x, float y);
	
	int 		nCharacters;
	
	ofTTFCharacter getCharacterAsPoints(int character);
	
	void setWordSpacing(float ws);
	float getWordSpacing();
	void setLetterSpacing(float ls);
	float getLetterSpacing();
	float getDescenderHeight();
protected:
	vector <ofTTFCharacter> charOutlines;
	
	float			letterSpacing;
	float 			lineHeight;
	float			wordSpacing;
	
	charProps 		* 	cps;			// properties for each character
	GLuint			*	texNames;		// textures for each character
	int				fontSize;
	bool			bMakeContours;

	void 			drawChar(int c, float x, float y);
	void			drawCharAsShape(int c, float x, float y);
	
	int 			ofNextPow2(int a);
	int				border, visibleBorder;

	
};


#endif

