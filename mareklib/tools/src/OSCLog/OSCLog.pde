import oscP5.*;
import controlP5.*;

import java.awt.*;

OscP5 oscP5;
ControlP5 controlP5;

String log = "";
long lastMessageTime = -10000L;
Textfield portTextfield;
controlP5.Button connectButton;
String port;
int portInt = 12345;

void setup() {
    
    frame.setResizable(true);
    loadPort();
    
    port = "" + portInt;
    
    size(350, 400);
    frameRate(25);
    oscP5 = new OscP5(this, portInt);
    setFont("Courier New", 12);
    
    
    
    controlP5 = new ControlP5(this);
    portTextfield = controlP5.addTextfield("port", 10,height-40,60,20);
    portTextfield.setAutoClear(true);
    portTextfield.setText(port);
    connectButton = controlP5.addButton("RECONNECT",0,(int)portTextfield.absolutePosition().x+70,(int)portTextfield.absolutePosition().y,50,20);
}

void controlEvent(ControlEvent theEvent) {
    //println(theEvent.label());
    try {
        portInt = Integer.parseInt(portTextfield.getText());
        oscP5 = new OscP5(this, portInt);
        savePort();
    } catch(Exception e) {
        portTextfield.setText(""+portInt);
    }
}
void loadPort() {
    String lines [] = loadStrings("OSCLogPort.txt");
    if(lines!=null && lines.length>0) {
        try {
            portInt = Integer.parseInt(lines[0]);
        } catch(Exception e) {
            portInt = 12345;
        }
    }
}

void savePort() {
    String [] str = new String[1];
    str[0] = ""+portInt;
    saveStrings("OSCLogPort.txt", str);
}
void draw() {
    
    portTextfield.setPosition(10,height-40);
    connectButton.setPosition(portTextfield.position().x+70,portTextfield.position().y);
    if(!mousePressed) {
        background(0);
        noStroke();
        long l = (long)millis() - lastMessageTime;
        if(l < 1000) {
            fill(100, 0.0, 0.0, map(l, 0.0, 1000, 255, 0.0));
            rect(0.0, 0.0, width, 17);
        }
        fill(255);
        text(log, 5F, 12F);
        if(log.length() > 10000)
            log = log.substring(0, 10000);
    }
    fill(20,20,20, 180);
    rect(0, height-50, width, 50);
}
    
void setFont(String s, int i) {
    Font font = new Font(s, 0, i);
        PFont pfont = new PFont(font, true, new char[] {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 
        'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 
        'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 
        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
        'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', 
        '8', '9', '!', '%', '*', '(', ')', '-', '_', '+', 
        '=', ';', ':', '\'', '"', '\\', '|', '/', '?', '<', 
        '>', ',', '.'
    });
    textFont(pfont, i);
}
    
public void oscEvent(OscMessage oscmessage) {
    String s = oscmessage.addrPattern();
    String s1 = oscmessage.typetag();
    for(int i = 0; i < s1.length(); i++) {
        if(s1.charAt(i) == 'i')
            s = (new StringBuilder(String.valueOf(s))).append(" ").append(oscmessage.get(i).intValue()).toString();
        if(s1.charAt(i) == 'f')
            s = (new StringBuilder(String.valueOf(s))).append(" ").append(oscmessage.get(i).floatValue()).toString();
        if(s1.charAt(i) == 'd')
            s = (new StringBuilder(String.valueOf(s))).append(" ").append(oscmessage.get(i).doubleValue()).toString();
        if(s1.charAt(i) == 's')
            s = (new StringBuilder(String.valueOf(s))).append(" \"").append(oscmessage.get(i).stringValue()).append("\"").toString();
        if(s1.charAt(i) == 'S')
            s = (new StringBuilder(String.valueOf(s))).append(" ").append(oscmessage.get(i).stringValue()).toString();
        if(s1.charAt(i) == 'r')
            s = (new StringBuilder(String.valueOf(s))).append(" color(").append(oscmessage.get(i).intValue()).append(")").toString();
        if(s1.charAt(i) == 'T')
            s = (new StringBuilder(String.valueOf(s))).append(" TRUE").toString();
        if(s1.charAt(i) == 'F')
            s = (new StringBuilder(String.valueOf(s))).append(" FALSE").toString();
        if(s1.charAt(i) == 'N')
            s = (new StringBuilder(String.valueOf(s))).append(" NIL").toString();
        if(s1.charAt(i) == 'I')
            s = (new StringBuilder(String.valueOf(s))).append(" INFINITUM").toString();
        if(s1.charAt(i) == 'b')
            s = (new StringBuilder(String.valueOf(s))).append(" (blob)").toString();
        if(s1.charAt(i) == 't')
            s = (new StringBuilder(String.valueOf(s))).append(" (time tag)").toString();
    }

    log = (new StringBuilder(String.valueOf(s))).append("\n").append(log).toString();
    lastMessageTime = millis();
}
