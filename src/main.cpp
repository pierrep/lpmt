#include "ofMain.h"
#include "ofApp.h"

//#define SET_CUSTOM_LINUX_ICON

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

    ofApp *app = new ofApp();

    #ifdef SET_CUSTOM_LINUX_ICON
    //Need to make setWindowIcon public in libs/openFrameworks/app/ofAppGLFWWindow.h to work
    ofAppGLFWWindow* win;
    win = dynamic_cast<ofAppGLFWWindow *> (ofGetWindowPtr());
    win->setWindowIcon("assets/icon_64x64.png");
    #endif

    ofRunApp(app);
}
