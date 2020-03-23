#pragma once

#include "ofxSimpleGuiControl.h"

#include "ofxSimpleGuiSliderBase.h"


class ofxSimpleGuiSliderFloat : public ofxSimpleGuiSliderBase<float> {
	
public:
	ofxSimpleGuiSliderFloat(ofxSimpleGuiConfig* config, string name, float &value, float min, float max) : ofxSimpleGuiSliderBase<float>(config, name, value, min, max) {
		controlType = "SliderFloat";
	}
};
