
#include "ofxSimpleGuiToggle.h"


ofxSimpleGuiToggle::ofxSimpleGuiToggle(string name, bool &value) : ofxSimpleGuiControl(name) {
	this->value	= &value;
	setMomentary(false);
	controlType = "Toggle";
	setup();
}

ofxSimpleGuiToggle& ofxSimpleGuiToggle::setMomentary(bool m) {
	momentary = m;
	return *this;
}


void ofxSimpleGuiToggle::setup() {
	setSize(config->gridSize.x - config->padding.x, config->toggleHeight);
	bLearning = false;
	bLearnt = false;
}

void ofxSimpleGuiToggle::loadFromXML(ofxXmlSettings &XML) {
	setValue(XML.getValue(controlType + "_" + key + ":value", 0));
}

void ofxSimpleGuiToggle::saveToXML(ofxXmlSettings &XML) {
	XML.addTag(controlType + "_" + key);
	XML.pushTag(controlType + "_" + key);
	XML.addValue("name", name);
	XML.addValue("value", getValue());
	XML.popTag();
}


bool ofxSimpleGuiToggle::getValue() {
	return (*value);
}

void ofxSimpleGuiToggle::setValue(bool b) {
	(*value) = b;
}

void ofxSimpleGuiToggle::toggle() {
	(*value) = !(*value);
}

void ofxSimpleGuiToggle::onPress(int x, int y, int button) {
    if(button == 2)
	{
     bLearning = !bLearning;
     bLearnt = false;
	}
	else{
	if(momentary) setValue(true);
	else toggle();
	}
}

void ofxSimpleGuiToggle::onRelease(int x, int y, int button) {
	if(momentary) setValue(false);
}

void ofxSimpleGuiToggle::keyPressed( int key ) {
	if(key==keyboardShortcut) onPress(0, 0, 0);
}

void ofxSimpleGuiToggle::keyReleased( int key ) {
	if(key==keyboardShortcut) onRelease(0, 0, 0);
}

void ofxSimpleGuiToggle::onKeyEnter() {
	toggle();
}

void ofxSimpleGuiToggle::update() {

}

void ofxSimpleGuiToggle::draw(float x, float y) {
	setPosition(x, y);

	glPushMatrix();
	glTranslatef(x, y, 0);

	ofEnableAlphaBlending();
	ofFill();

	setTextBGColor();
	if(bLearning){ofSetColor(255,0,0);}
	else if(bLearnt){ofSetColor(0,255,0);}
    ofDrawRectangle(0, 0, width, height);

    setFullColor(*value);
    ofDrawRectangle(2, 2, height-4, height-4);

	if((*value)) {
        ofPushStyle();
		setTextColor();
        ofSetLineWidth(2);
        ofDrawLine(2, 2, height-4, height-4);
        ofDrawLine(height-4, 2, 2, height-4);
        ofPopStyle();
	}

	setTextColor();
    if (config->bUseFont) {
        config->ttf.drawString(name, height + 5, 12);
    } else {
        ofDrawBitmapString(name, height + 5, 12);
    }

	ofDisableAlphaBlending();

	glPopMatrix();
}

