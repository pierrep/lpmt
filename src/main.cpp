#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setSize(1280,800);
    settings.numSamples = 4;
	settings.windowMode = OF_WINDOW;
	settings.setPosition(glm::vec2(100, 100));
    settings.title = "LPMT";
    ofCreateWindow(settings);
	
    //ofSetLogLevel(OF_LOG_VERBOSE);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());


}
