/**
 * ofxTimeline
 * openFrameworks graphical timeline addon
 *
 * Copyright (c) 2011-2012 James George
 * Development Supported by YCAM InterLab http://interlab.ycam.jp/en/
 * http://jamesgeorge.org + http://flightphase.com
 * http://github.com/obviousjim + http://github.com/flightphase
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once

#include "ofMain.h"

//----------------------------------------
static bool ofGetModifierAltPressed(ofKeyEventArgs& args) {
	if (args.hasModifier(OF_KEY_LEFT_ALT) || args.hasModifier(OF_KEY_RIGHT_ALT))
		return true;

	return false;
}

//----------------------------------------
static bool ofGetModifierAltPressed(ofMouseEventArgs& args) {
	if (args.hasModifier(OF_KEY_LEFT_ALT) || args.hasModifier(OF_KEY_RIGHT_ALT))
		return true;

	return false;
}

//----------------------------------------
static bool ofGetModifierShiftPressed(ofKeyEventArgs& args) {
	if (args.hasModifier(OF_KEY_LEFT_SHIFT) || args.hasModifier(OF_KEY_RIGHT_SHIFT))
		return true;

	return false;
}

//----------------------------------------
static bool ofGetModifierShiftPressed(ofMouseEventArgs& args) {
	if (args.hasModifier(OF_KEY_LEFT_SHIFT) || args.hasModifier(OF_KEY_RIGHT_SHIFT))
		return true;
	
	return false;
}

//----------------------------------------
static bool ofGetModifierControlPressed(ofKeyEventArgs& args) {
	if (args.hasModifier(OF_KEY_CONTROL))
		return true;

	return false;
}

//----------------------------------------
static bool ofGetModifierControlPressed(ofMouseEventArgs& args) {
	if (args.hasModifier(OF_KEY_CONTROL))
		return true;

	return false;
}

//----------------------------------------
static bool ofGetModifierSpecialPressed(ofKeyEventArgs& args) {
	if (args.hasModifier(OF_KEY_COMMAND))
		return true;

	return false;
}

//----------------------------------------
static bool ofGetModifierSpecialPressed(ofMouseEventArgs& args) {
	if (args.hasModifier(OF_KEY_COMMAND))
		return true;

	return false;
}

//convention for elements to be selected
#ifdef OF_TARGET_OSX
static bool ofGetModifierSelection(ofMouseEventArgs& args) {
	return ofGetModifierShiftPressed(args) || ofGetModifierSpecialPressed(args);
}
#else
static bool ofGetModifierSelection(ofMouseEventArgs& args) {
	return ofGetModifierShiftPressed(args);
}
#endif


#ifdef OF_TARGET_OSX
static bool ofGetModifierShortcutKeyPressed(ofKeyEventArgs& args) {
#ifdef MAC_USE_CONTROL
	//		cout << "using command" << endl;
	return ofGetModifierControlPressed(args);
#else
	//		cout << "using control" << endl;
	return ofGetModifierSpecialPressed(args);
#endif
}
#else
static bool ofGetModifierShortcutKeyPressed(ofKeyEventArgs& args) {
	return ofGetModifierControlPressed(args);
}
#endif

