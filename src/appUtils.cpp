#include "ofApp.h"

//-----------------------------------------------------------
void ofApp::openImageFile()
{
    ofLogNotice() << "Open Image file dialogue";
    ofFileDialogResult dialog_result = ofSystemLoadDialog("Load image file");

    if (dialog_result.bSuccess) {
        quads[activeQuad].loadImageFromFile(dialog_result.getName(), dialog_result.getPath());
        ofLogNotice() << "Loaded image: \"" << dialog_result.getPath() << "\"";
    }
}

//-----------------------------------------------------------
void ofApp::openVideoFile()
{
    ofFileDialogResult dialog_result = ofSystemLoadDialog("Load video file");

    if (dialog_result.bSuccess) {
        quads[activeQuad].loadVideoFromFile(dialog_result.getName(), dialog_result.getPath());
        ofLogNotice() << "Loaded video: \"" << dialog_result.getPath() << "\"";
    }
}

//-----------------------------------------------------------
void ofApp::loadSlideshow()
{
    ofFileDialogResult dialog_result = ofSystemLoadDialog("Find slideshow folder", true, "data"); // TODO: test if the default path works on linux, it doesn't seem to on windows

    if (dialog_result.bSuccess) {
        const std::string slideshowFolderName = dialog_result.getPath();
        quads[activeQuad].slideshowName = slideshowFolderName;
        ofLogNotice() << "Set slide show folder: \"" << slideshowFolderName << "\"";
    }
}

//-----------------------------------------------------------
void ofApp::openSharedVideoFile(int i)
{
    ofFileDialogResult dialog_result = ofSystemLoadDialog("Load shared video file");
    if (dialog_result.bSuccess) {
        if (sharedVideos[i].isLoaded()) {
            sharedVideos[i].closeMovie();
        }
        std::string path = dialog_result.getPath();
        sharedVideos[i].load(path);
        if (sharedVideos[i].isLoaded()) {
            ofLogNotice() << "Loaded shared video #" << i + 1 << ": \"" << path << "\"";
            sharedVideosFiles[i] = path;
            sharedVideos[i].setLoopState(OF_LOOP_NORMAL);
            sharedVideos[i].play();
            // sharedVideos[i].setVolume(0);
        }
    }
}

//-----------------------------------------------------------
void ofApp::openSharedVideoFile(std::string path, int i)
{
    if (sharedVideos[i].isLoaded()) {
        sharedVideos[i].closeMovie();
    }
    sharedVideos[i].load(path);
    if (sharedVideos[i].isLoaded()) {
        ofLogNotice() << "Loaded shared video: #" << i + 1 << ": \"" << path << "\"";
        sharedVideosFiles[i] = path;
        sharedVideos[i].setLoopState(OF_LOOP_NORMAL);
        sharedVideos[i].play();
        //sharedVideos[i].setVolume(0);
    }
}

//-----------------------------------------------------------
ofImage ofApp::loadImageFromFile()
{
    ofImage img;
    ofFileDialogResult dialog_result = ofSystemLoadDialog("Load image file", false);
    if (dialog_result.bSuccess) {
        string imgName = dialog_result.getName();
        string imgPath = dialog_result.getPath();
        img.load(imgPath);
        return img;
    }
    return img;
}

//--------------------------------------------------------------
void ofApp::resync()
{
    if (useTimeline) {
        timeline.setCurrentTimeSeconds(0.0);
    }

    for (int i = 0; i < MAX_QUADS; i++) {
        if (quads[i].initialized) {
            // resets video to start ing point
            if (quads[i].videoBg && quads[i].video.isLoaded()) {
                quads[i].video.setPosition(0.0);
            }
            // resets slideshow to first slide
            if (quads[i].slideshowBg) {
                quads[i].currentSlideId = 0;
                quads[i].slideTimer = 0;
            }
        }
    }
    for (int i = 0; i < 8; i++) {
        if (sharedVideos[i].isLoaded()) {
            sharedVideos[i].setPosition(0.0);
        }
    }
}

//--------------------------------------------------------------
void ofApp::startProjection()
{
    bStarted = true;
    if (useTimeline) {
        timeline.enable();
        timeline.play();
    }
    for (int i = 0; i < MAX_QUADS; i++) {
        if (quads[i].initialized) {
            quads[i].isOn = true;
            if (quads[i].videoBg && quads[i].video.isLoaded()) {
                quads[i].video.setVolume(quads[i].videoVolume);
                quads[i].video.play();
            }
        }
    }
    for (int i = 0; i < 8; i++) {
        if (sharedVideos[i].isLoaded()) {
            sharedVideos[i].play();
        }
    }
}

//--------------------------------------------------------------
void ofApp::stopProjection()
{
    bStarted = false;
    if (useTimeline) {
        timeline.stop();
        timeline.hide();
        timeline.disable();
    }
    for (int i = 0; i < MAX_QUADS; i++) {
        if (quads[i].initialized) {
            quads[i].isOn = false;
            if (quads[i].videoBg && quads[i].video.isLoaded()) {
                quads[i].video.setVolume(0);
                quads[i].video.stop();
            }
        }
    }
    for (int i = 0; i < 8; i++) {
        if (sharedVideos[i].isLoaded()) {
            sharedVideos[i].stop();
        }
    }
}

//--------------------------------------------------------------
void ofApp::copyQuadSettings(int sourceQuad)
{
    if (sourceQuad >= 0) {
        int layer = quads[activeQuad].layer;
        int quadNumber = quads[activeQuad].quadNumber;

        ofPoint corners[4];
        for (int i = 0; i < 4; i++) {
            corners[i] = quads[activeQuad].corners[i];
        }

        quads[activeQuad] = quads[sourceQuad];

        quads[activeQuad].layer = layer;
        quads[activeQuad].quadNumber = quadNumber;
        quads[activeQuad].isActive = true;

        for (int i = 0; i < 4; i++) {
            quads[activeQuad].corners[i] = corners[i];
        }
    }
}

//---------------------------------------------------------------
void ofApp::addQuad()
{
    if (isSetup) {
        bool hasSpareQuad = false;
        int index = 0;
        for (int i = 0; i < nOfQuads; i++) {
            if (quads[i].initialized == false) {
                hasSpareQuad = true;
                index = i;
                break;
            }
        }
        if (hasSpareQuad) {
            quads[index].setup(ofPoint(0.25, 0.25), ofPoint(0.75, 0.25), ofPoint(0.75, 0.75), ofPoint(0.25, 0.75), edgeBlendShader, quadMaskShader, surfaceShader, crossfadeShader, m_cameras, ttf);
            quads[index].quadNumber = index;

            // layers
            for (int i = 0; i < MAX_QUADS; i++) {
                if (layers[i] == -1) {
                    layers[i] = index;
                    quads[index].layer = i;
                    break;
                }
            }

            quads[activeQuad].isActive = false;
            quads[index].isActive = true;
            activeQuad = index;
            ++nOfQuads;
            m_gui.updatePages(quads[activeQuad]);

            // add timeline page for new quad
            timelineAddQuadPage(activeQuad);

            glDisable(GL_DEPTH_TEST);
        }
        else if (nOfQuads < MAX_QUADS) {
            quads[nOfQuads].setup(ofPoint(0.25, 0.25), ofPoint(0.75, 0.25), ofPoint(0.75, 0.75), ofPoint(0.25, 0.75), edgeBlendShader, quadMaskShader, surfaceShader, crossfadeShader, m_cameras, ttf);
            quads[nOfQuads].quadNumber = nOfQuads;

            // layers
            layers[nOfQuads] = nOfQuads;
            quads[nOfQuads].layer = nOfQuads;

            quads[activeQuad].isActive = false;
            quads[nOfQuads].isActive = true;
            activeQuad = nOfQuads;
            ++nOfQuads;
            m_gui.updatePages(quads[activeQuad]);

            // add timeline page for new quad
            timelineAddQuadPage(activeQuad);

            // next line fixes a bug i've been tracking down for a looong time
            glDisable(GL_DEPTH_TEST);
        }

    }
}

//---------------------------------------------------------------
void ofApp::deleteQuad()
{
    if (nOfQuads == 0)
        return;

    // remove timeline page for quad
    timelineRemoveQuadPage(activeQuad);

    // delete quad
    layers[quads[activeQuad].layer] = -1;
    quads[activeQuad].reset();
    nOfQuads--;

    for (int i = 0; i < MAX_QUADS; i++) {
        if (quads[i].isOn == true) {
            activeQuad = i;
            quads[activeQuad].isActive = true;
            m_gui.updatePages(quads[activeQuad]);

            // next line fixes a bug i've been tracking down for a looong time
            glDisable(GL_DEPTH_TEST);
            break;
        }
    }
}

//---------------------------------------------------------------
void ofApp::activateNextQuad()
{
    if (isSetup && (nOfQuads >= 2)) {
        quads[activeQuad].isActive = false; // activates next quad
        activeQuad += 1;
        if (activeQuad >= MAX_QUADS)
            activeQuad = 0;

        while (quads[activeQuad].initialized == false) {
            activeQuad++;
            if (activeQuad > (MAX_QUADS - 1)) {
                activeQuad = 0;
            }
        }

        quads[activeQuad].isActive = true;
        m_gui.updatePages(quads[activeQuad]);
    }
}

//---------------------------------------------------------------
void ofApp::activatePrevQuad()
{
    if (isSetup && (nOfQuads >= 2)) {
        quads[activeQuad].isActive = false; // activates prev quad
        activeQuad -= 1;

        if (activeQuad < 0)
            activeQuad = activeQuad = nOfQuads - 1;

        while (quads[activeQuad].initialized == false) {
            activeQuad--;
            if (activeQuad < 0) {
                activeQuad = MAX_QUADS - 1;
            }
        }

        quads[activeQuad].isActive = true;
        m_gui.updatePages(quads[activeQuad]);
    }
}
