/*
 *  ofxQTKitFormatOptions.h
 *  AlphaVideoExporter
 *
 *  Created by Marek Bereza on 07/12/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */



enum ofxQTKitQuality {
	ofxQTKitQuality_Lossless          = 0x00000400,
	ofxQTKitQuality_Max               = 0x000003FF,
	ofxQTKitQuality_Min               = 0x00000000,
	ofxQTKitQuality_Low               = 0x00000100,
	ofxQTKitQuality_Normal            = 0x00000200,
	ofxQTKitQuality_High              = 0x00000300
};

enum ofxQTKitFormat {
	
	
	
	ofxQTKitFormat_Cinepak,			//'cvid'
	ofxQTKitFormat_JPEG,			//'jpeg'
	ofxQTKitFormat_Graphics,		//'smc '
	ofxQTKitFormat_Animation,		//'rle '
	ofxQTKitFormat_AppleVideo,		//'rpza'
	ofxQTKitFormat_KodakPhotoCD,	//'kpcd'
	ofxQTKitFormat_PNG,				//'png '
	ofxQTKitFormat_MotionJPEG_A,	//'mjpa'
	ofxQTKitFormat_MotionJPEG_B,	//'mjpb'
	ofxQTKitFormat_SorensonV1,		//'SVQ1'
	ofxQTKitFormat_SorensonV3,		//'SVQ3'
	ofxQTKitFormat_MPEG4,			//'mp4v'
	ofxQTKitFormat_NTSC_DV25,		//'dvc '
	ofxQTKitFormat_PAL_DV25,		//'dvcp'
	ofxQTKitFormat_GIF,				//'gif '
	ofxQTKitFormat_H263,			//'h263'
	ofxQTKitFormat_TIFF,			//'tiff'
	ofxQTKitFormat_RGB,				//'raw '
	
	//Uncompressed Y′CbCr, 8-bit-per-component 4:2:2
	ofxQTKitFormat_2vuy,			//'2vuY'
	
	//Uncompressed Y′CbCr, 8-bit-per-component 4:2:2,
	ofxQTKitFormat_yuv2,			//'yuv2'
	
	//Uncompressed Y′CbCr, 8-bit-per-component 4:4:4, 
	ofxQTKitFormat_v308,			//'v308'
	
	//Uncompressed Y′CbCr, 8-bit-per-component 4:4:4:4, 
	ofxQTKitFormat_v408,			//'v408'
	
	//Uncompressed Y′CbCr, 10, 12, 14, or 16-bit-per-component 4:2:2, 
	ofxQTKitFormat_v216,			//'v216'
	
	//Uncompressed Y′CbCr, 10-bit-per-component 4:4:4, 
	ofxQTKitFormat_v410,			//'v410'
	
	//Uncompressed Y′CbCr, 10-bit-per-component 4:2:2, 
	ofxQTKitFormat_v210,			//'v210'
	
};
