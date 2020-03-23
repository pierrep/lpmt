#pragma once

#include "ofxSimpleGuiControl.h"

class ofxSimpleGuiFPSCounter : public ofxSimpleGuiControl {
public:
	ofxSimpleGuiFPSCounter(ofxSimpleGuiConfig* config);
	void setup();
	void draw(float x, float y);
};
