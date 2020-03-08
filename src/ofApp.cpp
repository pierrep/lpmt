#include "ofApp.h"
#include "Util.hpp"
#include "stdio.h"
#include <iostream>

#include <errno.h>
#include <string>
#include <sys/types.h>
#include <vector>

//--------------------------------------------------------------
ofApp::ofApp()
    : m_gui(this)
{
}

//--------------------------------------------------------------
void ofApp::exit()
{
    timeline.reset();

    ofLogNotice() << "exiting...";
}

//--------------------------------------------------------------
void ofApp::setup()
{
    //ofDisableAntiAliasing();
    ofSetVerticalSync(true);
    ofSetWindowTitle("LPMT");
    ofSetEscapeQuitsApp(false);
    autoStart = false;

    // read xml config file
    bWasConfigLoadSuccessful = xmlConfigFile.loadFile("config.xml");
    if (!bWasConfigLoadSuccessful) {
        ofLogWarning() << "Config file \"config.xml\" not found!";
    } else {
        ofLogNotice() << "Loaded config file: \"config.xml\"";
    }

    // kinect
    setupKinect();

    // camera stuff
    m_cameras.clear();
    if (bWasConfigLoadSuccessful) {
        setupCameras();
    }

    // shared videos setup
    sharedVideos.clear();
    for (int i = 0; i < 8; i++) {
        ofVideoPlayer video;
        sharedVideos.push_back(video);
        sharedVideosFiles.push_back("");
    }

    //double click time
    m_doubleclickTime = 500; // 500 milliseconds
    m_timeLastClicked = 0;

    // rotation angle for surface-rotation visual feedback
    m_totalRotationAngle = 0;

    if ((ofGetScreenWidth() > 1024) && (ofGetScreenHeight() > 800)) {
        default_window_width = 1024;
        default_window_height = 768;
    } else {
        default_window_width = 800;
        default_window_height = 600;
    }

    // initialize the splash screen
    m_isSplashScreenActive = true;
    m_SplashScreenImage.load("lpmt_splash.png");

    maskSetup = false;
    gridSetup = false;

    // OSC setup
    setupOSC();

    // MIDI setup
    setupMidi();

    // initialize the load flags
    m_saveProjectFlag = false;
    m_loadProjectFlag = false;
    m_loadImageFlag = false;
    m_loadVideoFlag = false;
    m_loadSlideshowFlag = false;
    m_loadSharedVideo0Flag = false;
    m_loadSharedVideo1Flag = false;
    m_loadSharedVideo2Flag = false;
    m_loadSharedVideo3Flag = false;
    m_loadSharedVideo4Flag = false;
    m_loadSharedVideo5Flag = false;
    m_loadSharedVideo6Flag = false;
    m_loadSharedVideo7Flag = false;
    m_resetCurrentQuadFlag = false;
    m_bezierSpherizeQuadFlag = false;
    m_bezierSpherizeQuadStrongFlag = false;
    m_bezierResetQuadFlag = false;

    // setup shaders
    edgeBlendShader.load("shaders/blend.vert", "shaders/blend.frag");
    quadMaskShader.load("shaders/mask.vert", "shaders/mask.frag");
    //chromaShader.load("shaders/chroma.vert", "shaders/chroma.frag");
    surfaceShader.load("shaders/surface.vert", "shaders/surface.frag");
    crossfadeShader.load("shaders/crossfade.vert", "shaders/crossfade.frag");

    ttf.load("type/OpenSans-Regular.ttf", 11);

    isSetup = true; // starts in quads setup mode
    bStarted = true; // starts running
    bSnapOn = true; // snap mode for surfaces corner is on
    m_sourceQuadForCopying = -1; // number of surface to use as source in copy/paste (per default no quad is selected)
    m_isSnapshotTextureOn = false; // snapshot background texture is turned off by default
    //bMpe = false;  // default is not using MostPixelsEver
    bFullscreen = 0; // starts in windowed mode
    bGui = 1; // gui is on at start

    useTimeline = false;
    timelineDurationSeconds = timelinePreviousDuration = 100.0;

    // initializes layers array
    for (int i = 0; i < MAX_QUADS; i++) {
        layers[i] = -1;
    }

    setupInitialQuads();

    // timeline setup
    timelineSetup(timelineDurationSeconds);
    bTimeline = false;
    timeline.setCurrentPage(ofToString(activeQuad));
    timeline.hide();
    timeline.disable();
    timeline.setAutosave(false);

    // GUI stuff
    m_gui.setupPages();
    m_gui.updatePages(quads[activeQuad]);
    m_gui.showPage(2);

    if (bWasConfigLoadSuccessful) {
        float timelineConfigDuration = xmlConfigFile.getValue("TIMELINE:DURATION", 10);
        timelineDurationSeconds = timelinePreviousDuration = timelineConfigDuration;
        timeline.setDurationInSeconds(timelineDurationSeconds);
        if (xmlConfigFile.getValue("TIMELINE:AUTOSTART", 0)) {
            timeline.togglePlay(); // if timeline autostart is defined in timeline it starts timeline playing
        }
    }

    // autostart
    if (bWasConfigLoadSuccessful) {
        autoStart = xmlConfigFile.getValue("PROJECTION:AUTO", 0);
    }

    if (autoStart) {
        loadSettingsFromXMLFile("_lpmt_settings.xml"); //TODO <- ??
        m_gui.updatePages(quads[activeQuad]);

        toggleSetupMode();

        bFullscreen = true;
        ofSetFullscreen(true);
    }
}

//--------------------------------------------------------------
void ofApp::mpeSetup()
{
    //    stopProjection();
    //    bMpe = true;
    //    // MPE stuff
    //    lastFrameTime = ofGetElapsedTimef();
    //    client.setup("mpe_client_settings.xml", true); //false means you can use backthread
    //    ofxMPERegisterEvents(this);
    //    //resync();
    //    startProjection();
    //    client.start();
    //    ofSetBackgroundAuto(false);
}

//--------------------------------------------------------------
void ofApp::prepare()
{
    // check for waiting OSC messages
    while (receiver.hasWaitingMessages()) {
        parseOsc();
    }

    if (bStarted) {
        // updates shared video sources
        for (unsigned int i = 0; i < 8; i++) {
            if (sharedVideos[i].isLoaded()) {
                sharedVideos[i].update();
            }
        }

        //check if load project button in the GUI was pressed
        if (m_loadProjectFlag) {
            m_loadProjectFlag = false;
            loadProject();
        }

        //check if save project button in the GUI was pressed
        if (m_saveProjectFlag) {
            m_saveProjectFlag = false;
            saveProject();
        }

        // check if image load button in the GUI was pressed
        if (m_loadImageFlag) {
            m_loadImageFlag = false;
            openImageFile();
        }

        // check if video load button in the GUI was pressed
        if (m_loadVideoFlag) {
            m_loadVideoFlag = false;
            openVideoFile();
        }

        // check if shared video load buttons in the GUI were pressed
        if (m_loadSharedVideo0Flag) {
            m_loadSharedVideo0Flag = false;
            openSharedVideoFile(0);
        } else if (m_loadSharedVideo1Flag) {
            m_loadSharedVideo1Flag = false;
            openSharedVideoFile(1);
        } else if (m_loadSharedVideo2Flag) {
            m_loadSharedVideo2Flag = false;
            openSharedVideoFile(2);
        } else if (m_loadSharedVideo3Flag) {
            m_loadSharedVideo3Flag = false;
            openSharedVideoFile(3);
        } else if (m_loadSharedVideo4Flag) {
            m_loadSharedVideo4Flag = false;
            openSharedVideoFile(4);
        } else if (m_loadSharedVideo5Flag) {
            m_loadSharedVideo5Flag = false;
            openSharedVideoFile(5);
        } else if (m_loadSharedVideo6Flag) {
            m_loadSharedVideo6Flag = false;
            openSharedVideoFile(6);
        } else if (m_loadSharedVideo7Flag) {
            m_loadSharedVideo7Flag = false;
            openSharedVideoFile(7);
        }
        // check if slide show load button in the GUI was pressed
        if (m_loadSlideshowFlag) {
            m_loadSlideshowFlag = false;
            loadSlideshow();
        }

        //check if quad dimensions reset button in the GUI was pressed
        if (m_resetCurrentQuadFlag) {
            m_resetCurrentQuadFlag = false;
            quadDimensionsReset(activeQuad);
            quadPlacementReset(activeQuad);
        }

        //check if quad bezier spherize button in the GUI was pressed
        if (m_bezierSpherizeQuadFlag) {
            m_bezierSpherizeQuadFlag = false;
            quadBezierSpherize(activeQuad);
        }

        //check if quad bezier spherize strong button in the GUI was pressed
        if (m_bezierSpherizeQuadStrongFlag) {
            m_bezierSpherizeQuadStrongFlag = false;
            quadBezierSpherizeStrong(activeQuad);
        }

        //check if quad bezier reset button in the GUI was pressed
        if (m_bezierResetQuadFlag) {
            m_bezierResetQuadFlag = false;
            quadBezierReset(activeQuad);
        }

// check if kinect close button in the GUI was pressed
#ifdef WITH_KINECT
        if (bCloseKinect) {
            bCloseKinect = false;
            kinect.kinect.setCameraTiltAngle(0);
            kinect.kinect.close();
        }

        // check if kinect close button in the GUI was pressed
        if (bOpenKinect) {
            bOpenKinect = false;
            kinect.kinect.open();
        }
#endif

        for (unsigned int i = 0; i < m_cameras.size(); i++) {
            if (m_cameras[i].getHeight() > 0) // isLoaded check
            {
                m_cameras[i].update();
            }
        }

        // sets default window background, grey in setup mode and black in projection mode
        if (isSetup) {
            ofBackground(20, 20, 20);
        } else {
            ofBackground(0, 0, 0);
        }
//ofSetWindowShape(800, 600);

// kinect update
#ifdef WITH_KINECT
        if (m_isKinectInitialized) {
            kinect.update();
        }
#endif

//timeline update
        if (timelineDurationSeconds != timelinePreviousDuration) {
            timelinePreviousDuration = timelineDurationSeconds;
            timeline.setDurationInSeconds(timelineDurationSeconds);
        }
        if (useTimeline) {
            timelineUpdate();
        }

        // loops through initialized quads and runs update, setting the border color as well
        for (int i = 0; i < MAX_QUADS; i++) {
            int idx = layers[i];
            if (idx >= 0) {
                if (quads[idx].initialized) {
                    quads[idx].update();
                    // frame delay correction for Mpe sync
                    //                    if(bMpe)
                    //                    {
                    //                        if(quads[idx].videoBg && quads[idx].video.isLoaded())
                    //                        {
                    //                            int mpeFrame = client.getFrameCount();
                    //                            int totFrames = quads[idx].video.getTotalNumFrames();
                    //                            int videoFrame = quads[idx].video.getCurrentFrame();
                    //                            //quads[idx].video.setFrame(mpeFrame%totFrames);
                    //                            if(abs((mpeFrame%totFrames) - videoFrame) > 2) // TODO: testing different values
                    //                            {
                    //                                //cout << mpeFrame%totFrames << endl;
                    //                                quads[idx].video.setFrame(mpeFrame%totFrames);
                    //                            }
                    //                        }
                    //                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::render()
{
    if (bStarted) {
        // if snapshot is on draws it as window background
        if (isSetup && m_isSnapshotTextureOn) {
            ofEnableAlphaBlending();
            ofSetHexColor(0xFFFFFF);
            m_snapshotBackgroundTexture.draw(0, 0, ofGetWidth(), ofGetHeight());
            ofDisableAlphaBlending();
        }

        // loops through initialized quads and calls their draw function
        for (int i = 0; i < MAX_QUADS; i++) {
            int idx = layers[i];
            if (idx >= 0) {
                if (quads[idx].initialized) {
                    quads[idx].draw(sharedVideos);
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
    //if (!bMpe)
    {
        if (m_isSplashScreenActive) {
            // turn the splash screen image of after 3 seconds
            if (ofGetElapsedTimef() > 3.f) {
                m_isSplashScreenActive = !m_isSplashScreenActive;
            }
        }
        prepare();
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{

    //if (!bMpe)
    {
        render();
    }

    if (isSetup) {
        if (bStarted) {
            // if we are rotating surface, draws a feedback rotation sector
            ofEnableAlphaBlending();
            ofFill();
            ofSetColor(219, 104, 0, 255); // orange
            m_rotationSector.draw();
            ofNoFill();
            ofDisableAlphaBlending();

            // in setup mode writes the number of the active quad at the bottom of the window
            ofSetHexColor(0xFFFFFF); // white
            ttf.drawString("Active surface: " + ofToString(activeQuad) + " Layer: " + ofToString(quads[activeQuad].layer), 30, ofGetHeight() - 25);
            for (int i = 0; i < MAX_QUADS; i++) {
                int idx = layers[i];
                ttf.drawString("layers[" + ofToString(i) + "] = " + ofToString(layers[i]), 600, ofGetHeight() - 600 + i * 20);
            }

            if (maskSetup) {
                ofSetHexColor(0xFF0000);
                ttf.drawString("Mask-editing mode ", 170, ofGetHeight() - 25);
            }
            if (bMidiHotkeyCoupling) {
                if (bMidiHotkeyLearning) {
                    ofSetColor(255, 255, 0);
                    ttf.drawString("waiting for MIDI or OSC message ", 170, ofGetHeight() - 25);
                } else {
                    ofSetColor(255, 0, 0);
                    ttf.drawString("MIDI or OSC hotkey coupling ", 170, ofGetHeight() - 25);
                }
                ofDrawRectangle(2, 2, ofGetWidth() - 4, ofGetHeight() - 4);
            }
            // draws gui
            m_gui.draw();
        }
    }

    if (bTimeline) {
        timeline.draw();
    }

    // if the splash screen is active, draw the splash screen image
    if (m_isSplashScreenActive) {
        ofEnableAlphaBlending();
        ofSetHexColor(0xFFFFFF); // white
        m_SplashScreenImage.draw((ofGetWidth() / 2) - 230, (ofGetHeight() / 2) - 110);
        ofDisableAlphaBlending();
    }

    if (bGui) {
        ofSetColor(255, 255, 255);
        ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWidth() - 100, ofGetHeight() - 30);
    }

    ofSetWindowTitle("LPMT");
}

//--------------------------------------------------------------
//void ofApp::mpeFrameEvent(ofxMPEEventArgs& event)
//{
//    if (bMpe)
//    {
//        if(client.getFrameCount()<=1)
//        {
//            resync();
//        }
//        prepare();
//        render();
//    }
//}

//--------------------------------------------------------------
//void ofApp::mpeMessageEvent(ofxMPEEventArgs& event)
//{
//    //received a message from the server
//}

//void ofApp::mpeResetEvent(ofxMPEEventArgs& event)
//{
//    //triggered if the server goes down, another client goes offline, or a reset was manually triggered in the server code
//}

//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs& args)
{
    if (bMidiHotkeyCoupling) {
        bMidiHotkeyLearning = true;
        midiHotkeyPressed = args.key;
    } else {

        if (args.hasModifier(OF_KEY_CONTROL) && (args.keycode == 'Q')) {
            ofExit(1);
        } else if (args.key == '+' && !bTimeline && !bGui) {
            raiseLayer(); // moves active layer one position up
        } else if (args.key == '-' && !bTimeline && !bGui) {
            lowerLayer(); // moves active layer one position down
        } else if ((args.key == 's' || args.key == 'S') && !bTimeline) {
            saveProject(); // saves quads settings to an .xml project file in data directory
        } else if ((args.key == 'l') && !bTimeline) {
            loadProject(); // let the user choose an .xml project file with all the quads settings and loads it
        } else if (args.key == 'w' && !bTimeline) {
            if (m_cameras.size() > 0) // if cameras are connected, take a snapshot of the specified camera and uses it as window background
            {
                m_isSnapshotTextureOn = !m_isSnapshotTextureOn;
                if (m_isSnapshotTextureOn) {
                    const int width = m_snapshotBackgroundCamera->getWidth();
                    const int height = m_snapshotBackgroundCamera->getHeight();
                    m_snapshotBackgroundCamera->update();
                    m_snapshotBackgroundTexture.allocate(width, height, GL_RGB);
                    m_snapshotBackgroundTexture.loadData(m_snapshotBackgroundCamera->getPixels().getData(), width, height, GL_RGB);
                }
            } else {
                ofLogError() << "Can't take a snapshot background picture. No camera connected.";
            }
        } else if (args.key == 'W' && !bTimeline) {
            m_isSnapshotTextureOn = !m_isSnapshotTextureOn; // loads an image file and uses it as window background
            if (m_isSnapshotTextureOn) {
                ofImage image(loadImageFromFile());
                m_snapshotBackgroundTexture.allocate(image.getWidth(), image.getHeight(), GL_RGB);
                m_snapshotBackgroundTexture.loadData(image.getPixels().getData(), image.getWidth(), image.getHeight(), GL_RGB);
            }
        } else if ((args.key == 'q' || args.key == 'Q') && !bTimeline) {
            if (isSetup) {
                quads[activeQuad].corners[0] = ofPoint(0.0, 0.0); // fills window with active quad
                quads[activeQuad].corners[1] = ofPoint(1.0, 0.0);
                quads[activeQuad].corners[2] = ofPoint(1.0, 1.0);
                quads[activeQuad].corners[3] = ofPoint(0.0, 1.0);
            }
        } else if (args.key == '>' && !bTimeline) {
            activateNextQuad();
        } else if (args.key == '<' && !bTimeline) {
            activatePrevQuad();
        } else if ((args.hasModifier(OF_KEY_CONTROL) && (args.keycode == 'C')) && !bTimeline) //copy
        {
            m_sourceQuadForCopying = activeQuad; // make currently active quad the source quad for copying
        } else if ((args.hasModifier(OF_KEY_CONTROL) && (args.keycode == 'V')) && !bTimeline) // paste
        {
            copyQuadSettings(m_sourceQuadForCopying); // paste settings from source surface to currently active surface
        } else
            // goes to first page of gui for active quad or, in mask edit mode, delete last drawn point
            if ((args.key == OF_KEY_F2) && !bTimeline) {
            if (maskSetup && quads[activeQuad].m_maskPoints.size() > 0) {
                quads[activeQuad].m_maskPoints.pop_back();
            } else {
                m_gui.showPage(2);
            }
        } else if ((args.key == 'd' || args.key == 'D') && !bTimeline) {
            if (maskSetup && quads[activeQuad].m_maskPoints.size() > 0) {
                if (quads[activeQuad].bHighlightMaskPoint) {
                    quads[activeQuad].m_maskPoints.erase(quads[activeQuad].m_maskPoints.begin() + quads[activeQuad].highlightedMaskPoint);
                }
            }
        } else if ((args.key == OF_KEY_F3) && !bTimeline) // goes to second page of gui for active quad
        {
            m_gui.showPage(3);
        } else if ((args.key == 'c' || args.key == 'C') && !bTimeline) // goes to third page of gui for active quad or, in edit mask mode, clears mask
        {
            if (maskSetup) {
                quads[activeQuad].m_maskPoints.clear();
            }
        } else if (args.key == OF_KEY_F4) {
            m_gui.showPage(4);
        } else if (args.key == OF_KEY_F1 && !bTimeline) // show general settings page of gui
        {
            m_gui.showPage(1);
        } else if ((args.key == 'a' || args.key == 'A') && !bTimeline) // adds a new quad in the middle of the screen
        {
            addQuad();
        } else if ((args.hasModifier(OF_KEY_CONTROL) && (args.keycode == 'X')) && !bTimeline) {
            deleteQuad();
        } else if (args.key == ' ' && !bTimeline) // toggles setup mode
        {
            toggleSetupMode();
        } else if ((args.key == 'f' || args.key == 'F') && !bTimeline) // toggles fullscreen mode
        {
            bFullscreen = !bFullscreen;
            if (!bFullscreen) {
                ofSetWindowShape(default_window_width, default_window_height);
                ofSetFullscreen(false);
                int screenW = ofGetScreenWidth();
                int screenH = ofGetScreenHeight();
                ofSetWindowPosition(screenW / 2 - default_window_width / 2, screenH / 2 - default_window_height / 2);
            } else if (bFullscreen == 1) {
                ofSetFullscreen(true);
            }
        } else if ((args.key == 'g' || args.key == 'G') && !bTimeline) // toggles gui
        {
            if (maskSetup) {
                maskSetup = false;
                for (int i = 0; i < MAX_QUADS; i++) {
                    if (quads[i].initialized) {
                        quads[i].isMaskSetup = false;
                    }
                }
            }
            m_gui.toggleDraw();
            bGui = !bGui;
        } else if ((args.key == 'm' || args.key == 'M') && !bTimeline) // toggles mask editing
        {
            if (!bGui) {
                maskSetup = !maskSetup;
                for (int i = 0; i < MAX_QUADS; i++) {
                    if (quads[i].initialized) {
                        quads[i].isMaskSetup = !quads[i].isMaskSetup;
                    }
                }
            }
        } else if ((args.key == 'b' || args.key == 'B') && !bTimeline) // toggles bezier deformation editing
        {
            if (!bGui) {
                gridSetup = !gridSetup;
                for (int i = 0; i < MAX_QUADS; i++) {
                    if (quads[i].initialized) {
                        quads[i].isBezierSetup = !quads[i].isBezierSetup;
                    }
                }
            }
        } else if (args.key == '[' && !bTimeline) {
            m_gui.prevPage();
        } else if (args.key == ']' && !bTimeline) {
            m_gui.nextPage();
        } else if ((args.key == 'r' || args.key == 'R') && !bTimeline) // resyncs videos to start point in every quad
        {
            resync();
        } else if ((args.key == 'p' || args.key == 'P') && !bTimeline) // starts and stops rendering
        {
            startProjection();
        } else if ((args.key == 'o' || args.key == 'O') && !bTimeline) {
            stopProjection();
        } else if ((args.key == 'n' || args.key == 'N') && !bTimeline) {
            //   mpeSetup();
        } else if ((args.key == 'h' || args.key == 'H' || args.key == OF_KEY_F1) && !bTimeline) // displays help in system dialog
        {
            ofBuffer buf = ofBufferFromFile("help_keys.txt");
            ofSystemAlertDialog(buf);
        } else if (args.key == OF_KEY_F11 && bTimeline) // show-hide stage when timeline is shown
        {
            if (bStarted) {
                bStarted = false;
                for (int i = 0; i < MAX_QUADS; i++) {
                    if (quads[i].initialized) {
                        quads[i].isOn = false;
                        if (quads[i].videoBg && quads[i].video.isLoaded()) {
                            quads[i].video.setVolume(0);
                            quads[i].video.stop();
                        }
                    }
                }
            } else if (!bStarted) {
                bStarted = true;
                for (int i = 0; i < MAX_QUADS; i++) {
                    if (quads[i].initialized) {
                        quads[i].isOn = true;
                        if (quads[i].videoBg && quads[i].video.isLoaded()) {
                            quads[i].video.setVolume(quads[i].videoVolume);
                            quads[i].video.play();
                        }
                    }
                }
            }
        } else
// toggle timeline
            if (args.key == OF_KEY_F10) {
            bTimeline = !bTimeline;
            timeline.toggleShow();
            if (bTimeline) {
                timeline.enable();
                m_gui.hide();
                bGui = false;
            } else {
                bStarted = true;
                for (int i = 0; i < MAX_QUADS; i++) {
                    if (quads[i].initialized) {
                        quads[i].isOn = true;
                        if (quads[i].videoBg && quads[i].video.isLoaded()) {
                            quads[i].video.setVolume(quads[i].videoVolume);
                            quads[i].video.play();
                        }
                    }
                }
            }
        } else if (args.key == OF_KEY_F12) // toggle timeline playing
        {
            timeline.togglePlay();
        } else if (args.key == OF_KEY_F9 && bTimeline) // toggle timeline BPM grid drawing
        {
            timeline.toggleShowBPMGrid();
        } else

            if (args.key == '*' && !bTimeline) {
            if (m_cameras.size() > 0) {

                if (m_cameras[quads[activeQuad].camNumber].getPixelFormat() == OF_PIXELS_RGBA) {
                    m_cameras[quads[activeQuad].camNumber].setPixelFormat(OF_PIXELS_BGRA);
                } else if (m_cameras[quads[activeQuad].camNumber].getPixelFormat() == OF_PIXELS_BGRA) {
                    m_cameras[quads[activeQuad].camNumber].setPixelFormat(OF_PIXELS_RGBA);
                }
            }
        } else if (args.key == '#' && !bTimeline) // rotation of surface around its center
        {
            ofMatrix4x4 rotation;
            ofMatrix4x4 centerToOrigin;
            ofMatrix4x4 originToCenter;
            ofMatrix4x4 resultingMatrix;
            centerToOrigin.makeTranslationMatrix(-quads[activeQuad].center);
            originToCenter.makeTranslationMatrix(quads[activeQuad].center);
            rotation.makeRotationMatrix(-5.0, 0, 0, 1);
            resultingMatrix = centerToOrigin * rotation * originToCenter;
            for (int i = 0; i < 4; i++) {
                quads[activeQuad].corners[i] = quads[activeQuad].corners[i] * resultingMatrix;
            }
        } else if (args.key == '$' && !bTimeline) {
            ofMatrix4x4 rotation;
            ofMatrix4x4 centerToOrigin;
            ofMatrix4x4 originToCenter;
            ofMatrix4x4 resultingMatrix;
            centerToOrigin.makeTranslationMatrix(-quads[activeQuad].center);
            originToCenter.makeTranslationMatrix(quads[activeQuad].center);
            rotation.makeRotationMatrix(5.0, 0, 0, 1);
            resultingMatrix = centerToOrigin * rotation * originToCenter;
            for (int i = 0; i < 4; i++) {
                quads[activeQuad].corners[i] = quads[activeQuad].corners[i] * resultingMatrix;
            }
        }

    } //end keys

    if (args.key == OF_KEY_F5) {
        bMidiHotkeyCoupling = !bMidiHotkeyCoupling;
        bMidiHotkeyLearning = false;
        midiHotkeyPressed = -1;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

// The mouseMoved method checks if the mouse is over one of the active quads corners,
// its center or its rotation mark. It also checks if the mouse is over a mask, bezier or
// grid marker if the program is in that mode. If the mouse is over one of these markers,
// the marker get selected.
void ofApp::mouseMoved(int x, int y)
{
    const ofPoint mousePosition(x, y);

    if (isSetup && !bGui && !maskSetup && !gridSetup && !bTimeline) {
        float smallestDist = 1.0;
        m_selectedCorner = -1;
        const ofPoint normalizedMousePosition = Util::normalizePoint(mousePosition);

        for (int i = 0; i < 4; i++) {
            const ofPoint difference = quads[activeQuad].corners[i] - normalizedMousePosition;
            const float distance = std::sqrt(difference.x * difference.x + difference.y * difference.y);

            if (distance < smallestDist && distance < 0.05) // value for dist threshold can vary between 0.1-0.05, fine tune it
            {
                m_selectedCorner = i;
                smallestDist = distance;
            }
        }

        if (m_selectedCorner >= 0) {
            quads[activeQuad].bHighlightCorner = true;
            quads[activeQuad].highlightedCorner = m_selectedCorner;
        } else {
            quads[activeQuad].bHighlightCorner = false;
            quads[activeQuad].highlightedCorner = -1;

            // distance from center
            const ofPoint differenceFromCenter = quads[activeQuad].center - normalizedMousePosition;
            const float distanceFromCenter = std::sqrt(differenceFromCenter.x * differenceFromCenter.x + differenceFromCenter.y * differenceFromCenter.y);
            if (distanceFromCenter < 0.05) {
                quads[activeQuad].bHighlightCenter = true;
            } else {
                quads[activeQuad].bHighlightCenter = false;
            }

            // distance from rotation grab point
            ofPoint rotationGrabPoint = quads[activeQuad].center;
            rotationGrabPoint.x += 0.1;
            const ofPoint rotationDifference = rotationGrabPoint - normalizedMousePosition;
            const float rotationDistance = std::sqrt(rotationDifference.x * rotationDifference.x + rotationDifference.y * rotationDifference.y);
            if (rotationDistance < 0.05) {
                quads[activeQuad].bHighlightRotation = true;
            } else {
                quads[activeQuad].bHighlightRotation = false;
            }
        }
    }

    else if (maskSetup && !gridSetup && !bTimeline) {
        float smallestDist = std::sqrt(ofGetWidth() * ofGetWidth() + ofGetHeight() * ofGetHeight());
        int whichPoint = -1;
        const ofPoint warpedMousePosition = quads[activeQuad].getWarpedPoint(mousePosition);

        for (int i = 0; i < quads[activeQuad].m_maskPoints.size(); i++) {
            const ofPoint maskPointInPixel = Util::scalePointToPixel(quads[activeQuad].m_maskPoints[i]);
            const ofPoint difference = maskPointInPixel - warpedMousePosition;
            const float distance = std::sqrt(difference.x * difference.x + difference.y * difference.y);

            if (distance < smallestDist && distance < 20.0) {
                whichPoint = i;
                smallestDist = distance;
            }
        }

        if (whichPoint >= 0) {
            quads[activeQuad].bHighlightMaskPoint = true;
            quads[activeQuad].highlightedMaskPoint = whichPoint;
        } else {
            quads[activeQuad].bHighlightMaskPoint = false;
            quads[activeQuad].highlightedMaskPoint = -1;
        }
    }

    else if (gridSetup && !maskSetup && !bTimeline) {
        float smallestDist = std::sqrt(ofGetWidth() * ofGetWidth() + ofGetHeight() * ofGetHeight());
        int whichPointRow = -1;
        int whichPointCol = -1;
        const ofPoint warpedMousePosition = quads[activeQuad].getWarpedPoint(mousePosition);

        if (quads[activeQuad].bBezier) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    const ofPoint bezierPointInPixel = Util::scalePointToPixel(ofPoint(quads[activeQuad].bezierPoints[i][j][0], quads[activeQuad].bezierPoints[i][j][1]));
                    const ofPoint difference = bezierPointInPixel - warpedMousePosition;
                    float distance = std::sqrt(difference.x * difference.x + difference.y * difference.y);

                    if (distance < smallestDist && distance < 20.0) {
                        whichPointRow = i;
                        whichPointCol = j;
                        smallestDist = distance;
                    }
                }
            }
        }

        else if (quads[activeQuad].bGrid) {
            for (int i = 0; i <= quads[activeQuad].gridRows; i++) {
                for (int j = 0; j <= quads[activeQuad].gridColumns; j++) {
                    const ofPoint gridPointInPixel = Util::scalePointToPixel(ofPoint(quads[activeQuad].gridPoints[i][j][0], quads[activeQuad].gridPoints[i][j][1]));
                    const ofPoint difference = gridPointInPixel - warpedMousePosition;
                    const float distance = std::sqrt(difference.x * difference.x + difference.y * difference.y);

                    if (distance < smallestDist && distance < 20.0) {
                        whichPointRow = i;
                        whichPointCol = j;
                        smallestDist = distance;
                    }
                }
            }
        }

        if (whichPointRow >= 0) {
            quads[activeQuad].bHighlightCtrlPoint = true;
            quads[activeQuad].highlightedCtrlPointRow = whichPointRow;
            quads[activeQuad].highlightedCtrlPointCol = whichPointCol;
        } else {
            quads[activeQuad].bHighlightCtrlPoint = false;
            quads[activeQuad].highlightedCtrlPointRow = -1;
            quads[activeQuad].highlightedCtrlPointCol = -1;
        }
    }
}

// The mouseDragged method moves or rotates the quad according to the currently
// selected markers (corners, center, rotation, mask, grid or bezier marker).
// The markers are selected in the mouseMoved method.
void ofApp::mouseDragged(int x, int y, int button)
{
    const ofPoint mousePosition(x, y);
    const ofPoint normalizedMouseMovement = Util::normalizePoint(mousePosition - m_lastMousePosition);

    // quad movement code
    if (isSetup && !bGui && !maskSetup && !gridSetup && !bTimeline) {
        // check if one of the corners is selected
        if (m_selectedCorner >= 0) {
            // move the selected corner
            quads[activeQuad].corners[m_selectedCorner] += normalizedMouseMovement;

            // scale the whole quad if shift is pressed and one of the corners is moved
            if (ofGetKeyPressed(OF_KEY_LEFT_SHIFT)) {
                // get the previous and the next corner
                int previousCorner = m_selectedCorner - 1;
                int nextCorner = m_selectedCorner + 1;

                if (previousCorner == -1)
                    previousCorner = 3;
                if (nextCorner == 4)
                    nextCorner = 0;

                // move the previous and next quad accordingly so the quad gets scaled
                if (m_selectedCorner == 0 || m_selectedCorner == 2) {
                    quads[activeQuad].corners[previousCorner].x += normalizedMouseMovement.x;
                    quads[activeQuad].corners[nextCorner].y += normalizedMouseMovement.y;
                } else {
                    quads[activeQuad].corners[previousCorner].y += normalizedMouseMovement.y;
                    quads[activeQuad].corners[nextCorner].x += normalizedMouseMovement.x;
                }
            }
        } else {
            // if no corner is selected, check if we can move or rotate whole quad
            // by dragging its center and rotation mark
            if (quads[activeQuad].bHighlightCenter) // TODO: verifiy if threshold value is good for distance
            {
                // move the entire quad
                for (int i = 0; i < 4; i++) {
                    quads[activeQuad].corners[i] += normalizedMouseMovement;
                }
            }
            // rotate the quad
            else if (quads[activeQuad].bHighlightRotation) {
                // compute the angle active quads center and the last two mouse positions
                const ofPoint centerInPixel = Util::scalePointToPixel(quads[activeQuad].center);
                const ofPoint vec1 = (m_lastMousePosition - centerInPixel);
                const ofPoint vec2 = (mousePosition - centerInPixel);
                const float deltaAngle = ofRadToDeg(std::atan2(vec2.y, vec2.x) - std::atan2(vec1.y, vec1.x));

                // add that angle to the total rotation
                m_totalRotationAngle += deltaAngle;
                m_rotationSector.clear();
                m_rotationSector.addVertex(centerInPixel);
                m_rotationSector.lineTo(centerInPixel.x + (0.025 * ofGetWidth()), centerInPixel.y);
                m_rotationSector.arc(centerInPixel, 0.025 * ofGetWidth(), 0.025 * ofGetWidth(), 0, m_totalRotationAngle, 40);
                m_rotationSector.close();

                // compute a matrices for the following steps: 1. move quads center to (0, 0)
                // 2. rotate quad 3. move quads center back to the original position
                // combine these matrices and apply them to the active quad
                const ofMatrix4x4 centerToOriginMatrix = ofMatrix4x4::newTranslationMatrix(-quads[activeQuad].center);
                const ofMatrix4x4 rotationMatrix = ofMatrix4x4::newRotationMatrix(deltaAngle, 0, 0, 1);
                const ofMatrix4x4 originToCenterMatrix = ofMatrix4x4::newTranslationMatrix(quads[activeQuad].center);
                const ofMatrix4x4 resultingMatrix = centerToOriginMatrix * rotationMatrix * originToCenterMatrix;
                for (int i = 0; i < 4; i++) {
                    quads[activeQuad].corners[i] = quads[activeQuad].corners[i] * resultingMatrix;
                }
            }
        }
    } else if (maskSetup && quads[activeQuad].bHighlightMaskPoint && !bTimeline) {
        // in mask setup mode, move the selected mask point
        const ofPoint warpedPoint = quads[activeQuad].getWarpedPoint(mousePosition);
        const ofPoint normalizedWarpedPoint = Util::normalizePoint(warpedPoint);

        quads[activeQuad].m_maskPoints[quads[activeQuad].highlightedMaskPoint] = normalizedWarpedPoint;
    }

    else if (gridSetup && quads[activeQuad].bHighlightCtrlPoint && !bTimeline) {
        const int currentRow = quads[activeQuad].highlightedCtrlPointRow;
        const int currentCol = quads[activeQuad].highlightedCtrlPointCol;
        const ofPoint warpedPoint = quads[activeQuad].getWarpedPoint(mousePosition);
        const ofPoint normalizedWarpedPoint = Util::normalizePoint(warpedPoint);

        if (quads[activeQuad].bBezier) {
            quads[activeQuad].bezierPoints[currentRow][currentCol][0] = normalizedWarpedPoint.x;
            quads[activeQuad].bezierPoints[currentRow][currentCol][1] = normalizedWarpedPoint.y;
        } else if (quads[activeQuad].bGrid) {
            quads[activeQuad].gridPoints[currentRow][currentCol][0] = normalizedWarpedPoint.x;
            quads[activeQuad].gridPoints[currentRow][currentCol][1] = normalizedWarpedPoint.y;
        }
    }

    // save the last mouse position for calculating the delta movement
    m_lastMousePosition = mousePosition;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    const ofPoint mousePosition(x, y);

    m_rotationSector.clear();
    // this is used for dragging the whole quad or one of it's corners
    m_lastMousePosition = mousePosition;

    // deactivate the splash screen on click
    if (m_isSplashScreenActive) {
        m_isSplashScreenActive = !m_isSplashScreenActive;
    }

    if (isSetup && !bGui && !bTimeline) {

        if (maskSetup && !gridSetup) {
            // if we are in mask setup mode and no mask point is selected, add a new mask point
            if (!quads[activeQuad].bHighlightMaskPoint) {
                quads[activeQuad].maskAddPoint(mousePosition);
            }
        }

        else {
            // check if the user double-clicked on a different quad and make it the active one
            unsigned long now = ofGetElapsedTimeMillis();
            if ((now - m_timeLastClicked) < m_doubleclickTime) {
                activateClosestQuad(mousePosition);
            }
            m_timeLastClicked = now;
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    m_totalRotationAngle = 0;
    m_rotationSector.clear();
    if (bSnapOn && isSetup && !bGui && !bTimeline) {
        if (m_selectedCorner >= 0) {
            // snap detection for near quads
            float smallestDist = 1.0;
            int snapQuad = -1;
            int snapCorner = -1;
            for (int i = 0; i < MAX_QUADS; i++) {
                if (i != activeQuad && quads[i].initialized) {
                    for (int j = 0; j < 4; j++) {
                        const ofPoint difference = quads[activeQuad].corners[m_selectedCorner] - quads[i].corners[j];
                        const float distance = std::sqrt(difference.x * difference.x + difference.y * difference.y);
                        // to tune snapping change distance value inside next if statement
                        if (distance < smallestDist && distance < 0.0075) {
                            snapQuad = i;
                            snapCorner = j;
                            smallestDist = distance;
                        }
                    }
                }
            }
            if (snapQuad >= 0 && snapCorner >= 0) {
                quads[activeQuad].corners[m_selectedCorner] = quads[snapQuad].corners[snapCorner];
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    timeline.setWidth(w);

    float x_ratio = ((float)w / ofGetScreenWidth());
    float y_ratio = ((float)h / ofGetScreenHeight());

    for (int i = 0; i < MAX_QUADS; i++) {
        if (quads[i].initialized) {
            //calculates screen ratio factor for window and fullscreen
            quads[i].screenFactorX = x_ratio;
            quads[i].screenFactorY = y_ratio;
            quads[i].bHighlightCorner = false;
            quads[i].allocateFbo(w, h);
            quadDimensionsReset(i);
        }
    }
}

//---------------------------------------------------------------
void ofApp::quadDimensionsReset(int q)
{
    quads[q].quadW = ofGetWidth();
    quads[q].quadH = ofGetHeight();
}

//---------------------------------------------------------------
void ofApp::quadPlacementReset(int q)
{
    quads[q].quadDispX = 0;
    quads[q].quadDispY = 0;
}

//---------------------------------------------------------------
void ofApp::quadBezierSpherize(int q)
{
    float w = (float)ofGetWidth();
    float h = (float)ofGetHeight();
    float k = (sqrt(2) - 1) * 4 / 3;
    quads[q].bBezier = true;

    float tmp_bezierPoints[4][4][3] = {
        { { 0.0f * h / w + (0.5f * (w / h - 1)) * h / w, 0, 0 }, { 0.5f * k * h / w + (0.5f * (w / h - 1)) * h / w, -0.5f * k, 0 }, { (1.0f * h / w) - (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, -0.5f * k, 0 }, { 1.0f * h / w + (0.5f * (w / h - 1)) * h / w, 0, 0 } },
        { { 0.0f * h / w - (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, 0.5f * k, 0 }, { 0 * h / w + (0.5f * (w / h - 1)) * h / w, 0, 0 }, { 1.0f * h / w + (0.5f * (w / h - 1)) * h / w, 0, 0 }, { 1.0f * h / w + (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, 0.5f * k, 0 } },
        { { 0.0f * h / w - (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, 1.0f - 0.5f * k, 0 }, { 0 * h / w + (0.5f * (w / h - 1)) * h / w, 1.0f, 0 }, { 1.0f * h / w + (0.5f * (w / h - 1)) * h / w, 1.0f, 0 }, { 1.0f * h / w + (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, 1.0f - 0.5f * k, 0 } },
        { { 0.0f * h / w + (0.5f * (w / h - 1)) * h / w, 1.0f, 0 }, { 0.5f * k * h / w + (0.5f * (w / h - 1)) * h / w, 1.0f + 0.5f * k, 0 }, { (1.0f * h / w) - (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, 1.0f + 0.5f * k, 0 }, { 1.0f * h / w + (0.5f * (w / h - 1)) * h / w, 1.0f, 0 } }
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 3; ++k) {
                quads[q].bezierPoints[i][j][k] = tmp_bezierPoints[i][j][k];
            }
        }
    }
    /*  quads[q].bezierPoints =
    {
        {   {(0.5f*w/h-0.5f)*h/w, 0, 0},  {0.5f*(k+w/h-1)*h/w, -0.5f*k, 0},    {0.5f*(1-k+w/h)*h/w, -0.5f*k, 0},    {1.0*h/w+(0.5f*(w/h-1))*h/w, 0, 0}    },
        {   {0*h/w-(0.5f*k*h/w)+(0.5f*(w/h-1))*h/w, 0.5f*k, 0},        {0*h/w+(0.5f*(w/h-1))*h/w, 0, 0},  {1.0*h/w+(0.5f*(w/h-1))*h/w, 0, 0},  {1.0*h/w+(0.5f*k*h/w)+(0.5f*(w/h-1))*h/w, 0.5f*k, 0}  },
        {   {0*h/w-(0.5f*k*h/w)+(0.5f*(w/h-1))*h/w, 1.0-0.5f*k, 0},        {0*h/w+(0.5f*(w/h-1))*h/w, 1.0, 0},  {1.0*h/w+(0.5f*(w/h-1))*h/w, 1.0, 0},  {1.0*h/w+(0.5f*k*h/w)+(0.5f*(w/h-1))*h/w, 1.0-0.5f*k, 0}  },
        {   {0*h/w+(0.5f*(w/h-1))*h/w, 1.0, 0},        {0.5f*k*h/w+(0.5f*(w/h-1))*h/w, 1.0+0.5f*k, 0},  {(1.0*h/w)-(0.5f*k*h/w)+(0.5f*(w/h-1))*h/w, 1.0+0.5f*k, 0},  {1.0*h/w+(0.5f*(w/h-1))*h/w, 1.0, 0}  }
    }; */
}

//---------------------------------------------------------------
void ofApp::quadBezierSpherizeStrong(int q)
{
    float w = (float)ofGetWidth();
    float h = (float)ofGetHeight();
    float k = (sqrt(2) - 1) * 4 / 3;
    quads[q].bBezier = true;

    float tmp_bezierPoints[4][4][3] = {
        { { 0 * h / w + (0.5f * (w / h - 1)) * h / w, 0, 0 }, { 0.5f * k * h / w + (0.5f * (w / h - 1)) * h / w, -0.5f * k, 0 }, { (1.0f * h / w) - (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, -0.5f * k, 0 }, { 1.0f * h / w + (0.5f * (w / h - 1)) * h / w, 0, 0 } },
        { { 0 * h / w - (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, 0.5f * k, 0 }, { 0 * h / w - (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, -0.5f * k, 0 }, { 1.0f * h / w + (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, -0.5f * k, 0 }, { 1.0f * h / w + (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, 0.5f * k, 0 } },
        { { 0 * h / w - (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, 1.0f - 0.5f * k, 0 }, { 0 * h / w - (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, 1.0f + 0.5f * k, 0 }, { 1.0f * h / w + (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, 1.0f + 0.5f * k, 0 }, { 1.0f * h / w + (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, 1.0f - 0.5f * k, 0 } },
        { { 0 * h / w + (0.5f * (w / h - 1)) * h / w, 1.0f, 0 }, { 0.5f * k * h / w + (0.5f * (w / h - 1)) * h / w, 1.0f + 0.5f * k, 0 }, { (1.0f * h / w) - (0.5f * k * h / w) + (0.5f * (w / h - 1)) * h / w, 1.0f + 0.5f * k, 0 }, { 1.0f * h / w + (0.5f * (w / h - 1)) * h / w, 1.0f, 0 } }
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 3; ++k) {
                quads[q].bezierPoints[i][j][k] = tmp_bezierPoints[i][j][k];
            }
        }
    }
}

//---------------------------------------------------------------
void ofApp::quadBezierReset(int q)
{
    quads[q].bBezier = true;
    float tmp_bezierPoints[4][4][3] = {
        { { 0, 0, 0 }, { 0.333, 0, 0 }, { 0.667, 0, 0 }, { 1.0, 0, 0 } },
        { { 0, 0.333, 0 }, { 0.333, 0.333, 0 }, { 0.667, 0.333, 0 }, { 1.0, 0.333, 0 } },
        { { 0, 0.667, 0 }, { 0.333, 0.667, 0 }, { 0.667, 0.667, 0 }, { 1.0, 0.667, 0 } },
        { { 0, 1.0, 0 }, { 0.333, 1.0, 0 }, { 0.667, 1.0, 0 }, { 1.0, 1.0, 0 } }
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 3; ++k) {
                quads[q].bezierPoints[i][j][k] = tmp_bezierPoints[i][j][k];
            }
        }
    }
}

//---------------------------------------------------------------
// This method activates the topmost quad under the mouse
void ofApp::activateClosestQuad(ofPoint mouse)
{
    // go through the layers from top to bottom
    for (int j = 35; j >= 0; j--) {
        int i = layers[j];
        if (i >= 0) {
            if (quads[i].initialized) {
                const ofPoint vertex0 = Util::scalePointToPixel(quads[i].corners[0]);
                const ofPoint vertex1 = Util::scalePointToPixel(quads[i].corners[1]);
                const ofPoint vertex2 = Util::scalePointToPixel(quads[i].corners[2]);
                const ofPoint vertex3 = Util::scalePointToPixel(quads[i].corners[3]);

                if ((Util::pointInTriangle2D(vertex0, vertex1, vertex2, mouse)
                        || Util::pointInTriangle2D(vertex2, vertex3, vertex0, mouse))
                    && (i != activeQuad)) {
                    quads[activeQuad].isActive = false;
                    activeQuad = i;
                    quads[activeQuad].isActive = true;
                    m_gui.updatePages(quads[activeQuad]);
                    break;
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::raiseLayer()
{
    int position;
    int target;

    for (int i = 0; i < 35; i++) {
        if (layers[i] == quads[activeQuad].quadNumber) {
            position = i;
            target = i + 1;
        }
    }
    if (layers[target] != -1) {
        int target_content = layers[target];
        layers[target] = quads[activeQuad].quadNumber;
        layers[position] = target_content;
        quads[activeQuad].layer = target;
        quads[target_content].layer = position;
    }
}

//--------------------------------------------------------------
void ofApp::lowerLayer()
{
    int position;
    int target;

    for (int i = 0; i < MAX_QUADS; i++) {
        if (layers[i] == quads[activeQuad].quadNumber) {
            position = i;
            target = i - 1;
        }
    }
    if (target >= 0) {
        if (layers[target] != -1) {
            int target_content = layers[target];
            layers[target] = quads[activeQuad].quadNumber;
            layers[position] = target_content;
            quads[activeQuad].layer = target;
            quads[target_content].layer = position;
        }
    }
}

//--------------------------------------------------------------
void ofApp::toggleSetupMode()
{

    if (isSetup) {
        isSetup = false;
        m_gui.hide();
        ofHideCursor();
        bGui = false;
        for (int i = 0; i < MAX_QUADS; i++) {
            if (quads[i].initialized) {
                quads[i].isSetup = false;
            }
        }
    } else {
        isSetup = true;
        m_gui.show();
        ofShowCursor();
        bGui = true;
        for (int i = 0; i < MAX_QUADS; i++) {
            if (quads[i].initialized) {
                quads[i].isSetup = true;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::setupInitialQuads()
{
    // defines the first 4 default quads
    quads[0].setup(ofPoint(0.0, 0.0), ofPoint(0.5, 0.0), ofPoint(0.5, 0.5), ofPoint(0.0, 0.5), edgeBlendShader, quadMaskShader, surfaceShader, crossfadeShader, m_cameras, ttf);
    quads[0].quadNumber = 0;

    quads[1].setup(ofPoint(0.5, 0.0), ofPoint(1.0, 0.0), ofPoint(1.0, 0.5), ofPoint(0.5, 0.5), edgeBlendShader, quadMaskShader, surfaceShader, crossfadeShader, m_cameras, ttf);
    quads[1].quadNumber = 1;

    quads[2].setup(ofPoint(0.0, 0.5), ofPoint(0.5, 0.5), ofPoint(0.5, 1.0), ofPoint(0.0, 1.0), edgeBlendShader, quadMaskShader, surfaceShader, crossfadeShader, m_cameras, ttf);
    quads[2].quadNumber = 2;

    quads[3].setup(ofPoint(0.5, 0.5), ofPoint(1.0, 0.5), ofPoint(1.0, 1.0), ofPoint(0.5, 1.0), edgeBlendShader, quadMaskShader, surfaceShader, crossfadeShader, m_cameras, ttf);
    quads[3].quadNumber = 3;

    // define last one as active quad
    activeQuad = 3;
    quads[activeQuad].isActive = true;

    // number of total quads, to be modified later at each quad insertion
    nOfQuads = 4;

    //layers
    layers[0] = 0;
    quads[0].layer = 0;
    layers[1] = 1;
    quads[1].layer = 1;
    layers[2] = 2;
    quads[2].layer = 2;
    layers[3] = 3;
    quads[3].layer = 3;
}

//--------------------------------------------------------------
void ofApp::setupCameras()
{
    xmlConfigFile.pushTag("cameras");
    // check how many cameras are defined in settings
    int numberOfCameras = 0;
    numberOfCameras = xmlConfigFile.getNumTags("camera");
    //m_cameras.reserve(numberOfCameras); // reserve memory, so the iterators don't get invalidated by reallocation

    // cycle through defined cameras trying to initialize them and populate the cameras vector
    for (int i = 0; i < numberOfCameras; i++) {
        // read the requested parameters for the camera
        xmlConfigFile.pushTag("camera", i);
        const int requestedCameraWidth = xmlConfigFile.getValue("requestedWidth", 640);
        const int requestedCameraHeight = xmlConfigFile.getValue("requestedHeight", 480);
        const int cameraID = xmlConfigFile.getValue("id", 0);
        const int useForSnapshotBackground = xmlConfigFile.getValue("useForSnapshotBackround", 0);
        xmlConfigFile.popTag();

        // try initialize a video grabber
        ofVideoGrabber camera;
        vector<ofVideoDevice> devices = camera.listDevices();

        for (unsigned int i = 0; i < devices.size(); i++) {
            ofLogNotice("LPMT") << "Camera Found - camera ID: " << devices[i].id << " Name: " << devices[i].deviceName << " Hardware Name:" << devices[i].hardwareName;
        }

        bool isVideoGrabberInitialized = false;
        int actualCameraWidth = 0;
        int actualCameraHeight = 0;

        for (unsigned int i = 0; i < devices.size(); i++) {

            if (devices[i].bAvailable) {
                if (devices[i].id == cameraID) {
                    camera.setDeviceID(cameraID);

                    isVideoGrabberInitialized = camera.initGrabber(requestedCameraWidth, requestedCameraHeight);
                    actualCameraWidth = static_cast<int>(camera.getWidth());
                    actualCameraHeight = static_cast<int>(camera.getHeight());

                    // inform the user that the requested camera dimensions and the actual ones might differ
                    ofLogNotice("LPMT") << "Camera with ID " << cameraID << " asked for dimensions " << requestedCameraWidth << "x" << requestedCameraHeight << " - actual size is " << actualCameraWidth << "x" << actualCameraHeight;
                }
            }
        }

        // check if the camera is available and eventually push it to cameras vector
        if (!isVideoGrabberInitialized || actualCameraWidth == 0 || actualCameraHeight == 0) {
            ofLogWarning("LPMT") << "Camera with ID " << cameraID << " was requested, but was not found or is not available.";
        } else {
            m_cameras.push_back(camera);
            // following vector is used for the combo box in SimpleGuiToo gui
            m_cameraIds.push_back(ofToString(cameraID));

            // if this camera is the first one or it is marked for being used
            // as the background snapshot camera, save a pointer to it
            if (useForSnapshotBackground == 1 || cameraID == 0) {
                m_snapshotBackgroundCamera = m_cameras.end() - 1;
                m_snapshotBackgroundTexture.allocate(camera.getWidth(), camera.getHeight(), GL_RGB);
            }
        }
    }
    xmlConfigFile.popTag();
}

//--------------------------------------------------------------
void ofApp::setupOSC()
{
    int oscReceivePort = OSC_DEFAULT_PORT;
    if (bWasConfigLoadSuccessful) {
        oscReceivePort = xmlConfigFile.getValue("OSC:LISTENING_PORT", OSC_DEFAULT_PORT);
        appId = xmlConfigFile.getValue("OSC:APPID", 0);
    }
    ofLogNotice() << "Listening for OSC messages on port: " << oscReceivePort;
    receiver.setup(oscReceivePort);

    current_msg_string = 0;
    oscControlMin = xmlConfigFile.getValue("OSC:GUI_CONTROL:SLIDER:MIN", 0.0f);
    oscControlMax = xmlConfigFile.getValue("OSC:GUI_CONTROL:SLIDER:MAX", 1.0f);
    ofLogNotice() << "osc control of gui sliders range: min=" << oscControlMin << " - max=" << oscControlMax;

    oscHotkeyMessages.clear();
    oscHotkeyKeys.clear();
}

//--------------------------------------------------------------
void ofApp::setupMidi()
{
#ifdef WITH_MIDI
    // print input ports to console
    midiIn.listPorts();
    // open port by number
    //midiIn.openPort(1);
    //midiIn.openPort("IAC Pure Data In");	// by name
    midiIn.openVirtualPort("LPMT Input"); // open a virtual port

    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midiIn.ignoreTypes(false, false, false);
    // add ofApp as a listener
    midiIn.addListener(this);
    // print received messages to the console
    midiIn.setVerbose(true);
    //clear vectors used for midi-hotkeys coupling
    midiHotkeyMessages.clear();
    midiHotkeyKeys.clear();
#endif

    bMidiHotkeyCoupling = false;
    bMidiHotkeyLearning = false;
    midiHotkeyPressed = -1;
}

//--------------------------------------------------------------
void ofApp::setupSyphon()
{
#ifdef WITH_SYPHON
    // Syphon setup
    syphClient.setup();
    //syphClient.setApplicationName("Simple Server");
    syphClient.setServerName("");
#endif
}

//--------------------------------------------------------------
void ofApp::setupKinect()
{
#ifdef WITH_KINECT
    m_isKinectInitialized = kinect.setup();
    bCloseKinect = false;
    bOpenKinect = false;

    for (int i = 0; i < MAX_QUADS; i++) {
        quads[i].setKinect(&kinect);
    }

#endif
}

//--------------------------------------------------------------
// let the user choose an .xml project file with all the quads settings and loads it
void ofApp::loadProject()
{
    ofFileDialogResult dialogResult = ofSystemLoadDialog("Load project file (.xml)");

    if (dialogResult.bSuccess) {
        loadSettingsFromXMLFile(dialogResult.getPath());
        m_gui.updatePages(quads[activeQuad]);
        m_gui.showPage(2);
    }
}

//--------------------------------------------------------------
// saves quads settings to an .xml project file in data directory
void ofApp::saveProject()
{
    ofFileDialogResult dialog_result = ofSystemSaveDialog("lpmt_settings.xml", "Save project file (.xml)");

    if (dialog_result.bSuccess) {
        saveCurrentSettingsToXMLFile(dialog_result.getPath());
    }
}
