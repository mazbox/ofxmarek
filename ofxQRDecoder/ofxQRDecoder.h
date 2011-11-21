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
	
	//@Overload
	//findQR
	// image: the image to explore to find codes
	// adaption_levels: higher for more precision, lower for fast elaboration (real-time).
	//
	// NOTES: Values below 1 will be interpreted as 1, values above 6 will be interpreted as 6
	string findQR(ofxCvGrayscaleImage image, unsigned short adaption_levels) {
	if (adaption_levels<=0) adaption_levels = 1;
	if (adaption_levels>6) adaption_levels=6;
	
		try {
			// set the image to analyze
			qr_decoder_set_image_buffer(decoder,image.getCvImage());
	    
			// try different levels of adaptiveness until we've decoded an image
			short sz,stat;
	    sz = 25;
	    unsigned short deltaSize = round(sz / adaption_levels) - (adaption_levels % 2); // calculate delta from levels, round the value and assure it is a multiple of 2
	    for(sz=25,stat=0; (sz>=3)&&((stat&QR_IMAGEREADER_DECODED)==0); sz-=deltaSize) {
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