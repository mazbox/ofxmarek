


#import <UIKit/UIKit.h>
#import <MessageUI/MessageUI.h>
@interface iPhoneMailView : UIViewController <MFMailComposeViewControllerDelegate> {
	void(*successCallback)(void);
	void(*failedCallback)(void);
	void(*savedCallback)(void);
	void(*cancelledCallback)(void);
	
}
- (void) setSuccessCallback:(void(*)(void))_successCallback
			andFailedCallback:(void(*)(void))_failedCallback
andSavedCallback:(void(*)(void))_savedCallback
andCancelledCallback:(void(*)(void))_cancelledCallback;

- (void) showMessage:(NSString*)to 
		 withSubject:(NSString*)subject 
			withBody:(NSString*)body 
	  withAttachment:(NSData*)attachment 
		withFilename:(NSString*)filename  
		withMimeType:(NSString*)mimeType;
@end
