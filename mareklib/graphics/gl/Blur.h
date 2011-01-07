#include "ofxFbo.h"
#include "ofxShader.h"

class Blur {
public:
	void setup(float width, float height) {
		fbo1.setup(width, height, GL_RGBA);
		fbo2.setup(width, height, GL_RGBA);
		noPasses = 2;
		blurDistance = 2.0;
		string vertSource = "void main() { "
		"gl_TexCoord[0] = gl_MultiTexCoord0; "
		"gl_Position = ftransform();  "
		"} "
		;
		shaderH.setupShaderFromSource(GL_VERTEX_SHADER, vertSource);
		shaderV.setupShaderFromSource(GL_VERTEX_SHADER, vertSource);
		
		shaderH.setupShaderFromSource(GL_FRAGMENT_SHADER, 
									  
		
"	  uniform sampler2DRect src_tex_unit0;"
"	  uniform float blurAmnt;"	  
"	  void main( void )"
"		{  "
"			vec2 st = gl_TexCoord[0].st;"			
"			vec4 color;"
"			color		+= 1.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * -4.0, 0.0));"
"			color		+= 2.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * -3.0, 0.0));"
"			color		+= 3.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * -2.0, 0.0));"
"			color		+= 4.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * -1.0, 0.0));"	
"			color		+= 5.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt, 0));"
"			color		+= 4.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * 1.0, 0.0));"
"			color		+= 3.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * 2.0, 0.0));"
"			color		+= 2.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * 3.0, 0.0));"
"			color		+= 1.0 * texture2DRect(src_tex_unit0, st + vec2(blurAmnt * 4.0, 0.0));"
"			color /= 25.0;"
"			gl_FragColor = color;"
"		}"
									  
									  
									  
		);
		
		
		shaderV.setupShaderFromSource(GL_FRAGMENT_SHADER, 
									  
									  
"	  uniform sampler2DRect src_tex_unit0;"
"	  uniform float blurAmnt;"
"	  void main( void ) {  "
"			vec2 st = gl_TexCoord[0].st;"
"			vec4 color;"
"			color		+= 1.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmnt * 4.0));"
"			color		+= 2.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmnt * 3.0));"
"			color		+= 3.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmnt * 2.0));"
"			color		+= 4.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmnt * 1.0));"	
"			color		+= 5.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmnt) );"
"			color		+= 4.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmnt * -1.0));"
"			color		+= 3.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmnt * -2.0));"
"			color		+= 2.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmnt * -3.0));"
"			color		+= 1.0 * texture2DRect(src_tex_unit0, st + vec2(0.0, blurAmnt * -4.0));"

"			color /= 25.0;"
"			gl_FragColor = color;"
"		}"
			  
									  
									  );
		
		shaderH.linkProgram();
		shaderV.linkProgram();
	}
	void begin() {
		fbo2.begin();
		ofClear(0, 0, 0, 0);
		fbo2.end();
		fbo1.begin();
		ofClear(0, 0, 0, 0);
	}
	
	void end() {
		fbo1.end();
		blur();
		fbo1.draw(0, 0);
	}
	
	
	void blur() {
		ofxFbo *src, *dst;
		src = &fbo1;
		dst = &fbo2;
		
		for(int i = 0; i < noPasses; i++) {
			//printf("Blur %d\n", i);
			shaderH.begin();
			shaderH.setUniform1f("blurAmnt", blurDistance);
			dst->begin();
			src->draw(0, 0);
			dst->end();
			shaderH.end();
			
			shaderV.begin();
			shaderV.setUniform1f("blurAmnt", blurDistance);
			src->begin();
			dst->draw(0, 0);
			src->end();
			shaderV.end();
		}
	}
	
	
	
private:
	ofxShader shaderH;
	ofxShader shaderV;
	
	ofxFbo fbo1, fbo2;
	
	float blurDistance;
	int noPasses;
};
