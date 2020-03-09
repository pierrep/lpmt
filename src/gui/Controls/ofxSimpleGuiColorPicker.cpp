/*
 *  ofxSimpleGuiColorPicker.cpp
 *  OpenCL Particles 3. single segment trails
 *
 *  Created by Mehmet Akten on 01/11/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSimpleGuiColorPicker.h"

ofxSimpleGuiColorPicker::ofxSimpleGuiColorPicker(string name, float* value, float max)
    : ofxSimpleGuiControl(name)
{
    this->value = value;
    this->min = 0;
    this->max = max;

    controlType = "ColorPicker";
    setup();
}

void ofxSimpleGuiColorPicker::setup()
{
    setSize(config->gridSize.x - config->padding.x, config->colorSliderHeight * 4 + config->sliderTextHeight);
    for (int i = 0; i < 4; i++) {
        pct[i] = ofMap(getValue(i), 0, max, 0.0, width);
        barwidth[i] = pct[i];
    }
}

void ofxSimpleGuiColorPicker::loadFromXML(ofxXmlSettings& XML)
{
    for (int i = 0; i < 4; i++) {
        setValue(XML.getValue(controlType + "_" + key + ":values_" + ofToString(i), 0.0f), i);
    }
}

void ofxSimpleGuiColorPicker::saveToXML(ofxXmlSettings& XML)
{
    XML.addTag(controlType + "_" + key);
    XML.pushTag(controlType + "_" + key);
    XML.addValue("name", name);
    for (int i = 0; i < 4; i++) {
        XML.addValue("values_" + ofToString(i), getValue(i));
    }
    XML.popTag();
}

float ofxSimpleGuiColorPicker::getValue(int i)
{
    return (value)[i];
}

void ofxSimpleGuiColorPicker::setValue(float f, int i)
{
    if (f < min)
        f = min;
    else if (f > max)
        f = max;
    (value)[i] = f;
}

void ofxSimpleGuiColorPicker::updateSlider()
{
    if (!enabled)
        return;

    int i = (getMouseY() - y) / config->colorSliderHeight;
    if (i < 0 || i >= 4)
        return;

    if (pct[i] > width) {
        pct[i] = width;
    } else {
        pct[i] = getMouseX() - x;
        setValue(ofMap(pct[i], 0.0, (float)width, 0, max), i);
    }
}

void ofxSimpleGuiColorPicker::onPress(int x, int y, int button)
{
    updateSlider();
}

void ofxSimpleGuiColorPicker::onDragOver(int x, int y, int button)
{
    updateSlider();
}

void ofxSimpleGuiColorPicker::onDragOutside(int x, int y, int button)
{
    updateSlider();
}

//--------------------------------------------------------------------- update
void ofxSimpleGuiColorPicker::update()
{
    if (!enabled)
        return;

    if (lock) {
        updateSlider();
    }

    //	enabled = false;
}

//--------------------------------------------------------------------- draw
void ofxSimpleGuiColorPicker::draw(float x, float y)
{

    //	enabled = true;

    //update postion of gui object
    setPosition(x, y);
    glPushMatrix();
    glTranslatef(x, y, 0);

    int startY = 0;
    for (int i = 0; i < 4; i++) {

        barwidth[i] = ofMap(getValue(i), 0, max, 0.0, (float)width);
        if (barwidth[i] > width)
            barwidth[i] = width;
        else if (barwidth[i] < 0)
            barwidth[i] = 0;

        ofEnableAlphaBlending();
        ofFill();
        setEmptyColor();
        ofDrawRectangle(0, startY, width, config->colorSliderHeight - 2); // 2 pixel margin

        // set slider color
        switch (i) {
        case 0:
            glColor3f(1, 0, 0);
            break;
        case 1:
            glColor3f(0, 1, 0);
            break;
        case 2:
            glColor3f(0, 0, 1);
            break;
        case 3:
            glColor3f(getValue(i), getValue(i), getValue(i));
            break;
        }

        // determine whether the mouse is within the element and over what slider
        int overBar = -1;
        if ((getMouseY() - y) >= 0) {
            overBar = (getMouseY() - y) / config->colorSliderHeight;
        }
        const bool withinElement = getMouseX() >= this->x && getMouseX() <= this->x + width;

        ofFloatColor valueStringColor(0.5, 0.5, 0.5);
        float sliderHeight;
        sliderHeight = config->colorSliderHeight - 2; // 2 pixel margin

        // change the color and height of the slider if the mouse is over
        if (overBar == i && withinElement) {

            switch (i) {
            case 1:
                // make the green slider text color black for better readability
                valueStringColor = ofFloatColor(0.f, 0.f, 0.f);
                break;
            case 3: {
                // make the alpha sliders text color the inverse of its value for a better readability
                const float inverseValue = 1.f - getValue(i);
                valueStringColor = ofFloatColor(inverseValue, inverseValue, inverseValue);
                break;
            }
            default:
                // red and blue slider text becomes white on mouse over
                valueStringColor = ofFloatColor(1.f, 1.f, 1.f);
                break;
            }
        }

        // draw slider
        ofDrawRectangle(0, startY, barwidth[i], sliderHeight);

        // draw value string
        glColor3f(valueStringColor.r, valueStringColor.g, valueStringColor.b);

        if (config->bUseFont) {
            config->ttf.drawString(ofToString(getValue(i), 4), 3, startY + 12);
        } else {
            ofDrawBitmapString(ofToString(getValue(i), 4), 3, startY + 12);
        }

        startY += config->colorSliderHeight;
    }

    ofFill();

    setTextBGColor();
    ofDrawRectangle(0, startY, width, config->sliderTextHeight);

    glColor3f(getValue(0), getValue(1), getValue(2));
    ofDrawRectangle(150, startY + 3, width - 150 - 3, config->sliderTextHeight - 8);

    setTextColor();
    string s = name;

    if (config->bUseFont) {
        config->ttf.drawString(name, 3, startY + 14);
    } else {
        ofDrawBitmapString(name, 3, startY + 14);
    }

    ofDisableAlphaBlending();
    glPopMatrix();
}
