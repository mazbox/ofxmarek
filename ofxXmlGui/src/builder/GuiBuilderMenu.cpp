/*
 *  GuiBuilderMenu.cpp
 *  Gui
 *
 *  Created by Marek Bereza on 15/04/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "GuiBuilderMenu.h"

#ifdef GLUT_API_VERSION
map<int,string> controlIds;
GuiBuilderMenu *builder;
void processMenuEvents(int eventId) {
	if(controlIds[eventId].find("new_")!=-1) {
		string type = controlIds[eventId].substr(4);
		builder->addElement(type, "new "+type, builder->lastMousePress.x-builder->gui->x, builder->lastMousePress.y-builder->gui->y);
	} else if(controlIds[eventId]==DELETE) {
		builder->inspector->clear();
		builder->gui->deleteFocusedControl();	
	}
	

}

void addGlutMenus(GuiBuilderMenu *_builder) {
	printf("ADded glut menus\n");
	builder = _builder;
	int menu = glutCreateMenu(processMenuEvents);
	vector<string> types = GuiControl::getTypes();
	controlIds[-1] = DELETE;
	glutAddMenuEntry("delete", -1);
	glutAddMenuEntry("-", -5);

	for(int  i = 0; i < types.size(); i++) {
		controlIds[i] = (string("new_")+types[i]).c_str();
		glutAddMenuEntry(types[i].c_str(), i);
	}
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
#endif


#ifdef TARGET_OSX
#include <Carbon/Carbon.h>
CFURLRef GetOpenDialogForUser(kDialogType type, char* title, char* message)
{
	NavDialogCreationOptions dialogOptions;
	NavDialogRef dialog = NULL;
	NavReplyRecord replyRecord;
	CFURLRef fileAsCFURLRef = NULL;
	FSRef fileAsFSRef;
	OSStatus status;
	
	// Get the standard set of defaults
	status = NavGetDefaultDialogCreationOptions(&dialogOptions);
	//require_noerr( status, CantGetNavOptions );
	
	dialogOptions.optionFlags = kNavNoTypePopup + kNavSupportPackages + kNavAllowOpenPackages;
	
	CFAllocatorRef alloc_default = kCFAllocatorDefault;  // = NULL;
	
	if (title != NULL) {
		CFStringRef cftitle = CFStringCreateWithCString(alloc_default,title,kCFStringEncodingMacRoman);
		dialogOptions.windowTitle = cftitle;
	}
	
	if (message != NULL) {
		CFStringRef cfmessage = CFStringCreateWithCString(alloc_default,message,kCFStringEncodingMacRoman);
		dialogOptions.message = cfmessage;
	}
	
	
	// Make the window app-wide modal
	dialogOptions.modality = kWindowModalityAppModal;
	
	// Create the dialog
	if (type == kDialogFile) {
		status = NavCreateGetFileDialog(&dialogOptions, NULL, NULL, NULL, NULL, NULL, &dialog);
	} else if (type == kDialogFolder) {
		status = NavCreateChooseFolderDialog(&dialogOptions, NULL, NULL, NULL, &dialog);
	}
	require_noerr( status, CantCreateDialog );
	
	// Show it
	status = NavDialogRun(dialog);
	require_noerr( status, CantRunDialog );
	
	// Get the reply
	status = NavDialogGetReply(dialog, &replyRecord);
	require( ((status == noErr) || (status == userCanceledErr)), CantGetReply );
	
	// If the user clicked "Cancel", just bail
	if ( status == userCanceledErr ) goto UserCanceled;
	
	// Get the file
	status = AEGetNthPtr(&(replyRecord.selection), 1, typeFSRef, NULL, NULL, &fileAsFSRef, sizeof(FSRef), NULL);
	require_noerr( status, CantExtractFSRef );
	
	// Convert it to a CFURL
	fileAsCFURLRef = CFURLCreateFromFSRef(NULL, &fileAsFSRef);
	
	// Cleanup
CantExtractFSRef:
UserCanceled:
	verify_noerr( NavDisposeReply(&replyRecord) );
CantGetReply:
CantRunDialog:
	NavDialogDispose(dialog);
CantCreateDialog:
//CantGetNavOptions:
    return fileAsCFURLRef;
}

CFURLRef GetSaveDialogForUser(char* title, char* message)
{
	NavDialogCreationOptions dialogOptions;
	FSRef output_file;
	CFURLRef fileAsCFURLRef = NULL;
	NavReplyRecord replyRecord;
	OSStatus status;
	
	// Get the standard set of defaults
	status = NavGetDefaultDialogCreationOptions( &dialogOptions );
	//require_noerr( status, CantGetNavOptions );
	
	dialogOptions.optionFlags = kNavNoTypePopup + kNavSupportPackages + kNavAllowOpenPackages;
	
	CFAllocatorRef alloc_default = kCFAllocatorDefault;  // = NULL;
	
	if (title != NULL) {
		CFStringRef cftitle = CFStringCreateWithCString(alloc_default,title,kCFStringEncodingMacRoman);
		dialogOptions.windowTitle = cftitle;
	}
	
	if (message != NULL) {
		CFStringRef cfmessage = CFStringCreateWithCString(alloc_default,message,kCFStringEncodingMacRoman);
		dialogOptions.message = cfmessage;
	}
	// Make the window app-wide modal
	dialogOptions.modality = kWindowModalityAppModal;
	
	NavDialogRef dialog;
	status = NavCreatePutFileDialog ( &dialogOptions, NULL, NULL, NULL, NULL, &dialog);
	//require_noerr( status, CantCreateDialog );
	
	status = NavDialogRun(dialog);
	//require_noerr( status, CantRunDialog );
	
	// get dialog reply
	NavReplyRecord reply;
	status = NavDialogGetReply(dialog, &reply);
	//require( ((status == noErr) || (status == userCanceledErr)), CantGetReply );
	
	//get file directory
	AEKeyword keyword;
	DescType actual_type;
	Size actual_size;
	FSRef output_dir;
	status = AEGetNthPtr(&(reply.selection), 1, typeFSRef, &keyword, &actual_type,
						 &output_dir, sizeof(output_file), &actual_size);
	//require_noerr( status, CantExtractFSRef );
	
	UInt8 output_dir_name[1024];
	FSRefMakePath(&output_dir, output_dir_name, 1024 );
	
	// now get filename
	CFIndex len = CFStringGetLength(reply.saveFileName);
	if (len > 255)
		len = 255;
	UniChar output_filename[255];
	CFStringGetCharacters(reply.saveFileName, CFRangeMake(0, len), output_filename);
	
	// need to unlink the old file
	if ( reply.replacing )
	{
		FSRef oldfile;
		
		status = FSMakeFSRefUnicode(&output_dir, len, output_filename,
									kTextEncodingUnicodeDefault,
									&oldfile);
		if (status == noErr) status = FSDeleteObject(&oldfile);
		//overwrite failed!
		require_noerr( status, UserCanceled );
	}
	
	//create fsref again to new file (NOTE: this actually makes a file...)
	status = FSCreateFileUnicode( &output_dir, len, output_filename, kFSCatInfoNone,
								 NULL, &output_file, NULL );
	//require_noerr( status, CantExtractFSRef );
	
	// Convert it to a CFURL
	fileAsCFURLRef = CFURLCreateFromFSRef(NULL, &output_file);
	
//CantExtractFSRef:
UserCanceled:
	verify_noerr( NavDisposeReply(&replyRecord) );
//CantGetReply:
//CantRunDialog:
	// cleanup dialog
	NavDialogDispose(dialog);
//CantCreateDialog:
//CantGetNavOptions:
    return fileAsCFURLRef;
}


//--------------------------------------------------------------
string getStringFromDialog(kDialogType type, char* windowTitle, char* windowMessage) {
	CFURLRef cfUrl = NULL;
	
	cfUrl = GetOpenDialogForUser(type, windowTitle, windowMessage);
	
	CFStringRef cfString = NULL;
	
	string fileNameToLoad = "";
	
	if ( cfUrl != NULL )
	{
		cfString = CFURLCopyFileSystemPath( cfUrl, kCFURLPOSIXPathStyle );
		CFRelease( cfUrl );
		
		// copy from a CFString into a local c string (http://www.carbondev.com/site/?page=CStrings+)
		const int kBufferSize = 255;
		
		char folderURL[kBufferSize];
		Boolean bool1 = CFStringGetCString(cfString,folderURL,kBufferSize,kCFStringEncodingMacRoman);
		
		
		string url1 = folderURL;
		string url2 = "";
		string finalURL = url1 + "/" + url2;
		fileNameToLoad = finalURL;
		
		if (fileNameToLoad.length() > 0)
			fileNameToLoad.erase(fileNameToLoad.length()-1);
	}
	
	return fileNameToLoad;
}

//--------------------------------------------------------------
string getStringFromSaveDialog (char* windowTitle, char* windowMessage){
	CFURLRef cfUrl = NULL;
	
	cfUrl = GetSaveDialogForUser(windowTitle, windowMessage);
	
	CFStringRef cfString = NULL;
	
	string fileNameToSave = "";
	
	if ( cfUrl != NULL )
	{
		cfString = CFURLCopyFileSystemPath( cfUrl, kCFURLPOSIXPathStyle );
		CFRelease( cfUrl );
		
		// copy from a CFString into a local c string (http://www.carbondev.com/site/?page=CStrings+)
		const int kBufferSize = 255;
		
		char folderURL[kBufferSize];
		Boolean bool1 = CFStringGetCString(cfString,folderURL,kBufferSize,kCFStringEncodingMacRoman);
		
		
		string url1 = folderURL;
		string url2 = "";
		string finalURL = url1 + "/" + url2;
		fileNameToSave = finalURL;
		
		if (fileNameToSave.length() > 0) fileNameToSave.erase(fileNameToSave.length()-1);
	}
	return fileNameToSave;
	
}

#endif