
#include "ofxSimpleGuiTitle.h"

ofxSimpleGuiTitle::ofxSimpleGuiTitle(ofxSimpleGuiConfig* config, string name, float height, bool bSquare)
    : ofxSimpleGuiControl(config, name)
{
    beToggle = false;
    beenPressed = false;
    //		this->value	= &value;
    value = NULL;
    controlType = "Title";
    bForceSquare = bSquare;

    if (height == 0)
        height = config->titleHeight;
    if (hasTitle == false)
        height /= 2;
    setSize(config->gridSize.x - config->padding.x, height);
    setup();
}

void ofxSimpleGuiTitle::setup()
{
}

void ofxSimpleGuiTitle::loadFromXML(ofxXmlSettings& XML)
{
    if (!value)
        return;
    setValue(XML.getValue(controlType + "_" + key + ":value", 0));
}

void ofxSimpleGuiTitle::saveToXML(ofxXmlSettings& XML)
{
    if (!value)
        return;
    XML.addTag(controlType + "_" + key);
    XML.pushTag(controlType + "_" + key);
    XML.addValue("name", name);
    XML.addValue("value", getValue());
    XML.popTag();
}

bool ofxSimpleGuiTitle::getValue()
{
    if (!value)
        return false;
    return (*value);
}
void ofxSimpleGuiTitle::setValue(bool b)
{
    if (!value)
        return;
    (*value) = b;
}
void ofxSimpleGuiTitle::toggle()
{
    if (!value)
        return;
    (*value) = !(*value);
}

void ofxSimpleGuiTitle::setToggleMode(bool b)
{
    if (!value)
        return;
    beToggle = b;
}

void ofxSimpleGuiTitle::onPress(int x, int y, int button)
{
    if (!value)
        return;
    beenPressed = true;
    if (beToggle)
        (*value) = !(*value);
    else
        (*value) = true;
}

void ofxSimpleGuiTitle::onRelease(int x, int y, int button)
{
    if (!value)
        return;
    if (!beToggle)
        (*value) = false;
}

void ofxSimpleGuiTitle::draw(float x, float y)
{
    setPosition(x, y);

    if (hasTitle == false)
        return;

    glPushMatrix();
    glTranslatef(x, y, 0);

    ofEnableAlphaBlending();
    ofFill();

    ofSetHexColor(config->fullActiveColor);
//    if (config->rounded && !bForceSquare) {
//        roundedRect(0, 0, width, height, config->rectRadius);
//    } else
    {
        ofDrawRectangle(0, 0, width, height);
    }

    // if a toggle
    if (value && (*value) && beToggle) {
        setTextColor();
        //ofLine(0, 0, box.width, box.height);
        //ofLine(box.width, 0, 0, box.height);
    }

    setTextColor(value != NULL);
    if (config->bUseFont) {
        config->ttf.drawString(name, 3, 15);
    } else {
        ofDrawBitmapString(name, 3, 15);
    }

    ofDisableAlphaBlending();

    glPopMatrix();
}
