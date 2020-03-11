#pragma once

#include "ofxSimpleGuiControl.h"
#include "ofxTextInputField.h"

template <typename Type>
class ofxSimpleGuiSliderBase : public ofxSimpleGuiControl {
public:
    Type* value;
    Type min, max;

    float barwidth;
    float pct;

    float lerpSpeed;
    Type targetValue;
    Type oldValue;
    Type increment;

    ofxTextInputField textField;
    bool enterText;

    //--------------------------------------------------------------------- construct
    ofxSimpleGuiSliderBase(string name, Type& value, Type min, Type max)
        : ofxSimpleGuiControl(name)
    {
        this->value = &value;
        this->min = min;
        this->max = max;

        targetValue = value;
        oldValue = targetValue;
        controlType = "SliderBase";

        setIncrement(0);
        setSmoothing(0);

        setup();
    }

    void setup()
    {
        setSize(config->gridSize.x - config->padding.x, config->sliderHeight + config->sliderTextHeight);
        pct = ofMap((*value), min, max, 0.0, width);
        barwidth = pct;
        enterText = false;
        textField.disable();
        textField.text = ofToString((*value));
        bLearning = false;
        bLearnt = false;
        midiSlideControls.clear();
    }

    void loadFromXML(ofxXmlSettings& XML)
    {
        setValue((Type)XML.getValue(controlType + "_" + key + ":value", 0.0f));
    }

    void setSmoothing(float smoothing)
    {
        lerpSpeed = 1.0f - smoothing * 0.9; // so smoothing :1 still results in some motion!
    }

    void setIncrement(Type increment)
    {
        this->increment = increment;
    }

    void saveToXML(ofxXmlSettings& XML)
    {
        XML.addTag(controlType + "_" + key);
        XML.pushTag(controlType + "_" + key);
        XML.addValue("name", name);
        XML.addValue("value", getValue());
        XML.popTag();
    }

    Type getValue()
    {
        return (*value);
    }

    void setValue(Type f)
    {
        setTargetValue(f);
        oldValue = *value = targetValue;
    }

    void setTargetValue(Type f)
    {
        targetValue = ofClamp(f, min, max);
    }

    void increase()
    {
        if (increment == 0)
            setIncrement((max - min) * 0.001);
        //		oldValue = *value;		// save oldValue (so the draw doesn't update target but uses it)
        setTargetValue(*value + increment);
    }

    void decrease()
    {
        if (increment == 0)
            setIncrement((max - min) * 0.001);
        //		oldValue = *value;		// save oldValue (so the draw doesn't update target but uses it)
        setTargetValue(*value - increment);
    }

    void updateSlider()
    {
        if (!enabled)
            return;

        if (pct > width) {
            pct = width;
        } else {
            pct = getMouseX() - x;
            float temp = ofMap(pct, 0.0, width, min, max, true);

            targetValue = (Type)temp;
            oldValue = *value; // save oldValue (so the draw doesn't update target but uses it)
            textField.text = ofToString((*value));
        }
    }

    void onPress(int x, int y, int button)
    {
        if (button == 2) {
            bLearning = !bLearning;
            bLearnt = false;
            midiSlideControls.clear();
        } else {
            updateSlider();
        }

        if (enterText) {
            enterText = false;
            setValue((Type)ofToFloat(textField.text));
            textField.endEditing();
            textField.disable();
            textField.cursorPosition = 0;
        }
    }

    void onDoublePress(int x, int y, int button)
    {
        enterText = true;
        textField.cursorPosition = 0;
        textField.text = ofToString((*value));
        //textField.enable();
        textField.beginEditing();
    }

    void onDragOver(int x, int y, int button)
    {
        updateSlider();
    }

    void onDragOutside(int x, int y, int button)
    {
        updateSlider();
    }

    void onKeyRight()
    {
        increase();
    }

    void onKeyLeft()
    {
        decrease();
    }

    void onKeyUp()
    {
        increase();
    }

    void onKeyDown()
    {
        decrease();
    }

    void onKeyEnter()
    {
        if (enterText) {
            enterText = false;
            setValue((Type)ofToFloat(textField.text));
            textField.endEditing();
            textField.disable();
            textField.cursorPosition = 0;
        } else {
            enterText = true;
            textField.cursorPosition = 0;
            textField.text = ofToString((*value));
            textField.enable();
            textField.beginEditing();
        }
    }

    //--------------------------------------------------------------------- update
    void update()
    {
        if (!enabled)
            return;

        if (oldValue != *value) { // if value has changed programmatically by something else
            oldValue = targetValue = *value; // save the value in target and oldvalue
        } else { // otherwise lerp
            *value += (Type)((targetValue - *value) * lerpSpeed);
            oldValue = *value; // and save oldvalue
        }

        if (lock) {
            updateSlider();
        }
    }

    //--------------------------------------------------------------------- draw
    void draw(float x, float y)
    {
        setPosition(x, y);

        // Value Clamp
        barwidth = ofMap((*value), min, max, 0.0, (float)width);
        if (barwidth > width)
            barwidth = width;
        else if (barwidth < 0)
            barwidth = 0;

        ofEnableAlphaBlending();
        glPushMatrix();
        glTranslatef(x, y, 0);
        ofFill();

        setEmptyColor();
        ofDrawRectangle(0, config->sliderHeight, width, config->sliderHeight);

        setFullColor();
        ofDrawRectangle(0, config->sliderHeight, barwidth, config->sliderHeight);

        setTextBGColor();
        if (bLearning) {
            if (midiSlideControls.size() == 1) {
                ofSetColor(255, 255, 0);
            } else {
                ofSetColor(255, 0, 0);
            }
        } else if (bLearnt) {
            ofSetColor(0, 255, 0);
        }
        if (config->rounded) {
            roundedRect(0, 0, width, config->sliderTextHeight, config->rectRadius);
        } else {
            ofDrawRectangle(0, 0, width, config->sliderTextHeight);
        }
        setTextColor();
        if (enterText) {
            string s = name + ":";
            if (config->bUseFont) {
                config->ttf.drawString(s, 3, 14);
            } else {
                ofDrawBitmapString(s, 3, 14);
            }
            textField.bounds.x = width - config->ttf.stringWidth(textField.text)-30;
            textField.bounds.y = config->sliderHeight + 3;
            textField.draw();
        } else {
            string s = name;
            if (config->bUseFont) {
                config->ttf.drawString(s, 3, 14);
                s = ofToString((*value));
                config->ttf.drawString(s, 3, config->sliderHeight + 14);
            } else {
                ofDrawBitmapString(s, 3, 12);
            }
        }
        ofDisableAlphaBlending();
        glPopMatrix();
    }
};
