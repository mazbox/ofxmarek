/*
 *  StringUtils.cpp
 *  Gui
 *
 *  Created by Marek Bereza on 05/05/2010.
 *
 */
#include "StringUtils.h"

vector<string> split(string str, string delim) {
	vector<string> results;
	int cutAt;
	while( (cutAt = str.find_first_of(delim)) != str.npos ) {
		if(cutAt > 0) {
			results.push_back(str.substr(0,cutAt));
		}
		str = str.substr(cutAt+1);
	}
	if(str.length() > 0) {
		results.push_back(str);
	}
	return results;
}