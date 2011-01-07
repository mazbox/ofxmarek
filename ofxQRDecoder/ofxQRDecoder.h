/*
 *  ofxQRDecoder.h
 *  openFrameworks
 *
 *  Created by Marek Bereza on 28/07/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#include "ofxOpenCv.h"
#include "decodeqr.h"
class ofxQRDecoder {
public:
	ofxQRDecoder() {
		decoder = qr_decoder_open();
		decodedString = "";
		timeDiscovered = 0.f;
	}
	
	string findQR(ofxCvGrayscaleImage image) {
		try {
			// set the image to analyze
			qr_decoder_set_image_buffer(decoder,image.getCvImage());
			
			// try different levels of adaptiveness until we've decoded an image
			short sz,stat;
            for(sz=25,stat=0; (sz>=3)&&((stat&QR_IMAGEREADER_DECODED)==0); sz-=2) {
                stat=qr_decoder_decode(decoder,sz);
			}
			
			if(stat==QR_IMAGEREADER_DECODED) {
				QrCodeHeader header;
				if(qr_decoder_get_header(decoder,&header)){
					//
					// get QR code text
					// To null terminate, a buffer size is larger than body size.
					//
					char buf[header.byte_size+1];
					qr_decoder_get_body(decoder,(unsigned char *)buf,header.byte_size+1);
					decodedString = buf;
					timeDiscovered = ofGetElapsedTimef();
				}
			}
		} catch(int i) {
			printf("We had an error from the Qr code reader\n");
			qr_decoder_set_status(decoder, 0);
		}
		if(ofGetElapsedTimef() - timeDiscovered>1.f) {
			decodedString = "";
		}
		return decodedString;
	}
	
private:
	QrDecoderHandle decoder;
	string decodedString;
	float timeDiscovered;
};