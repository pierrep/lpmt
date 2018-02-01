#pragma once

#include "ofxSimpleGuiValueControl.h"
#include <types/ofColor.h>


class ofxSimpleGuiButton : public ofxSimpleGuiValueControl<bool> {
public:
	bool			beToggle;
	bool			beenPressed;

	ofxSimpleGuiButton(string name, bool &value);
	void setup();

	void loadFromXML(ofxXmlSettings &XML);
	void saveToXML(ofxXmlSettings &XML);
	void keyPressed( int key );

	bool getValue();
	void setValue(bool b);
	void toggle();
	void fixBGColorToActivated(bool isFixed);  // set the background color to active permanently (used for the header tabs)

	void setToggleMode(bool b);

	void onPress(int x, int y, int button);
	void onRelease(int x, int y, int button);
	void update();
	void draw(float x, float y);

private:
    ofColor backgroundColor;
    bool isBGColorFixed;
};
