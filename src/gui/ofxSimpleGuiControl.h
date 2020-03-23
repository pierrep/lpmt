/***********************************************************************

 Copyright (c) 2008, 2009, 2010, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.

 based on Todd Vanderlin's ofxSimpleGui API
 http://toddvanderlin.com/

 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/

#pragma once

#include "ofxMSAInteractiveObject.h"
#include "ofxMidi.h"
#include "ofxOsc.h"
#include "ofxSimpleGuiConfig.h"
#include "ofxXmlSettings.h"

class ofxSimpleGuiControl : public ofxMSAInteractiveObject {
public:
    string name; // Human readable name this is what is displayed on screen (includes keyboard shortcut)
    string origName; // the original name (excluding keyboard shortcut)
    string key; // Machine readable name (don't change this after creating control, used for saving/loading)
    string controlType;
    bool lock;
    bool focused;
    bool newColumn;
    bool hasTitle;
    bool bLearning;
    bool bLearnt;
    char keyboardShortcut;

    ofxMidiMessage midiControl;
    ofxOscMessage oscControl;
    vector<ofxMidiMessage> midiSlideControls;

    ofxSimpleGuiControl(ofxSimpleGuiConfig* config, string name);
    ofxSimpleGuiControl& setName(string newName);
    ofxSimpleGuiControl& setKey(string newKey);
    ofxSimpleGuiControl& setConfig(ofxSimpleGuiConfig* config);
    ofxSimpleGuiControl& setNewColumn(bool b);

    ofxSimpleGuiControl& setTextColor(bool clickable = true);
    ofxSimpleGuiControl& setTextBGColor(bool clickable = true);
    ofxSimpleGuiControl& setFullColor(bool forceActive = false);
    ofxSimpleGuiControl& setEmptyColor();
    ofxSimpleGuiControl& setKeyboardShortcut(char c);

    virtual void loadFromXML(ofxXmlSettings& XML) {}
    virtual void saveToXML(ofxXmlSettings& XML) {}

    virtual void setup() {}

    virtual void draw(float x, float y) {}
    virtual void draw() { draw(x, y); }

    virtual void onKeyUp() {} // up key is pressed
    virtual void onKeyDown() {} // down key is pressed
    virtual void onKeyLeft() {} // left key is pressed
    virtual void onKeyRight() {} // right key is pressed
    virtual void onKeyEnter() {} // enter key is pressed

    void roundedRect(float x, float y, float w, float h, float r);
    void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY);

    // from ofxMSAInteractiveObject
    virtual void onRollOver(int x, int y) {} // called when mouse enters object x, y, width, height
    virtual void onRollOut() {} // called when mouse leaves object x, y, width, height
    virtual void onMouseMove(int x, int y) {} // called when mouse moves while over object x, y, width, height
    virtual void onDragOver(int x, int y, int button) {} // called when mouse moves while over object and button is down
    virtual void onDragOutside(int x, int y, int button) {} // called when mouse moves while outside the object after being clicked on it
    virtual void onPress(int x, int y, int button) {} // called when mouse presses while over object
    virtual void onPressOutside(int x, int y, int button) {} // called when mouse presses while outside object
    virtual void onRelease(int x, int y, int button) {} // called when mouse releases while over object
    virtual void onReleaseOutside(int x, int y, int button) {} // called when mouse releases outside of object after being pressed on object

    virtual void keyPressed(int key) {}
    virtual void keyReleased(int key) {}

protected:
    ofxSimpleGuiConfig* config;
};
