#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setSize(1280,800);
    settings.numSamples = 4;
    ofCreateWindow(settings);
	//ofSetLogLevel(OF_LOG_VERBOSE);

    //ofSetupOpenGL(1280,800, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());


}
