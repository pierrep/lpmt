#pragma once

#include "ofxSimpleGuiControl.h"

#include "ofxSimpleGuiSliderBase.h"


class ofxSimpleGuiSliderInt : public ofxSimpleGuiSliderBase<int> {
	
public:
	ofxSimpleGuiSliderInt(ofxSimpleGuiConfig* config, string name, int &value, int min, int max) : ofxSimpleGuiSliderBase<int>(config, name, value, min, max) {
		controlType = "SliderInt";
		setIncrement(1);
	}

};
