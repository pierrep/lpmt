
#include "ofxSimpleGuiContent.h"


ofxSimpleGuiContent::ofxSimpleGuiContent(ofxSimpleGuiConfig* config,string name, ofBaseDraws& content, float fixwidth) : ofxSimpleGuiControl(config, name) {
	this->content = &content;
	this->fixwidth  = fixwidth;
	controlType = "Content";
	setup();
}

void ofxSimpleGuiContent::setup() {
	fixheight = fixwidth * content->getHeight()/content->getWidth();
	setSize(fixwidth, fixheight + config->sliderTextHeight);
}

void ofxSimpleGuiContent::draw(float x, float y) {
	if(content->getWidth() == 0 && content->getHeight() ==0) return;

	setPosition(x, y);
	setup();

	glPushMatrix();
	glTranslatef(x, y, 0);
	ofEnableAlphaBlending();
	ofFill();
	glColor4f(0, 0, 0, 0.8f);
    ofDrawRectangle(0, 0, width, fixheight);

	ofSetColor(255,255,255);
	content->draw(0, 0, width, fixheight);

	ofFill();
	setTextBGColor();
    ofDrawRectangle(0, fixheight, width, config->sliderTextHeight);

	setTextColor();
	ofDrawBitmapString(name, 3, fixheight + 15);
	ofDisableAlphaBlending();
	glPopMatrix();
}
