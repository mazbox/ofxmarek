

#import "iPhoneMailView.h"

#include "iPhoneUtils.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
@implementation iPhoneMailView




- (void) setSuccessCallback:(void(*)(void))_successCallback
		  andFailedCallback:(void(*)(void))_failedCallback
		   andSavedCallback:(void(*)(void))_savedCallback
	   andCancelledCallback:(void(*)(void))_cancelledCallback;
{
	
	
	successCallback = _successCallback;
	failedCallback = _failedCallback;
	savedCallback = _savedCallback;
	cancelledCallback = _cancelledCallback;
	
}

- (id) init {
	if (self = [super init]) {
		successCallback = nil;
		failedCallback = nil;
		cancelledCallback = nil;
		savedCallback = nil;
	}
	
	return self;
}   
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}
-(void) show: (MFMailComposeViewController *)mail {
	[ofxiPhoneGetUIWindow() addSubview:self.view];
	//[ofxiPhoneGetUIWindow() addSubview:mail.view];
    [self presentModalViewController:mail animated:YES];
    [mail release];
}

- (void) showMessage:(NSString*)to 
		 withSubject:(NSString*)subject 
			withBody:(NSString*)body 
	  withAttachment:(NSData*)attachment 
		withFilename:(NSString*)filename  
		withMimeType:(NSString*)mimeType {
	
	if(!canSendEmail()) {
		warnWithAlert("No Email Account", "There is no email account set up on this device. Please set it up in order to use this feature\n");

		return;
	}
	MFMailComposeViewController *mail = [[MFMailComposeViewController alloc] init];
	//[picker setDisplayedProperties:[NSArray arrayWithObject:[NSNumber numberWithInt:kABPersonEmailProperty]]];
    mail.mailComposeDelegate = self;
	
	
	[mail setSubject:subject];
	if([to length]>0) {
		[mail setToRecipients:[NSArray arrayWithObjects:to, nil]];
	}
	[mail setMessageBody:body isHTML: YES];
	
	if(attachment!=nil) {
		NSLog(@"here are the attachment details: %@  %@", mimeType, filename);
		[mail addAttachmentData:attachment mimeType:mimeType fileName:filename];
	}
	
	
	
	[self performSelectorOnMainThread:@selector(show:) withObject:mail waitUntilDone:NO];
	
}
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}


- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}




#pragma mark -
#pragma mark MFMail delegate methods
- (void)
mailComposeController:(MFMailComposeViewController*)controller 
			didFinishWithResult:(MFMailComposeResult)result 
error:(NSError*)error {
	[controller dismissModalViewControllerAnimated:YES];
	if(result!=MFMailComposeResultFailed) {
		

#ifdef __DEBUG__
		Dprintf("Send mail succeeded\n");
#endif
		if(cancelledCallback!=nil) {
			if(result==MFMailComposeResultCancelled) {
				cancelledCallback();
			}
		}

		if(successCallback!=nil) {
			if(result==MFMailComposeResultSent) {
				successCallback();
			}
		}
		if(savedCallback!=nil) {
			if(result==MFMailComposeResultSaved) {
				savedCallback();
			}
		}
	} else {
		
#ifdef __DEBUG__
		if(error==MFMailComposeErrorCodeSaveFailed) {
			Dprintf("Save failed\n");
		} else if(error.code==MFMailComposeErrorCodeSendFailed) {
			Dprintf("Send failed\n");
		}
#endif
		if(failedCallback!=nil) failedCallback();

	}
	for (UIView *v in [ofxiPhoneGetUIWindow() subviews]) {
		if(v!=ofxiPhoneGetGLView()) {
			[v removeFromSuperview];
		}
	}
	/*[self.view removeFromSuperview];
	[self release];
	[ofxiPhoneGetUIWindow() makeKeyAndVisible];*/
}

@end
