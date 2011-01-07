import os
import re

controlPath = '../src/controls/basic'

controlsDir = os.listdir(controlPath)

controlFiles = []

def removeStarsAtBeginningsOfLines(text):
	lines = text.split("\n")
	out = ""
	for line in lines:
		if re.match("^[\s]*\*", line):
			line = line[line.find("*")+1:]
		out += line + "<br />"
			
	return out
	
def getHeader(code):
	start = code.find("/*")
	if start==-1:
		return ""
	end = code.find("*/")
	if end==-1:
		return ""
	if start>end:
		return ""
	
	return removeStarsAtBeginningsOfLines(code[start+1:end])
	
def createItem(text):
	return "<li>" + text + "</li>\n"
	
output = """
<html>
<head>
</head>
<body>
<ul>
"""

for file in controlsDir:

	if file.endswith(".h"):
		controlFiles.append(file)
 
for file in controlFiles:
	path = controlPath + '/' + file

	f = open(path, 'r')
	contents = f.read()
	output += createItem(getHeader(contents))
	
output += """
</ul>
</body>
</html>
"""

f = open("documentation.html", 'w')

f.write(output)


print output