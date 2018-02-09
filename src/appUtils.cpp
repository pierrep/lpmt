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
            std::cout << "Loaded shared video #" << i + 1 << ": \"" << path << "\"" << std::endl;
            sharedVideosFiles[i] = path;
            sharedVideos[i].setLoopState(OF_LOOP_NORMAL);
            sharedVideos[i].play();
            sharedVideos[i].setVolume(0);
            for(int j=0; j < MAX_QUADS; j++)
            {
                if (quads[j].initialized)
                {
                    quads[j].vids[i] = sharedVideos[i];
                }
            }
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
            sharedVideos[i].setVolume(0);
            for(int j=0; j < MAX_QUADS; j++)
            {
                if (quads[j].initialized)
                {
                    quads[j].vids[i] = sharedVideos[i];
                }
            }
        }
}


//-----------------------------------------------------------
ofImage ofApp::loadImageFromFile()
{
    ofFileDialogResult dialog_result = ofSystemLoadDialog("Load image file", false);
    if(dialog_result.bSuccess)
    {
        ofImage img;
        string imgName = dialog_result.getName();
        string imgPath = dialog_result.getPath();
        ofFile image(imgPath);
        img.load(image);
        return img;
    }

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


        /*
        quads[activeQuad].quadDispX = quads[sourceQuad].quadDispX;
        quads[activeQuad].quadDispY = quads[sourceQuad].quadDispY;
        quads[activeQuad].quadW = quads[sourceQuad].quadW;
        quads[activeQuad].quadH = quads[sourceQuad].quadH;
        quads[activeQuad].imgBg = quads[sourceQuad].imgBg;
        //quads[activeQuad].loadedImg = quads[sourceQuad].loadedImg;
        quads[activeQuad].bgImg = quads[sourceQuad].bgImg;
        if ((quads[activeQuad].imgBg) && (quads[activeQuad].bgImg != ""))
        {
            quads[activeQuad].loadImageFromFile(quads[activeQuad].loadedImg, quads[activeQuad].bgImg);
        }
        quads[activeQuad].imgHFlip = quads[sourceQuad].imgHFlip;
        quads[activeQuad].imgVFlip = quads[sourceQuad].imgVFlip;
        quads[activeQuad].videoBg = quads[sourceQuad].videoBg;
        //quads[activeQuad].loadedVideo = quads[sourceQuad].loadedVideo;
        quads[activeQuad].bgVideo = quads[sourceQuad].bgVideo;
        if ((quads[activeQuad].videoBg) && (quads[activeQuad].bgVideo != ""))
        {
            quads[activeQuad].loadVideoFromFile(quads[activeQuad].loadedVideo, quads[activeQuad].bgVideo);
        }
        quads[activeQuad].videoHFlip = quads[sourceQuad].videoHFlip;
        quads[activeQuad].videoVFlip = quads[sourceQuad].videoVFlip;
        quads[activeQuad].bgSlideshow = quads[sourceQuad].bgSlideshow;
        quads[activeQuad].colorBg = quads[sourceQuad].colorBg;
        quads[activeQuad].transBg = quads[sourceQuad].transBg;
        quads[activeQuad].transDuration = quads[sourceQuad].transDuration;
        quads[activeQuad].slideshowBg = quads[sourceQuad].slideshowBg;
        quads[activeQuad].slideshowSpeed = quads[sourceQuad].slideshowSpeed;
        quads[activeQuad].slideFit = quads[sourceQuad].slideFit;
        quads[activeQuad].slideKeepAspect = quads[sourceQuad].slideKeepAspect;
        quads[activeQuad].camBg = quads[sourceQuad].camBg;
        quads[activeQuad].camWidth = quads[sourceQuad].camWidth;
        quads[activeQuad].camHeight = quads[sourceQuad].camHeight;
        quads[activeQuad].camHFlip = quads[sourceQuad].camHFlip;
        quads[activeQuad].camVFlip = quads[sourceQuad].camVFlip;
        quads[activeQuad].camMultX = quads[sourceQuad].camMultX;
        quads[activeQuad].camMultY = quads[sourceQuad].camMultY;
        quads[activeQuad].imgMultX = quads[sourceQuad].imgMultX;
        quads[activeQuad].imgMultY = quads[sourceQuad].imgMultY;
        quads[activeQuad].videoMultX = quads[sourceQuad].videoMultX;
        quads[activeQuad].videoMultY = quads[sourceQuad].videoMultY;
        quads[activeQuad].videoSpeed = quads[sourceQuad].videoSpeed;
        quads[activeQuad].videoVolume = quads[sourceQuad].videoVolume;
        quads[activeQuad].videoLoop = quads[sourceQuad].videoLoop;
        // copy colors
        quads[activeQuad].bgColor = quads[sourceQuad].bgColor;
        quads[activeQuad].secondColor = quads[sourceQuad].secondColor;
        quads[activeQuad].imgColorize = quads[sourceQuad].imgColorize;
        quads[activeQuad].videoColorize = quads[sourceQuad].videoColorize;
        quads[activeQuad].camColorize = quads[sourceQuad].camColorize;

        quads[activeQuad].bBlendModes = quads[sourceQuad].bBlendModes;
        quads[activeQuad].blendMode = quads[sourceQuad].blendMode;
        quads[activeQuad].edgeBlendBool = quads[sourceQuad].edgeBlendBool;
        quads[activeQuad].edgeBlendExponent = quads[sourceQuad].edgeBlendExponent;
        quads[activeQuad].edgeBlendGamma = quads[sourceQuad].edgeBlendGamma;
        quads[activeQuad].edgeBlendLuminance = quads[sourceQuad].edgeBlendLuminance;
        quads[activeQuad].edgeBlendAmountSin = quads[sourceQuad].edgeBlendAmountSin;
        quads[activeQuad].edgeBlendAmountDx = quads[sourceQuad].edgeBlendAmountDx;
        quads[activeQuad].edgeBlendAmountTop = quads[sourceQuad].edgeBlendAmountTop;
        quads[activeQuad].edgeBlendAmountBottom = quads[sourceQuad].edgeBlendAmountBottom;
        //mask stuff
        quads[activeQuad].bMask = quads[sourceQuad].bMask;
        quads[activeQuad].maskInvert = quads[sourceQuad].maskInvert;
        quads[activeQuad].m_maskPoints = quads[sourceQuad].m_maskPoints;
        quads[activeQuad].crop[0] = quads[sourceQuad].crop[0];
        quads[activeQuad].crop[1] = quads[sourceQuad].crop[1];
        quads[activeQuad].crop[2] = quads[sourceQuad].crop[2];
        quads[activeQuad].crop[3] = quads[sourceQuad].crop[3];
        quads[activeQuad].circularCrop[0] = quads[sourceQuad].circularCrop[0];
        quads[activeQuad].circularCrop[1] = quads[sourceQuad].circularCrop[1];
        quads[activeQuad].circularCrop[2] = quads[sourceQuad].circularCrop[2];
        // deform stuff
        quads[activeQuad].bDeform = quads[sourceQuad].bDeform;
        quads[activeQuad].bBezier = quads[sourceQuad].bBezier;
        quads[activeQuad].bGrid = quads[sourceQuad].bGrid;
        // bezier stuff
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                quads[activeQuad].bezierPoints[j][k][0] = quads[sourceQuad].bezierPoints[j][k][0];
                quads[activeQuad].bezierPoints[j][k][1] = quads[sourceQuad].bezierPoints[j][k][1];
                quads[activeQuad].bezierPoints[j][k][2] = quads[sourceQuad].bezierPoints[j][k][2];
            }
        }
        */

    }
}
