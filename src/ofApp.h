#pragma once

#define MAX_QUADS 36
#define MAX_SHARED_VIDEOS 8

//#define WITH_KINECT
#define WITH_MIDI
#ifdef TARGET_OSX
  //  #define WITH_SYPHON
#endif

// OSC stuff - listen on port 12345
#define OSC_DEFAULT_PORT 12345
#define NUM_MSG_STRINGS 20

#include "ofMain.h"
#include "quad.h"
#include "GUI.hpp"
#include "ofxOsc.h"
#include "ofxTimeline.h"
#include "ofxXmlSettings.h"
#include "ofxSimpleGuiToo.h"
//#include "ofxMostPixelsEver.h"

#ifdef WITH_KINECT
#include "kinectManager.h"
#endif

#ifdef WITH_SYPHON
#include "ofxSyphon.h"
#endif

#ifdef WITH_MIDI
#include "ofxMidi.h"
#endif


#ifdef WITH_MIDI
class ofApp : public ofBaseApp, public ofxMidiListener
#else
class ofApp : public ofBaseApp
#endif
{

public:

    ofApp();
    void exit();
    void setup();
    void update();
    void draw();
    void keyPressed(ofKeyEventArgs& args);
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);

    void prepare();
    void render();
    void setupInitialQuads();
    void setupCameras();
    void setupOSC();
    void setupMidi();
    void setupSyphon();
    void setupKinect();
    void mpeSetup();
    void resync();
    void startProjection();
    void stopProjection();

    void addQuad();
    void deleteQuad();
    void copyQuadSettings(int sourceQuad);
    void activateNextQuad();
    void activatePrevQuad();
    void quadDimensionsReset(int q);
    void quadPlacementReset(int q);
    void quadBezierSpherize(int q);
    void quadBezierSpherizeStrong(int q);
    void quadBezierReset(int q);

    void openImageFile();
    void openVideoFile();
    void loadSlideshow();
    void openSharedVideoFile(int i);
    void openSharedVideoFile(string path, int i);

    void loadProject();
    void saveProject();
    void raiseLayer();
    void lowerLayer();
    void toggleEditMode();
    void saveCurrentSettingsToXMLFile(std::string xmlFilePath);
    void loadSettingsFromXMLFile(std::string xmlFilePath);
    ofImage loadImageFromFile(); // snapshot loading
    void activateClosestQuad(ofPoint point);
    void parseOsc();

    int default_window_width;
    int default_window_height;

    ofTrueTypeFont ttf;
    GUI m_gui;
    ofxXmlSettings xmlConfigFile;

    quad quads[MAX_QUADS];
    int layers[MAX_QUADS];

    // Surfaces
    int activeQuad;
    int nOfQuads;
    int m_sourceQuadForCopying;    
    int m_selectedCorner;

    bool autoStart;
    bool isEditMode;
    bool bWasConfigLoadSuccessful;
    bool bFullscreen;
    bool bGui;
    bool bTimeline;
    bool bStarted;
    bool maskSetup;
    bool gridSetup;
    bool bSnapOn;

    // splash screen variables
    bool m_isSplashScreenActive;
    ofImage m_SplashScreenImage;

    // OSC stuff
    int appId;
    ofxOscReceiver receiver;
    int	current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    float oscControlMin;
    float oscControlMax;
    vector<ofxOscMessage> oscHotkeyMessages;
    vector<int> oscHotkeyKeys;

    // Shaders
    ofShader edgeBlendShader;
    ofShader quadMaskShader;
    //ofShader chromaShader;
    ofShader surfaceShader;
    ofShader crossfadeShader;

    // gui elements
    bool showGui;

    // gui button flags
    bool m_loadProjectFlag;
    bool m_saveProjectFlag;
    bool m_loadImageFlag;
    bool m_loadVideoFlag;
    bool m_loadSlideshowFlag;
    bool m_loadSharedVideo0Flag;
    bool m_loadSharedVideo1Flag;
    bool m_loadSharedVideo2Flag;
    bool m_loadSharedVideo3Flag;
    bool m_loadSharedVideo4Flag;
    bool m_loadSharedVideo5Flag;
    bool m_loadSharedVideo6Flag;
    bool m_loadSharedVideo7Flag;
    bool m_resetCurrentQuadFlag;
    bool m_bezierSpherizeQuadFlag;
    bool m_bezierSpherizeQuadStrongFlag;
    bool m_bezierResetQuadFlag;
    bool bMidiHotkeyCoupling;
    bool bMidiHotkeyLearning;
    int midiHotkeyPressed;

    float m_totalRotationAngle;
    ofPolyline m_rotationSector;

    // snapshot background variables
    bool m_isSnapshotTextureOn;
    ofTexture m_snapshotBackgroundTexture;
	std::vector<ofVideoGrabber> m_cameras;
	std::vector<ofVideoGrabber>::iterator m_snapshotBackgroundCamera;
	std::vector<string> m_cameraIds;

    vector<ofVideoPlayer> sharedVideos;
    vector<string> sharedVideosFiles;

    vector<string> imgFiles;
    vector<string> videoFiles;
    vector<string> slideshowFolders;

    // double-click variables for quad picking
    unsigned long m_doubleclickTime;
    unsigned long long m_timeLastClicked;

    ofPoint m_lastMousePosition;

    // timeline
    ofxTimeline timeline;
    int timelineDurationSeconds;
    float timelinePreviousDuration;
    void timelineSetup(float duration);
    void timelineUpdate();
    void timelineAddQuadPage(int i);
    void timelineRemoveQuadPage(int i);
    void timelineTriggerReceived(ofxTLBangEventArgs& trigger);
    bool useTimeline;

    #ifdef WITH_KINECT
    kinectManager kinect;
    bool m_isKinectInitialized;
    bool bCloseKinect;
    bool bOpenKinect;
    #endif

    // syphon
    #ifdef WITH_SYPHON
	ofxSyphonClient syphClient;
    #endif

    // MIDI stuff
    #ifdef WITH_MIDI
    void newMidiMessage(ofxMidiMessage& eventArgs);
    ofxMidiIn midiIn;
    ofxMidiMessage midiMessage;
    vector<ofxMidiMessage> midiHotkeyMessages;
    vector<int> midiHotkeyKeys;
    #endif

    // MostPixelsEver stuff
    /*
    bool bMpe;
    ofxMPEClient client;
    void mpeFrameEvent(ofxMPEEventArgs& event);
    void mpeMessageEvent(ofxMPEEventArgs& event);
    void mpeResetEvent(ofxMPEEventArgs& event);
    float lastFrameTime;
    */

};

