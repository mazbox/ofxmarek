#pragma once
#include <string>
using namespace std;

/**
 * Returns 1 if this device can send emails, 0 if it can't
 */
bool canSendEmail();

class MailListener {
public:
	virtual void mailSent() = 0;
	virtual void mailCancelled() {}
	virtual void mailSaved() {}
	virtual void mailFailed() = 0;
};

void iPhoneSendEmail(char *to, char *subject, char *body, char *attachmentPath, MailListener *listener = NULL);


string getTmpPath();
string getBundlePath();



bool hasInternetConnection();


int getFreeMemory();


string iPhoneGetDocsDir();

string getUDID();

/** Warns the user with a standard alert dialog */
void warnWithAlert(string title, string message);


void launchUrl(string url);



bool downloadFile(string url, string destination);