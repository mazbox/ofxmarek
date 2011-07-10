
#pragma once
namespace mareklib {
	
	/** draws the framerate in desired colour at the bottom left */
	void drawFramerate(int color = 0xFF0000);
	
	/** 
	 * For packaging mac apps - you need to add the line
	 * cp -r bin/data "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/"
	 * to your "run script build phase"
	 */
	void setDataPathRootToAppContents();
	
	/** Gives you a string time stamp */
	string dateTimeString();
	
	/** Gets the home directory (mac) */
	string getHomeDirectory();
	
	/** Gets you the path to the desktop for mac */
	string getDesktopPath();
	
	/** 
	 * Gets you the path the prefs directory for this app, 
	 * and creates a new one if it doesn't exist. You need
	 * to specify the app name so it knows what to name
	 * the directory.
	 */
	string getPreferencesDirectory(string appName);
};