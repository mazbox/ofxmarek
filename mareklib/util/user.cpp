string username = "push";
FILE *fp = popen("who am I", "r");
if(fp!=NULL) {
	char name[512];
	if(fgets(name, 512, fp)) {
		string username = name;
		if(username.find(' ')!=-1) {
			username = username.substr(0, username.find(' '));
			
			printf("%s\n", username.c_str());
		}
	}
} else {
	printf("Couldn't find user's name, going with default\n");
}