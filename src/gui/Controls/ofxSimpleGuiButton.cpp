
#include "ofxSimpleGuiButton.h"


ofxSimpleGuiButton::ofxSimpleGuiButton(ofxSimpleGuiConfig* config, string name, bool &value, bool bSquare) : ofxSimpleGuiValueControl<bool>(config, name, value) {
	beToggle	= false;
	beenPressed = false;
	isBGColorFixed = false;
	controlType = "Button";
    bForceSquare = bSquare;
	setup();
}

void ofxSimpleGuiButton::setup() {
	setSize(config->gridSize.x - config->padding.x, config->buttonHeight);
}

void ofxSimpleGuiButton::loadFromXML(ofxXmlSettings &XML) {
	setValue(XML.getValue(controlType + "_" + key + ":value", 0));
}

void ofxSimpleGuiButton::saveToXML(ofxXmlSettings &XML) {
	XML.addTag(controlType + "_" + key);
	XML.pushTag(controlType + "_" + key);
	XML.addValue("name", name);
	XML.addValue("value", getValue());
	XML.popTag();
}

void ofxSimpleGuiButton::keyPressed( int key ) {
	if(key==keyboardShortcut) toggle();
}



bool ofxSimpleGuiButton::getValue() {
	return (*value);
}

void ofxSimpleGuiButton::setValue(bool b) {
	(*value) = b;
}

void ofxSimpleGuiButton::toggle() {
	(*value) = !(*value);
}

void ofxSimpleGuiButton::setToggleMode(bool b) {
	beToggle = b;
}

void ofxSimpleGuiButton::onPress(int x, int y, int button) {
	beenPressed = true;
	if(beToggle) (*value) = !(*value);
	else (*value) = true;
}

void ofxSimpleGuiButton::onRelease(int x, int y, int button) {
	if(!beToggle) (*value) = false;
}

void ofxSimpleGuiButton::fixBGColorToActivated(bool isFixed) {
    isBGColorFixed = isFixed;
}

void ofxSimpleGuiButton::update()
{
    if(isBGColorFixed) {
        backgroundColor = ofColor::fromHex(config->textBGOverColor);
    }
    else {
        setTextBGColor();
        backgroundColor = ofGetStyle().color;
    }
}

void ofxSimpleGuiButton::draw(float x, float y) {
	setPosition(x, y);

	glPushMatrix();
	glTranslatef(x, y, 0);

	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(backgroundColor);
    if(config->rounded || !bForceSquare)
	{
        roundedRect(0,0,width,height,config->rectRadius);
	}
    else
    {
        ofDrawRectangle(0, 0, width, height);
    }



	// if a toggle
	if((*value) && beToggle) {
		setTextColor();
		//ofLine(0, 0, box.width, box.height);
		//ofLine(box.width, 0, 0, box.height);
	}

	setTextColor();

    if (config->bUseFont) {
        float offset = config->ttf.stringWidth(name);
        config->ttf.drawString(name, width/2-offset/2, 14);
    } else {
        ofDrawBitmapString(name, 4, 14);
    }

	ofDisableAlphaBlending();

	glPopMatrix();
}
