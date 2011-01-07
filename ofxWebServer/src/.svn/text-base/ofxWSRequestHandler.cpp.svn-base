/*
 *  ofxWSRequestHandler.cpp
 *  webServerExample
 *
 *  Created by Marek Bereza on 27/03/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */


#include "ofxWSRequestHandler.h"


void ofxWSRequestHandler::httpResponse(string data) {
	mg_printf(conn, "%s",data.c_str());
}


void ofxWSRequestHandler::httpResponseData(char *data, int length) {
	mg_write(conn, (const void *)data, length);
}


/**
 * Returns the client's ip address
 */
string ofxWSRequestHandler::getIPAddress() {
	return ipAddress;
}

/**
 * gets a parameter from the request.
 */
string ofxWSRequestHandler::getRequestParameter(string name) {
	string result = "";
	int pos = query.find(name+"=");
	if(pos!=-1) {
		int end = query.find("&", pos);
		pos = pos + name.size() + 1;
		if(end!=-1) {
			result = query.substr(pos, end - pos);
		} else {
			result = query.substr(pos);
		}
	}
	return result;
}

/**
 * returns the query string from the request
 */
string ofxWSRequestHandler::getQuery() {
	return query;
}