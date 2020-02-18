#include "ofApp.h"


//-----------------------------------------------------------
void ofApp::openImageFile()
{
    ofFileDialogResult dialog_result = ofSystemLoadDialog("Load image file");

    if(dialog_result.bSuccess)
    {
        quads[activeQuad].loadImageFromFile(dialog_result.getName(), dialog_result.getPath());
        std::cout << "Loaded image: \"" << dialog_result.getPath() << "\"" << std::endl;
    }
}

//-----------------------------------------------------------
void ofApp::openVideoFile()
{
    ofFileDialogResult dialog_result = ofSystemLoadDialog("Load video file");

    if(dialog_result.bSuccess)
    {
        quads[activeQuad].loadVideoFromFile(dialog_result.getName(), dialog_result.getPath());
        std::cout << "Loaded video: \"" << dialog_result.getPath() << "\"" << std::endl;
    }
}

//-----------------------------------------------------------
void ofApp::loadSlideshow()
{
    ofFileDialogResult dialog_result = ofSystemLoadDialog("Find slideshow folder", true, "data"); // TODO: test if the default path works on linux, it doesn't seem to on windows

    if(dialog_result.bSuccess)
    {
        const std::string slideshowFolderName = dialog_result.getPath();
        quads[activeQuad].slideshowName = slideshowFolderName;
        std::cout << "Set slide show folder: \"" << slideshowFolderName << "\"" << std::endl;
   }
}

//-----------------------------------------------------------
void ofApp::openSharedVideoFile(int i)
{
    ofFileDialogResult dialog_result = ofSystemLoadDialog("Load shared video file");
    if(dialog_result.bSuccess)
    {
        if (sharedVideos[i].isLoaded())
        {
            sharedVideos[i].closeMovie();
        }
        std::string path = dialog_result.getPath();
        sharedVideos[i].load(path);
        if(sharedVideos[i].isLoaded())
        {
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
        if (sharedVideos[i].isLoaded())
        {
            sharedVideos[i].closeMovie();
        }
        sharedVideos[i].load(path);
        if(sharedVideos[i].isLoaded())
        {
            std::cout << "Loaded shared video: #" << i + 1 << ": \"" << path << "\"" << std::endl;
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
    if(dialog_result.bSuccess)
    {        
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
    #ifdef WITH_TIMELINE
    if(useTimeline)
    {
        timeline.setCurrentTimeSeconds(0.0);
    }
    #endif

    for(int i = 0; i < MAX_QUADS; i++)
    {
        if (quads[i].initialized)
        {
            // resets video to start ing point
            if (quads[i].videoBg && quads[i].video.isLoaded())
            {
                quads[i].video.setPosition(0.0);
            }
            // resets slideshow to first slide
            if (quads[i].slideshowBg)
            {
                quads[i].currentSlide = 0;
                quads[i].slideTimer = 0;
            }
            // reset trans colors
            if (quads[i].colorBg && quads[i].transBg)
            {
                quads[i].transCounter = 0;
                quads[i].transUp = true;
            }
        }
    }
    for(int i=0; i<8; i++)
    {
        if(sharedVideos[i].isLoaded())
        {
            sharedVideos[i].setPosition(0.0);
        }
    }
}

//--------------------------------------------------------------
void ofApp::startProjection()
{
    bStarted = true;
    #ifdef WITH_TIMELINE
    if(useTimeline)
    {
        timeline.enable();
        timeline.play();
    }
    #endif
    for(int i = 0; i < MAX_QUADS; i++)
    {
        if (quads[i].initialized)
        {
            quads[i].isOn = true;
            if (quads[i].videoBg && quads[i].video.isLoaded())
            {
                quads[i].video.setVolume(quads[i].videoVolume);
                quads[i].video.play();
            }
        }
    }
    for(int i=0; i<8; i++)
    {
        if(sharedVideos[i].isLoaded())
        {
            sharedVideos[i].play();
        }
    }
}

//--------------------------------------------------------------
void ofApp::stopProjection()
{
    bStarted = false;
    #ifdef WITH_TIMELINE
    if(useTimeline)
    {
        timeline.stop();
        timeline.hide();
        timeline.disable();
    }
    #endif
    for(int i = 0; i < MAX_QUADS; i++)
    {
        if (quads[i].initialized)
        {
            quads[i].isOn = false;
            if (quads[i].videoBg && quads[i].video.isLoaded())
            {
                quads[i].video.setVolume(0);
                quads[i].video.stop();
            }
        }
    }
    for(int i=0; i<8; i++)
    {
        if(sharedVideos[i].isLoaded())
        {
            sharedVideos[i].stop();
        }
    }
}

//--------------------------------------------------------------
void ofApp::copyQuadSettings(int sourceQuad)
{
    if(sourceQuad >= 0)
    {
        int layer = quads[activeQuad].layer;
        int quadNumber = quads[activeQuad].quadNumber;

        ofPoint corners[4];
        for(int i=0;i < 4;i++) {
            corners[i] = quads[activeQuad].corners[i];
        }

        quads[activeQuad] = quads[sourceQuad];

        quads[activeQuad].layer = layer;
        quads[activeQuad].quadNumber = quadNumber;
        quads[activeQuad].isActive = true;

        for(int i=0;i < 4;i++) {
            quads[activeQuad].corners[i] = corners[i];
        }
    }
}
