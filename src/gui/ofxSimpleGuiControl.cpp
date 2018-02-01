/***********************************************************************

 Copyright (c) 2008, 2009, 2010, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.

 based on Todd Vanderlin's ofxSimpleGui API
 http://toddvanderlin.com/

 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/


#include "ofxSimpleGuiControl.h"


ofxSimpleGuiControl::ofxSimpleGuiControl(string name) {
	controlType = "";
	this->config = &defaultSimpleGuiConfig;
	setName(name);
	setKey(key);
	setPosition(0, 0);
	lock	  = false;
	focused	  = false;
	newColumn = false;
	bLearning = false;
	bLearnt = false;
	setKeyboardShortcut(0);

	setup();

	disableAllEvents();		// just for safety to make sure nothing is registered twice
//	enableAppEvents();
//	enableMouseEvents();
//	disableKeyEvents();
}

ofxSimpleGuiControl &ofxSimpleGuiControl::setConfig(ofxSimpleGuiConfig *config) {
	this->config = config;
	setup();
	return *this;
}

ofxSimpleGuiControl &ofxSimpleGuiControl::setNewColumn(bool b) {
	newColumn = b;
	return *this;
}



ofxSimpleGuiControl &ofxSimpleGuiControl::setName(string newName) {
	name = newName;
	origName = name;
	if(key.compare("") == 0) setKey("");	// if key has not been set yet, set name as key too

	hasTitle = (name.compare("") != 0);
	return *this;
}


ofxSimpleGuiControl &ofxSimpleGuiControl::setKey(string newKey) {
	if(newKey.compare("") == 0) key = name;
	else key = newKey;
	for(int i=0; i<key.size(); i++) {
		if(key[i] == ' ') key[i] = '_';
	}
	return *this;
}

ofxSimpleGuiControl &ofxSimpleGuiControl::setTextColor(bool clickable) {
	if(isMouseOver() && clickable) ofSetHexColor(config->textOverColor);
	else ofSetHexColor(config->textColor);
	return *this;
}

ofxSimpleGuiControl &ofxSimpleGuiControl::setTextBGColor(bool clickable) {
	if(isMouseOver() && clickable) ofSetHexColor(config->textBGOverColor);
	else ofSetColor(config->textBGColor.r, config->textBGColor.g, config->textBGColor.b, config->textBGColor.a);
	return *this;
}

ofxSimpleGuiControl &ofxSimpleGuiControl::setFullColor(bool forceActive) {
	if(ofGetMousePressed() || forceActive) ofSetHexColor(config->fullActiveColor);
	else if(isMouseOver()) ofSetHexColor(config->fullOverColor);
	else ofSetHexColor(config->fullColor);
	return *this;
}

ofxSimpleGuiControl &ofxSimpleGuiControl::setEmptyColor() {
	ofSetColor(config->emptyColor.r, config->emptyColor.g, config->emptyColor.b, config->emptyColor.a);
	//		if(isMouseOver()) ofSetHexColor(config->overColor.r, config->overColor.g, config->overColor.b);
	//		if(focused && !isMouseOver()) ofSetHexColor(config->focusColor.r, config->focusColor.g, config->focusColor.b);
	return *this;
}

ofxSimpleGuiControl &ofxSimpleGuiControl::setKeyboardShortcut(char c) {
	keyboardShortcut = c;
	if(c) {
	//	printf("ofxSimpleGuiControl::setKeyboardShortcut %s %c\n", name.c_str(), c);
		name = origName + " (" + c + ")";
	} else {
		name = origName;
	}
	return *this;
}

void ofxSimpleGuiControl::roundedRect(float x, float y, float w, float h, float r) {
    ofBeginShape();
        ofVertex(x+r, y);
        ofVertex(x+w-r, y);
        quadraticBezierVertex(x+w, y, x+w, y+r, x+w-r, y);
        ofVertex(x+w, y+h-r);
        quadraticBezierVertex(x+w, y+h, x+w-r, y+h, x+w, y+h-r);
        ofVertex(x+r, y+h);
        quadraticBezierVertex(x, y+h, x, y+h-r, x+r, y+h);
        ofVertex(x, y+r);
        quadraticBezierVertex(x, y, x+r, y, x, y+r);
    ofEndShape();
}

void ofxSimpleGuiControl::quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY) {
  float cp1x = prevX + 2.0/3.0*(cpx - prevX);
  float cp1y = prevY + 2.0/3.0*(cpy - prevY);
  float cp2x = cp1x + (x - prevX)/3.0;
  float cp2y = cp1y + (y - prevY)/3.0;

  // finally call cubic Bezier curve function
  ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
};
