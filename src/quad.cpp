#include "quad.h"
#include "Util.hpp"

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>

quad::quad()
{

}

void quad::reset()
{
    // sets default variables
    initialized = true;
    isActive = false;
    isOn = false;
    isMaskSetup = false;
    colorBg = false;
    transBg = false;
    transUp = true;
    camBg = false;
    camGreenscreen = false;
    imgBg = false;
    videoBg = false;
    videoLoop = true;
    videoGreenscreen = false;
    sharedVideoBg = false;
    sharedVideoNum = 1;
    sharedVideoId = sharedVideoNum -1;
    slideshowBg = false;
    slideFit = false;
    slideKeepAspect = true;

    videoHFlip = false;
    imgHFlip = false;
    camHFlip = false;
    videoVFlip = false;
    imgVFlip = false;
    camVFlip = false;

    camNumber = prevCamNumber = 0;
    camMultX = 1;
    camMultY = 1;

    imgMultX = 1.0;
    imgMultY = 1.0;

    videoMultX = 1.0;
    videoMultY = 1.0;
    videoSpeed = 1.0;
    previousSpeed = 1.0;
    videoVolume = 0;

    currentSlide = 0;
    slideshowSpeed = 1.0;
    slideTimer = ofGetElapsedTimef();

    transDuration = 1.0;
    fps = ofGetFrameRate();
    transCounter = 0;

    // initialize the solid and transition colors with solid white
    bgColor = ofFloatColor(1, 1, 1, 1);
    secondColor = ofFloatColor(1, 1, 1, 1);

    // initialize some colors with transparent black
    startColor = ofFloatColor(0, 0, 0, 0);
    endColor = ofFloatColor(0, 0, 0, 0);

    // initialize some colors with solid white
    imgColorize = ofFloatColor(1, 1, 1, 1);
    videoColorize = ofFloatColor(1, 1, 1, 1);
    camColorize = ofFloatColor(1, 1, 1, 1);

    // greenscreen setup
    colorGreenscreen = ofFloatColor(0, 0, 0, 0);
    thresholdGreenscreen = 10;

    kinectBg = false;
    kinectImg = false;
    kinectMask = false;
    kinectMultX = 1.0;
    kinectMultY = 1.0;
    kinectColorize.r = 1.0;
    kinectColorize.g = 1.0;
    kinectColorize.b = 1.0;
    kinectColorize.a = 1.0;
    nearDepthTh = 255;
    farDepthTh = 0;
    kinectBlur = 3;
    kinectContourMin = 0.01;
    kinectContourMax = 1.0;
    kinectContourSimplify = 0.0;
    kinectContourSmooth = 10;
    getKinectContours = false;
    getKinectGrayImage = false;
    kinectContourCurved = false;

    #ifdef WITH_SYPHON
    // syphon variables
    bSyphon = false;
    syphonPosX = 0.0;
    syphonPosY = 0.0;
    syphonScaleX = 1.0;
    syphonScaleY = 1.0;
    #endif

    edgeBlendBool = false;
    edgeBlendExponent = 1.0;
    edgeBlendGamma = 1.8;
    edgeBlendLuminance = 0.0;
    edgeBlendAmountSin = 0.3;
    edgeBlendAmountDx = 0.3;
    edgeBlendAmountTop = 0.0;
    edgeBlendAmountBottom = 0.0;

    quadDispX = 0;
    quadDispY = 0;
    quadW = ofGetWidth();
    quadH = ofGetHeight();

    bBlendModes = false;
    blendMode = 0;

    bHighlightCorner = false;
    highlightedCorner = -1;

    bHighlightCenter = false;
    bHighlightRotation = false;

    m_maskPoints.clear();
    bMask = false;
    maskInvert = false;

    bHighlightMaskPoint = false;
    highlightedMaskPoint = -1;

    bDeform = false;

    //This sets up my Bezier Surface
    bBezier = false;
    isBezierSetup = false;
    highlightedCtrlPointRow = -1;
    highlightedCtrlPointCol = -1;

    // prepare bezier surface evaluator with control points
    bezierSurfaceSetup();

    // prepare grid surface evaluator
    bGrid = false;
    gridColumns = 7;
    gridRows = 6;
    gridSurfaceSetup();

    timelineRed = 1.0;
    timelineGreen = 1.0;
    timelineBlue = 1.0;
    timelineAlpha = 1.0;

    bTimelineTint = false;
    bTimelineColor = false;
    bTimelineAlpha = false;
    bTimelineSlideChange = false;
}

void quad::setup(ofPoint point1, ofPoint point2, ofPoint point3, ofPoint point4, ofShader &edgeBlendShader, ofShader &quadMaskShader, ofShader &chromaShader, vector<ofVideoGrabber> &cameras, vector<ofVideoPlayer> &sharedVideos, ofTrueTypeFont &font)
{
    reset();

    isSetup = true;
    isOn = true;

    shaderBlend = &edgeBlendShader;
    maskShader = &quadMaskShader;
    greenscreenShader = &chromaShader;
    //camera = &camGrabber;

    ttf = font;
    vids = sharedVideos;
    cams = cameras;
    if(cams.size() > 0)
    {
        camAvailable = true;
        setupCamera();
    }
    else
    {
        camAvailable = false;
    }

    bgImg = string("");
    loadedImg = string("");
    loadedVideo = string("");
    slideshowName = string("");
    loadedSlideshow = string("");

    quadNumber = 0;
    //this is just for our gui / mouse handles
    //this will end up being the destination quad we are warping too

    // setup the corner points
    corners[0] = point1;
    corners[1] = point2;
    corners[2] = point3;
    corners[3] = point4;

    center = (corners[0] + corners[1] + corners[2] + corners[3]) / 4;

    crop[0] = 0.0;
    crop[1] = 0.0;
    crop[2] = 0.0;
    crop[3] = 0.0;

    circularCrop[0] = 0.5;
    circularCrop[1] = 0.5;
    circularCrop[2] = 0.0;

    //videos = videoFiles;
    //slideshows = slideshowFolders;

    allocateFbo(ofGetWidth(), ofGetHeight());

    //calculates screen ratio factor for window and fullscreen
    screenFactorX = (ofGetWidth()/(float)ofGetScreenWidth());
    screenFactorY = (ofGetHeight()/(float)ofGetScreenHeight());

}


void quad::update()
{
    if (isOn)
    {
        sharedVideoId = sharedVideoNum - 1;

        if(camAvailable && camNumber != prevCamNumber)
        {
            setupCamera();
            prevCamNumber = camNumber;
        }
        //recalculates center of quad
        center = (corners[0] + corners[1] + corners[2] + corners[3]) / 4;

        // solid colors ---------------------------------------------------------------
        // calculates transition between two solid colors
        if (colorBg && transBg)
        {
            if (transUp)
            {
                startColor = bgColor;
                endColor = secondColor;
            }
            else
            {
                startColor = secondColor;
                endColor = bgColor;
            }
            // using fps detected at setup is suboptimal
            // but updating it at each cycle triggers a flickering effect
            // needs more work (e.g. introducing a roundup of actual framerate)
            // now we update fps value just if it differs more than 50fps from actual rate
            //transStep = (transDuration * ofGetFrameRate());
            //if (abs(fps-ofGetFrameRate()) > 50) {fps = ofGetFrameRate();}
            transStep = (transDuration * fps);
            transColor.r = startColor.r + (((endColor.r - startColor.r) / transStep) * transCounter);
            transColor.g = startColor.g + (((endColor.g - startColor.g) / transStep) * transCounter);
            transColor.b = startColor.b + (((endColor.b - startColor.b) / transStep) * transCounter);
            transColor.a = startColor.a + (((endColor.a - startColor.a) / transStep) * transCounter);
            transCounter += 1;
            if (transCounter >= transStep)
            {
                transCounter = 0;
                transUp = !transUp;
            }

        }


        // live camera --------------------------------------------------------------


        // video --------------------------------------------------------------------
        // loads video
        if (videoBg)
        {
            video.setVolume(videoVolume);
            // check for looping config parameter of video and sets loopstate - OF_LOOP_NORMAL = cycles / OF_LOOP_NONE = stops at the end
            if (videoLoop)
            {
                video.setLoopState(OF_LOOP_NORMAL);
            }
            else
            {
                video.setLoopState(OF_LOOP_NONE);
            }

            if (video.isLoaded())
            {
                video.update();
            }

            // changevideo speed
            if (previousSpeed != videoSpeed)
            {
                video.setSpeed(videoSpeed);
                previousSpeed = videoSpeed;
            }
        }

        // slideshow -----------------------------------------------------------------
        if (slideshowBg)
        {
            // put it to off while loading images
            slideshowBg = false;
            // if a different slideshow has been chosen in gui we do load its images
            if ((slideshowName != loadedSlideshow) && slideshowName != "")
            {
                // we exclude "." and ".." directories if present
                if (slideshowName != "." && slideshowName != "..")
                {
                    // we scan the img dir for images
                    string slidesDir = slideshowName;
                    // make two arrays, one for imgs names and one for images
                    slidesnames = vector<string>();
                    slides = vector<ofImage>();
                    // read all content of show folder
                    getdir(slidesDir,slidesnames);
                    // for each name found loads the image and populates the imgs array (excluding "." an "..")
                    for (unsigned int i = 0; i < slidesnames.size(); i++)
                    {
                        if (slidesnames[i] != "." && slidesnames[i] != "..")
                        {
                            slide.load(slideshowName+"/"+slidesnames[i]);
                            slides.push_back(slide);
                        }
                    }
                    loadedSlideshow = slideshowName;
                    currentSlide = 0;
                    slideTimer = 0;
                }
            }
            // turn it on again for drawing
            // update fps
            //if (abs(fps-ofGetFrameRate()) > 50) {fps = ofGetFrameRate();}
            slideFramesDuration = (slideshowSpeed * fps);
            slideshowBg = true;
        }

        // -------------------------
        // finds kinect blobs with OpenCV
        #ifdef WITH_KINECT
        if (kinectBg)
        {
            kinectThreshImage.clear();
            kinectContourImage.clear();
            kinectThreshImage.allocate(quadKinect->kinect.width, quadKinect->kinect.height);
            kinectContourImage.allocate(quadKinect->kinect.width, quadKinect->kinect.height);
            kinectThreshImage = quadKinect->getThresholdDepthImage(nearDepthTh, farDepthTh, kinectBlur);
            kinectContourImage = kinectThreshImage;
            kinectContourFinder.findContours(kinectContourImage, (quadKinect->kinect.width*quadKinect->kinect.height)*kinectContourMin, (quadKinect->kinect.width*quadKinect->kinect.height)*kinectContourMax, 20, true);
            // clear kinect path if any
            kinectPath.clear();
            kinectPath.setFilled(true);
            for( int i=0; i<(int)kinectContourFinder.blobs.size(); i++ )
            {
                ofPolyline poly(kinectContourFinder.blobs[i].pts);
                poly.close();
                poly.simplify(kinectContourSimplify);
                ofPolyline polySmoothed = poly.getSmoothed(kinectContourSmooth);
                //polySmoothed.close();
                vector<ofPoint> points = polySmoothed.getVertices();

                for(size_t j=0; j<points.size(); j++ )
                {
                    if (kinectContourCurved)
                    {
                        kinectPath.curveTo(points[j]);
                    }
                    else
                    {
                        kinectPath.lineTo(points[j]);
                    }
                }
                kinectPath.close();
            }
        }
        #endif

        //we set matrix to the default - 0 translation
        //and 1.0 scale for x y z and w
        for(int i = 0; i < 16; i++)
        {
            if(i % 5 != 0) matrix[i] = 0.0;
            else matrix[i] = 1.0;
        }

        //we set the warp coordinates
        //source coordinates as the dimensions of our window
        src[0] = ofPoint(           0,             0);
        src[1] = ofPoint(ofGetWidth(),             0);
        src[2] = ofPoint(ofGetWidth(), ofGetHeight());
        src[3] = ofPoint(           0, ofGetHeight());

        //corners are in 0.0 - 1.0 range
        //so we scale up so that they are at the window's scale
        for(int i = 0; i < 4; i++)
        {
            dst[i].x = corners[i].x * (float)ofGetWidth();
            dst[i].y = corners[i].y * (float) ofGetHeight();
        }

    }
}


// DRAW -----------------------------------------------------------------
void quad::draw()
{
    if (isOn)
    {
        // recalculates bezier surface
        if(bBezier)
        {
            bezierSurfaceUpdate();
        }
        if(bGrid)
        {
            gridSurfaceUpdate();
        }


        quadFbo.begin();
        ofClear(0.0,0.0,0.0,0.0);
        ofEnableAlphaBlending();
        // -- NOW LETS DRAW!!!!!!  -----

        // if a solid color content or color transition is set it draws it
        // solid colors ----------------------------------------------------------------
        if (colorBg)
        {
            ofFill();
            // if we have two colors it draws with calculated transition color
            if (transBg)
            {
                ofSetColor(transColor.r * 255 * timelineRed, transColor.g * 255 * timelineGreen, transColor.b * 255 * timelineBlue, transColor.a * 255 * timelineAlpha);
            }
            // this in case of only one color set
            else
            {
                if(bTimelineColor) {
                    ofSetColor(timelineColor.r * 255 * timelineRed, timelineColor.g * 255 * timelineGreen, timelineColor.b * 255 * timelineBlue, timelineColor.a * 255 * timelineAlpha);
                } else {
                    ofSetColor(bgColor.r * 255 * timelineRed, bgColor.g * 255 * timelineGreen, bgColor.b * 255 * timelineBlue, bgColor.a * 255 * timelineAlpha);
                }
            }
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            ofNoFill();
        }

        // video ----------------------------------------------------------------------
        //if a video content is chosen it draws it
        if (videoBg)
        {
            if (videoHFlip || videoVFlip)
            {
                glPushMatrix();
                if(videoHFlip && !videoVFlip)
                {
                    ofTranslate(videoWidth*videoMultX,0);
                    glScalef(-1,1,1);
                }
                else if(videoVFlip && !videoHFlip)
                {
                    ofTranslate(0,videoHeight*videoMultY);
                    glScalef(1,-1,1);
                }
                else
                {
                    ofTranslate(videoWidth*videoMultX,videoHeight*videoMultY);
                    glScalef(-1,-1,1);
                }
            }
            ofSetColor(videoColorize.r * 255 * timelineRed, videoColorize.g * 255 * timelineGreen, videoColorize.b * 255 * timelineBlue, videoColorize.a * 255 * timelineAlpha);
            if (!videoLoop)
            {
                // in no-looping mode it stops drawing video frame when video reaches the end
                // using 'getIsMovieDone()' because there are problems with getting head position under GStream
                if (!video.getIsMovieDone())
                {
                    if (videoGreenscreen)
                    {
                        greenscreenShader->begin();
                        greenscreenShader->setUniformTexture("tex", video.getTexture(),0 );
                        greenscreenShader->setUniform1f("greenscreenR", colorGreenscreen.r);
                        greenscreenShader->setUniform1f("greenscreenG", colorGreenscreen.g);
                        greenscreenShader->setUniform1f("greenscreenB", colorGreenscreen.b);
                        // we pass tint values too
                        greenscreenShader->setUniform1f("tintR", videoColorize.r);
                        greenscreenShader->setUniform1f("tintG", videoColorize.g);
                        greenscreenShader->setUniform1f("tintB", videoColorize.b);
                        greenscreenShader->setUniform1f("greenscreenT", (float)thresholdGreenscreen/255.0);
                        video.draw(0,0,videoWidth*videoMultX, videoHeight*videoMultY);
                        greenscreenShader->end();
                    }
                    else
                    {
                        video.draw(0,0,videoWidth*videoMultX, videoHeight*videoMultY);
                    }
                }
            }
            else
            {
                if (videoGreenscreen)
                {
                        greenscreenShader->begin();
                        greenscreenShader->setUniformTexture("tex", video.getTexture(),0 );
                        greenscreenShader->setUniform1f("greenscreenR", colorGreenscreen.r);
                        greenscreenShader->setUniform1f("greenscreenG", colorGreenscreen.g);
                        greenscreenShader->setUniform1f("greenscreenB", colorGreenscreen.b);
                        // we pass tint values too
                        greenscreenShader->setUniform1f("tintR", videoColorize.r);
                        greenscreenShader->setUniform1f("tintG", videoColorize.g);
                        greenscreenShader->setUniform1f("tintB", videoColorize.b);
                        greenscreenShader->setUniform1f("greenscreenT", (float)thresholdGreenscreen/255.0);
                        if(video.isPlaying())
                        {
                            video.draw(0,0,videoWidth*videoMultX, videoHeight*videoMultY);
                        }
                        greenscreenShader->end();
                }
                else
                {
                    if(video.isPlaying())
                    {
                        video.draw(0,0,videoWidth*videoMultX, videoHeight*videoMultY);
                    }
                }
            }
            if (videoHFlip || videoVFlip)
            {
                glPopMatrix();
            }
        }

        // shared video ----------------------------------------------------------------------

        if (sharedVideoBg)
        {
            if (videoHFlip || videoVFlip)
            {
                glPushMatrix();
                if(videoHFlip && !videoVFlip)
                {
                    ofTranslate(vids[sharedVideoId].getWidth()*videoMultX,0);
                    glScalef(-1,1,1);
                }
                else if(videoVFlip && !videoHFlip)
                {
                    ofTranslate(0,vids[sharedVideoId].getHeight()*videoMultY);
                    glScalef(1,-1,1);
                }
                else
                {
                    ofTranslate(vids[sharedVideoId].getWidth()*videoMultX,vids[sharedVideoId].getHeight()*videoMultY);
                    glScalef(-1,-1,1);
                }
            }
            ofSetColor(videoColorize.r * 255 * timelineRed, videoColorize.g * 255 * timelineGreen, videoColorize.b * 255 * timelineBlue, videoColorize.a * 255 * timelineAlpha);
            if (videoGreenscreen)
            {
                greenscreenShader->begin();
                greenscreenShader->setUniformTexture("tex", vids[sharedVideoId].getTexture(),0 );
                greenscreenShader->setUniform1f("greenscreenR", colorGreenscreen.r);
                greenscreenShader->setUniform1f("greenscreenG", colorGreenscreen.g);
                greenscreenShader->setUniform1f("greenscreenB", colorGreenscreen.b);
                // we pass tint values too
                greenscreenShader->setUniform1f("tintR", videoColorize.r);
                greenscreenShader->setUniform1f("tintG", videoColorize.g);
                greenscreenShader->setUniform1f("tintB", videoColorize.b);
                greenscreenShader->setUniform1f("greenscreenT", (float)thresholdGreenscreen/255.0);
                vids[sharedVideoId].draw(0,0,vids[sharedVideoId].getWidth()*videoMultX, vids[sharedVideoId].getHeight()*videoMultY);
                greenscreenShader->end();
            }
            else
            {
                vids[sharedVideoId].draw(0,0,vids[sharedVideoId].getWidth()*videoMultX, vids[sharedVideoId].getHeight()*videoMultY);
            }
            if (videoHFlip || videoVFlip)
            {
                glPopMatrix();
            }
        }


        // camera ------------------------------------------------------------------------------
        // camera stuff
        if (camAvailable && camBg && cams[camNumber].getWidth() > 0)
        {
            if (camHFlip || camVFlip)
            {
                glPushMatrix();
                if(camHFlip && !camVFlip)
                {
                    ofTranslate(camWidth*camMultX,0);
                    glScalef(-1,1,1);
                }
                else if(camVFlip && !camHFlip)
                {
                    ofTranslate(0,camHeight*camMultY);
                    glScalef(1,-1,1);
                }
                else
                {
                    ofTranslate(camWidth*camMultX,camHeight*camMultY);
                    glScalef(-1,-1,1);
                }
            }
            ofSetColor(camColorize.r * 255 * timelineRed, camColorize.g * 255 * timelineGreen, camColorize.b * 255 * timelineBlue, camColorize.a * 255 * timelineAlpha);
            if (camGreenscreen)
            {
                greenscreenShader->begin();
                greenscreenShader->setUniformTexture("tex", cams[camNumber].getTexture(),0 );
                greenscreenShader->setUniform1f("greenscreenR", colorGreenscreen.r);
                greenscreenShader->setUniform1f("greenscreenG", colorGreenscreen.g);
                greenscreenShader->setUniform1f("greenscreenB", colorGreenscreen.b);
                // we pass tint values too
                greenscreenShader->setUniform1f("tintR", camColorize.r);
                greenscreenShader->setUniform1f("tintG", camColorize.g);
                greenscreenShader->setUniform1f("tintB", camColorize.b);
                greenscreenShader->setUniform1f("greenscreenT", (float)thresholdGreenscreen/255.0);
                cams[camNumber].getTexture().draw(0,0,camWidth*camMultX,camHeight*camMultY);
                greenscreenShader->end();
            }
            else
            {
                //camTexture.draw(0,0,camWidth*camMultX,camHeight*camMultY); // orig
                cams[camNumber].getTexture().draw(0,0,camWidth*camMultX,camHeight*camMultY);

            }
            if (camHFlip || camVFlip)
            {
                glPopMatrix();
            }
        }

        // draws slideshows
        if (slideshowBg)
        {
            if (slides.size() > 0)
            {
                // if we reached the end of slides vector, it loops back to first slide
                if (currentSlide >= slides.size())
                {
                    currentSlide = 0;
                }
                slide = slides[currentSlide];
                // color is set according to still img colorization combo
                ofSetColor(imgColorize.r * 255 * timelineRed, imgColorize.g * 255 * timelineGreen, imgColorize.b * 255 * timelineBlue, imgColorize.a * 255 * timelineAlpha);
                // default is drawing image with its size unchanged, so we set mult factors = 1.0
                float multX = 1.0;
                float multY = 1.0;
                if (slideFit)
                {
                    float fitX = ofGetWidth()/slide.getWidth();
                    float fitY = ofGetHeight()/slide.getHeight();
                    if (slideKeepAspect)
                    {
                        // we calculate the factor for fitting the image in quad respecting img aspect ratio
                        if (fitX >= fitY)
                        {
                            multX = fitY;
                            multY = fitY;
                        }
                        else
                        {
                            multX = fitX;
                            multY = fitX;
                        }
                    }
                    // this is for stretching image to whole quad size
                    else
                    {
                        multX = fitX;
                        multY = fitY;
                    }
                }
                // at last we draw the image with appropriate size multiplier
                slide.draw(0,0,slide.getWidth()*multX, slide.getHeight()*multY);

                // if slide showing time has elapsed it switches to next slide
                if (slideTimer > slideFramesDuration )
                {
                    // check if we are using timeline to change slides
                    if(!bTimelineSlideChange)
                    {
                        currentSlide += 1;
                    }
                    slideTimer = 0;
                }
                slideTimer += 1;
            }
        }


        //if an image content is chosen it draws it (maybe use it as mask as well?)
        if (imgBg)
        {
            if (imgHFlip || imgVFlip)
            {
                glPushMatrix();
                if(imgHFlip && !imgVFlip)
                {
                    ofTranslate(img.getWidth()*imgMultX*screenFactorX,0);
                    glScalef(-1,1,1);
                }
                else if(imgVFlip && !imgHFlip)
                {
                    ofTranslate(0,img.getHeight()*imgMultY*screenFactorY);
                    glScalef(1,-1,1);
                }
                else
                {
                    ofTranslate(img.getWidth()*imgMultX*screenFactorX,img.getHeight()*imgMultY*screenFactorY);
                    glScalef(-1,-1,1);
                }
            }
            ofSetColor(imgColorize.r * 255 * timelineRed, imgColorize.g * 255 * timelineGreen, imgColorize.b * 255 * timelineBlue, imgColorize.a * 255 * timelineAlpha);
            //img.draw(0,0,img.getWidth()*imgMultX, img.getHeight()*imgMultY);
            //img.draw(0,0,img.getWidth()*imgMultX/(img.getWidth()/ofGetWidth()), img.getHeight()*imgMultY/(img.getHeight()/ofGetHeight()));
            img.draw(0,0,img.getWidth()*imgMultX*screenFactorX, img.getHeight()*imgMultY*screenFactorY);
            if (imgHFlip || imgVFlip)
            {
                glPopMatrix();
            }
        }

        // kinect stuff
        #ifdef WITH_KINECT
        if (kinectBg && kinectImg)
        {
            ofSetColor(kinectColorize.r * 255 * timelineRed, kinectColorize.g * 255 * timelineGreen, kinectColorize.b * 255 * timelineBlue, kinectColorize.a * 255 * timelineAlpha);
            //quadKinect->grayImage.draw(0,0,quadKinect->grayImage.getWidth()*kinectMultX,quadKinect->grayImage.getHeight()*kinectMultY);
            if (getKinectContours)
            {
                ofPushStyle();
                glPushMatrix();
                glScalef( kinectMultX, kinectMultY, 0.0 );
                // ----------- draw the kinect path made of detected blobs
                ofColor pathColor(kinectColorize.r * 255 * timelineRed, kinectColorize.g * 255 * timelineGreen, kinectColorize.b * 255 * timelineBlue, kinectColorize.a * 255 * timelineAlpha);
                kinectPath.setFillColor(pathColor);
                kinectPath.draw();
                glPopMatrix();
                ofPopStyle();
            }
            else if (getKinectGrayImage)
            {
                quadKinect->grayImage.draw(0,0,quadKinect->grayImage.getWidth()*kinectMultX,quadKinect->grayImage.getHeight()*kinectMultY);
            }
            else
            {
                kinectThreshImage.draw(0,0,quadKinect->grayImage.getWidth()*kinectMultX,quadKinect->grayImage.getHeight()*kinectMultY);
            }
        }
        #endif

        #ifdef WITH_SYPHON
        // syphon stuff
		if (bSyphon)
		{
			ofSetColor(255, 255, 255);
			syphClientTex->draw(syphonPosX, syphonPosY, syphonScaleX*syphClientTex->getWidth(), syphonScaleY*syphClientTex->getHeight());
		}
        #endif

        ofDisableAlphaBlending();
        quadFbo.end();

        //mask on mask FBO
        maskFbo.begin();
        ofClear(0.0,0.0,0.0,0.0);
        ofEnableAlphaBlending();
        ofFill();
        ofEnableSmoothing();

        // crop rectangular mask
        ofSetColor(255,255,255);
        ofDrawRectangle(0,0,ofGetWidth(),crop[0]*ofGetHeight());
        ofDrawRectangle(ofGetWidth()*(1-crop[1]),0,ofGetWidth()*crop[1],ofGetHeight());
        ofDrawRectangle(0,ofGetHeight()*(1-crop[2]),ofGetWidth(),crop[2]*ofGetHeight());
        ofDrawRectangle(0,0,ofGetWidth()*crop[3], ofGetHeight());

        // crop circular mask
        if(circularCrop[2]>0.0)
        {
            ofSetCircleResolution(64);
            ofDrawCircle(circularCrop[0]*ofGetWidth(), circularCrop[1]*ofGetHeight(), circularCrop[2]*ofGetWidth());
            ofSetCircleResolution(22);
        }

        // draw the user-defined mask on the mask FBO
        if(m_maskPoints.size() > 0)
        {
            ofSetColor(255, 255, 255); // white
            ofBeginShape();
            for(size_t i = 0; i < m_maskPoints.size(); i++)
            {
                ofPoint scaledPoint = Util::scalePointToPixel(m_maskPoints[i]);
                ofVertex(scaledPoint);
            }
            ofEndShape(true);
        }
        #ifdef WITH_KINECT
        if(kinectBg && kinectMask)
        {
            ofSetColor(255,255,255);
            if (getKinectContours)
            {
                ofPushStyle();
                glPushMatrix();
                glScalef( kinectMultX, kinectMultY, 0.0 );
                // ----------- draw the kinect path made of detected blobs
                ofColor pathColor(255, 255, 255, 255);
                kinectPath.setFillColor(pathColor);
                kinectPath.draw();
                glPopMatrix();
                ofPopStyle();
            }
            else
            {
                kinectThreshImage.draw(0,0,quadKinect->grayImage.getWidth()*kinectMultX,quadKinect->grayImage.getHeight()*kinectMultY);
            }
        }
        #endif
        ofDisableSmoothing();
        ofNoFill();
        ofDisableAlphaBlending();
        maskFbo.end();

        // save actual GL coordinates
        ofPushMatrix();
        // find transformation matrix
        findHomography(src, dst, matrix);
        //finally lets multiply our matrix
        //wooooo hoooo!
        glMultMatrixf(matrix);


        if(edgeBlendBool)
        {
            if(quadFbo.getWidth()>0)
            {
                ofEnableAlphaBlending();
                shaderBlend->begin();
                shaderBlend->setUniformTexture ("tex", quadFbo.getTexture(), 0);
                shaderBlend->setUniform1f("exponent", edgeBlendExponent);
                shaderBlend->setUniform1f("userGamma", edgeBlendGamma);
                shaderBlend->setUniform3f("userLuminance", edgeBlendLuminance, edgeBlendLuminance, edgeBlendLuminance);
                shaderBlend->setUniform4f("amount", edgeBlendAmountSin, edgeBlendAmountTop, edgeBlendAmountDx, edgeBlendAmountBottom);
                shaderBlend->setUniform1i("w", ofGetWidth());
                shaderBlend->setUniform1i("h", ofGetHeight());
                //set ofColor to white
                ofSetColor(255,255,255);
                //Blend modes stuff (with shaders would be better, but it scales bad on older GPUs)
                if(bBlendModes)
                {
                    glEnable(GL_BLEND);
                    applyBlendmode();
                }
                if(!bDeform)
                {
                    quadFbo.draw(0+quadDispX,0+quadDispY,quadW,quadH);
                    shaderBlend->end();
                }
                else
                {


                    targetFbo.begin();
                    ofClear(0.0,0.0,0.0,0.0);
                    quadFbo.draw(0+quadDispX,0+quadDispY,quadW,quadH);
                    shaderBlend->end();
                    targetFbo.end();

                    if(bBezier)
                    {
                        targetFbo.getTexture().bind();

                        glMatrixMode(GL_TEXTURE);
                        glPushMatrix();//to scale the texture
                        glLoadIdentity();

                        ofTextureData texData = targetFbo.getTexture().getTextureData();
                        if(texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB)
                        {
                            glScalef(targetFbo.getTexture().getWidth(), targetFbo.getTexture().getHeight(), 1.0f);
                        }
                        else
                        {
                            glScalef(targetFbo.getTexture().getWidth() / texData.tex_w, targetFbo.getTexture().getHeight() / texData.tex_h, 1.0f);
                        }
                        glMatrixMode(GL_MODELVIEW);

                        glEnable(GL_MAP2_VERTEX_3);
                        glEnable(GL_AUTO_NORMAL);
                        // this tries to prevent the double alpha problem
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                        //draw the bezier shape
                        glEvalMesh2(GL_FILL, 0, 20, 0, 20);
                        glDisable(GL_BLEND);
                        glDisable(GL_MAP2_VERTEX_3);
                        glDisable(GL_AUTO_NORMAL);

                        targetFbo.getTexture().unbind();
                        glMatrixMode(GL_TEXTURE);
                        glPopMatrix();// texture scale pop matrix
                        glMatrixMode(GL_MODELVIEW);
                    }
                    else if(bGrid)
                    {
                        targetFbo.getTexture().bind();
                        gridMesh.drawFaces();
                        targetFbo.getTexture().unbind();
                        if(isActive && isBezierSetup)
                        {
                            ofPushStyle();
                            ofSetColor(255,255,255,200);
                            gridMesh.drawWireframe();
                            ofPopStyle();
                        }
                    }

                }

                if(bBlendModes)
                {
                    glDisable(GL_BLEND);
                }
                ofDisableAlphaBlending();
            }
        }

        else
        {
            if(bMask)
            {
                if(quadFbo.getWidth()>0)
                {
                    ofEnableAlphaBlending();
                    if (maskInvert)
                    {
                        maskMode = 1;
                    }
                    else
                    {
                        maskMode = 0;
                    }
                    maskShader->begin();
                    maskShader->setUniformTexture ("tex", quadFbo.getTexture(), 0);
                    maskShader->setUniformTexture ("mask", maskFbo.getTexture(), 1);
                    maskShader->setUniform1i ("mode", maskMode);
                    //set ofColor to white
                    ofSetColor(255,255,255);
                    //Blend modes stuff (with shaders would be better, but it scales bad on older GPUs)
                    if(bBlendModes)
                    {
                        glEnable(GL_BLEND);
                        applyBlendmode();
                    }
                    if(!bDeform)
                    {
                        quadFbo.draw(0+quadDispX,0+quadDispY,quadW,quadH);
                        maskShader->end();
                    }
                    else
                    {
                        targetFbo.begin();
                        ofClear(0.0,0.0,0.0,0.0);
                        quadFbo.draw(0+quadDispX,0+quadDispY,quadW,quadH);
                        maskShader->end();
                        targetFbo.end();


                        if(bBezier)
                        {
                            targetFbo.getTexture().bind();

                            glMatrixMode(GL_TEXTURE);
                            glPushMatrix();//to scale the texture
                            glLoadIdentity();

                            ofTextureData texData = targetFbo.getTexture().getTextureData();
                            if(texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB)
                            {
                                glScalef(targetFbo.getTexture().getWidth(), targetFbo.getTexture().getHeight(), 1.0f);
                            }
                            else
                            {
                                glScalef(targetFbo.getTexture().getWidth() / texData.tex_w, targetFbo.getTexture().getHeight() / texData.tex_h, 1.0f);
                            }
                            glMatrixMode(GL_MODELVIEW);
                            glEnable(GL_MAP2_VERTEX_3);
                            glEnable(GL_AUTO_NORMAL);
                            // this tries to prevent the double alpha problem
                            glEnable(GL_BLEND);
                            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                            //draw the bezier shape
                            glEvalMesh2(GL_FILL, 0, 20, 0, 20);
                            glDisable(GL_BLEND);
                            glDisable(GL_MAP2_VERTEX_3);
                            glDisable(GL_AUTO_NORMAL);

                            targetFbo.getTexture().unbind();
                            glMatrixMode(GL_TEXTURE);
                            glPopMatrix();// texture scale pop matrix
                            glMatrixMode(GL_MODELVIEW);
                        }
                        else if(bGrid)
                        {
                            targetFbo.getTexture().bind();
                            gridMesh.drawFaces();
                            targetFbo.getTexture().unbind();
                            if(isActive && isBezierSetup)
                            {
                                ofPushStyle();
                                ofSetColor(255,255,255,200);
                                gridMesh.drawWireframe();
                                ofPopStyle();
                            }
                        }

                    }

                    if(bBlendModes)
                    {
                        glDisable(GL_BLEND);
                    }
                    ofDisableAlphaBlending();

                }
            }

            else
            {
                if(quadFbo.getWidth()>0)
                {
                    ofEnableAlphaBlending();
                    //set ofColor to white
                    ofSetColor(255,255,255);
                    //Blend modes stuff (with shaders would be better, but it scales bad on older GPUs)
                    if(bBlendModes)
                    {
                        glEnable(GL_BLEND);
                        applyBlendmode();
                    }

                    if(!bDeform)
                    {
                        quadFbo.draw(0+quadDispX,0+quadDispY,quadW,quadH);
                    }
                    else
                    {

                        if(bBezier)
                        {
                            quadFbo.getTexture().bind();

                            glMatrixMode(GL_TEXTURE);
                            glPushMatrix();//to scale the texture
                            glLoadIdentity();

                            ofTextureData texData = quadFbo.getTexture().getTextureData();
                            if(texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB)
                            {
                                glScalef(quadFbo.getTexture().getWidth(), quadFbo.getTexture().getHeight(), 1.0f);
                            }
                            else
                            {
                                glScalef(quadFbo.getTexture().getWidth() / texData.tex_w, quadFbo.getTexture().getHeight() / texData.tex_h, 1.0f);
                            }
                            glMatrixMode(GL_MODELVIEW);
                            //draw the bezier shape
                            glEnable(GL_MAP2_VERTEX_3);
                            glEnable(GL_AUTO_NORMAL);

                            glEvalMesh2(GL_FILL, 0, 20, 0, 20);

                            glDisable(GL_MAP2_VERTEX_3);
                            glDisable(GL_AUTO_NORMAL);

                            quadFbo.getTexture().unbind();
                            glMatrixMode(GL_TEXTURE);
                            glPopMatrix();// texture scale pop matrix
                            glMatrixMode(GL_MODELVIEW);
                        }
                        else if(bGrid)
                        {
                            quadFbo.getTexture().bind();
                            gridMesh.drawFaces();
                            quadFbo.getTexture().unbind();
                            if(isActive && isBezierSetup)
                            {
                                ofPushStyle();
                                ofSetColor(255,255,255,200);
                                gridMesh.drawWireframe();
                                ofPopStyle();
                            }
                        }
                    }

                    if(bBlendModes)
                    {
                        glDisable(GL_BLEND);
                    }
                    ofDisableAlphaBlending();
                }
            }
        }


        // draw mask hatch if we are in mask setup mode
        if(isActive && isMaskSetup)
        {
            ofEnableAlphaBlending();
            //set ofColor to red with alpha
            //ofSetColor(255,100,100,180);
            ofSetColor(100,139,150,160);
            maskFbo.draw(0+quadDispX,0+quadDispY,quadW,quadH);
            ofDisableAlphaBlending();
        }

        // in setup mode draw the border of the quad and if it is active draw the helper grid
        if (isSetup)
        {
            ofNoFill();
            ofSetLineWidth(1.0);
            ofEnableSmoothing();
            if (isActive)
            {
                // draws helper grid on active quad
                ofSetHexColor(0x444444); // dark-grey
                // horizontal lines
                ofDrawLine(0, ofGetHeight() / 4, ofGetWidth(), ofGetHeight() / 4);
                ofDrawLine(0, ofGetHeight() / 2, ofGetWidth(), ofGetHeight() / 2);
                ofDrawLine(0, ofGetHeight() / 2 + ofGetHeight() / 4, ofGetWidth(), ofGetHeight() / 2 + ofGetHeight() / 4);
                // vertical lines
                ofDrawLine(ofGetWidth() / 4, 0, ofGetWidth() / 4, ofGetHeight());
                ofDrawLine(ofGetWidth() / 2, 0, ofGetWidth() / 2, ofGetHeight());
                ofDrawLine(ofGetWidth() / 2 + ofGetWidth() / 4, 0, ofGetWidth() / 2 + ofGetWidth() / 4, ofGetHeight());
                // little arrow at top center to indicate where up is
                ofDrawLine(ofGetWidth() / 2, 0, ofGetWidth() / 2 - 20, 20);
                ofDrawLine(ofGetWidth() / 2, 0, ofGetWidth() / 2 + 20, 20);
                ofDrawLine(ofGetWidth() / 2 - 20, 20, ofGetWidth() / 2 + 20, 20);

                // set active quads border to be white
                ofSetHexColor(0xFFFFFF); // white
                ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            }
            else
            {
                // if the quad is not active draw a grey border
                ofSetHexColor(0x666666); // light-grey
                ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            }
            ofDisableSmoothing();
        }

        ofPopMatrix();

        if (isActive)
        {
            // draws markers for bezier deform setup
            if(isBezierSetup)
            {
                if (bBezier)
                {
                    drawBezierMarkers();
                }
                else if (bGrid)
                {
                    drawGridMarkers();
                }
            }
            // draws mask markers and contour in mask-setup mode
            if(isMaskSetup)
            {
                drawMaskMarkers();
            }
        }

        if (isSetup)
        {
            // transform the normalized coordinates into window pixel coordinates
            const ofPoint centerInPixel = Util::scalePointToPixel(center);

            // draw the a string with the surface number a little offset in black, as a drop shadow
            ofSetHexColor(0x000000); // black
            ttf.drawString("surface " + ofToString(quadNumber), centerInPixel.x + 10, centerInPixel.y - 10);

            if (isActive)
            {

                if(bHighlightCorner && highlightedCorner >= 0)
                {
                    // if the mouse is over a corner, draw two orange circles around it, to show it is draggable
                    ofSetColor(219, 104, 0, 255);
                    ofEnableAlphaBlending();
                    const ofPoint highlightedCornerInPixel = Util::scalePointToPixel(corners[highlightedCorner]);
                    ofDrawCircle(highlightedCornerInPixel, 5);
                    ofDrawCircle(highlightedCornerInPixel, 20);
                    ofDisableAlphaBlending();
                }

                if(bHighlightCenter)
                {
                    // if the mouse is over the center handle, fill it with a little semi-transparent orange square
                    ofFill();
                    ofEnableAlphaBlending();
                    ofSetColor(219, 104, 0, 120); // semi-transparent orange
                    ofDrawRectangle(centerInPixel.x - 5, centerInPixel.y - 5, 10, 10);
                    ofDisableAlphaBlending();
                    ofNoFill();

                    // draw a move cursor next to the center handle to show it is dragable
                    ofDrawLine(centerInPixel.x - 25, centerInPixel.y, centerInPixel.x - 15, centerInPixel.y);
                    ofDrawLine(centerInPixel.x - 15, centerInPixel.y, centerInPixel.x - 19, centerInPixel.y + 4);
                    ofDrawLine(centerInPixel.x - 15, centerInPixel.y, centerInPixel.x - 19, centerInPixel.y - 4);
                    ofDrawLine(centerInPixel.x - 25, centerInPixel.y, centerInPixel.x - 25, centerInPixel.y - 10);
                    ofDrawLine(centerInPixel.x - 25, centerInPixel.y - 10, centerInPixel.x - 29, centerInPixel.y - 6);
                    ofDrawLine(centerInPixel.x - 25, centerInPixel.y - 10, centerInPixel.x - 21, centerInPixel.y - 6);

                    ofDrawLine(centerInPixel.x - 25, centerInPixel.y, centerInPixel.x - 35, centerInPixel.y);
                    ofDrawLine(centerInPixel.x - 35, centerInPixel.y, centerInPixel.x - 31, centerInPixel.y + 4);
                    ofDrawLine(centerInPixel.x - 35, centerInPixel.y, centerInPixel.x - 31, centerInPixel.y - 4);
                    ofDrawLine(centerInPixel.x - 25, centerInPixel.y, centerInPixel.x - 25, centerInPixel.y + 10);
                    ofDrawLine(centerInPixel.x - 25, centerInPixel.y + 10, centerInPixel.x - 29, centerInPixel.y + 6);
                    ofDrawLine(centerInPixel.x - 25, centerInPixel.y + 10, centerInPixel.x - 21, centerInPixel.y + 6);

                    // enclose the move cursor in a circle
                    ofSetCircleResolution(48);
                    ofDrawCircle(centerInPixel.x - 25, centerInPixel.y, 12);
                    ofSetCircleResolution(40);
                    ofSetHexColor(0x444444);

                }

                if(bHighlightRotation)
                {
                    // if the mouse is over the rotation handle, fill it with semi-transparent orange
                    ofFill();
                    ofEnableAlphaBlending();
                    ofSetColor(219, 104, 0, 120); // semi-transparent orange
                    ofDrawCircle((center.x + 0.1) * ofGetWidth(), centerInPixel.y ,5);
                    ofDisableAlphaBlending();
                    ofSetHexColor(0x444444);
                    ofNoFill();
                }

                ofSetHexColor(0x444444); // grey
                // draw the center handle
                ofDrawRectangle(centerInPixel.x - 5, centerInPixel.y - 5, 10, 10);
                // draw the rotation handle
                ofDrawCircle((center.x + 0.1) * ofGetWidth(), centerInPixel.y, 5);
                ofDrawLine(centerInPixel.x, centerInPixel.y, (center.x + 0.1) * ofGetWidth(), centerInPixel.y);

                // draw a string with the surface number in orange if the quad is the active one
                ofSetColor(219, 104, 0, 255); // solid orange
                ttf.drawString("surface " + ofToString(quadNumber), centerInPixel.x + 8, centerInPixel.y - 12);

            }
            else
            {
                // draw a string with the surface number in white if the quad is not the active one
                ofSetHexColor(0xFFFFFF);// white
                ttf.drawString("surface " + ofToString(quadNumber), centerInPixel.x + 8, centerInPixel.y - 12);
            }
        }
    }
}

//--------------------------------------------------------------
void quad::applyBlendmode()
{
    switch(blendMode)
    {
        case 0:
            // None
            glBlendFunc(GL_ONE, GL_ZERO);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case 1:
            // Normal Alpha-Blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case 2:
            // Additiv (with Alpha)
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case 3:
            // Subtract
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
            break;
        case 4:
            // Multiply
            glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case 5:
            // Screen
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
            glBlendEquation(GL_FUNC_ADD);
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void quad::allocateFbo(int w, int h)
{
    settings.internalformat = GL_RGBA;
    settings.numSamples = 4;
    settings.useDepth = false;
    settings.useStencil = false;
    settings.width = w;
    settings.height = h;
    quadFbo.allocate(settings);
    maskFbo.allocate(settings);
    targetFbo.allocate(settings);
}

//---------------------------------------------------------------
// adds a new point to the mask point vector
// the given point has to be in non-normalized window pixel coordinates
void quad::maskAddPoint(ofPoint point)
{
    const ofPoint warpedPoint = getWarpedPoint(point);
    const ofPoint normalizedPoint(warpedPoint.x / ofGetWidth(), warpedPoint.y / ofGetHeight());

    if((m_maskPoints.empty()) || (m_maskPoints.size() == 1)) {
        m_maskPoints.push_back(normalizedPoint);
    } else {
        list<ofVec3f> pts;

        float min_diff = RAND_MAX;
        for(unsigned int i=0; i < m_maskPoints.size(); i++)
        {
            float diff = (m_maskPoints.at(i) - normalizedPoint).lengthSquared();
            if(diff < min_diff)
            {
                pts.push_front(m_maskPoints.at(i));
                min_diff = diff;
            } else {
                pts.push_back(m_maskPoints.at(i));
            }
        }

        ofVec3f min = pts.front();

        vector<ofVec3f>::iterator itr;
        for(itr = m_maskPoints.begin();itr != m_maskPoints.end(); itr++)
        {
            if(*itr == min) {
                break;

            }
        }
        m_maskPoints.insert(itr,normalizedPoint);
    }
}

//---------------------------------------------------------------
ofPoint quad::getWarpedPoint(ofPoint point)
{
    return findWarpedPoint(src, dst, point);
}

//---------------------------------------------------------------
void quad::loadImageFromFile(string imgName, string imgPath)
{
    ofFile image(imgPath);
    imgBg = true;
    img.load(image);
    bgImg = imgPath;
    loadedImg = imgName;
}

//---------------------------------------------------------------
void quad::loadVideoFromFile(string videoName, string videoPath)
{
    //ofFile movie(videoPath);
    videoBg = true;
    bgVideo = videoPath;
    loadedVideo = videoName;
    if (video.isLoaded())
    {
        video.closeMovie();
    }
    video.load(videoPath);
    videoWidth = video.getWidth();
    videoHeight = video.getHeight();

    video.play();
    loadedVideo = videoName;
}

//---------------------------------------------------------------
// a func for reading a dir content to a vector of strings
int quad::getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL)
    {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

//---------------------------------------------------------------
void quad::setupCamera()
{
    camWidth = cams[camNumber].getWidth();
    camHeight = cams[camNumber].getHeight();

}

//---------------------------------------------------------------
// Bezier helpers -----------------------------------
// Bezier setup -------------------------------------
void quad::bezierSurfaceSetup()
{
    float tmp_bezierPoints[4][4][3] =
    {
        {   {0, 0, 0},          {0.333, 0, 0},    {0.667, 0, 0},    {1.0, 0, 0}    },
        {   {0, 0.333, 0},        {0.333, 0.333, 0},  {0.667, 0.333, 0},  {1.0, 0.333, 0}  },
        {   {0, 0.667, 0},        {0.333, 0.667, 0},  {0.667, 0.667, 0},  {1.0, 0.667, 0}  },
        {   {0, 1.0, 0},        {0.333, 1.0, 0},  {0.667, 1.0, 0},  {1.0, 1.0, 0}  }
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 3; ++k) {
                bezierPoints[i][j][k] = tmp_bezierPoints[i][j][k];
            }
        }
    }

    //This sets up my Bezier Surface
    float tmp_ctrlPoints[4][4][3] = {
        {   {bezierPoints[0][0][0]*ofGetWidth(), bezierPoints[0][0][1]*ofGetHeight(), 0}, {bezierPoints[0][1][0]*ofGetWidth(), bezierPoints[0][1][1]*ofGetHeight(), 0}, {bezierPoints[0][2][0]*ofGetWidth(), bezierPoints[0][2][1]*ofGetHeight(), 0}, {bezierPoints[0][3][0]*ofGetWidth(), bezierPoints[0][3][1]*ofGetHeight(), 0} },
        {   {bezierPoints[1][0][0]*ofGetWidth(), bezierPoints[1][0][1]*ofGetHeight(), 0}, {bezierPoints[1][1][0]*ofGetWidth(), bezierPoints[1][1][1]*ofGetHeight(), 0}, {bezierPoints[1][2][0]*ofGetWidth(), bezierPoints[1][2][1]*ofGetHeight(), 0}, {bezierPoints[1][3][0]*ofGetWidth(), bezierPoints[1][3][1]*ofGetHeight(), 0}  },
        {   {bezierPoints[2][0][0]*ofGetWidth(), bezierPoints[2][0][1]*ofGetHeight(), 0}, {bezierPoints[2][1][0]*ofGetWidth(), bezierPoints[2][1][1]*ofGetHeight(), 0}, {bezierPoints[2][2][0]*ofGetWidth(), bezierPoints[2][2][1]*ofGetHeight(), 0}, {bezierPoints[2][3][0]*ofGetWidth(), bezierPoints[2][3][1]*ofGetHeight(), 0}  },
        {   {bezierPoints[3][0][0]*ofGetWidth(), bezierPoints[3][0][1]*ofGetHeight(), 0}, {bezierPoints[3][1][0]*ofGetWidth(), bezierPoints[3][1][1]*ofGetHeight(), 0}, {bezierPoints[3][2][0]*ofGetWidth(), bezierPoints[3][2][1]*ofGetHeight(), 0}, {bezierPoints[3][3][0]*ofGetWidth(), bezierPoints[3][3][1]*ofGetHeight(), 0}  }
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 3; ++k) {
                bezierCtrlPoints[i][j][k] = tmp_ctrlPoints[i][j][k];
            }
        }
    }

    //This sets up my Texture Surface
    GLfloat texpts [2][2][2] =
    {
        { {0, 0}, {1, 0} }, { {0, 1}, {1, 1} }
    };

    // enable depth test, so we only see the front
    glEnable(GL_DEPTH_TEST);
    //set up bezier surface with a 4th order evaluator
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &bezierCtrlPoints[0][0][0]);
    //set up texture map for bezier surface
    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts[0][0][0]);
    glEnable(GL_MAP2_TEXTURE_COORD_2);
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_AUTO_NORMAL);
    glMapGrid2f(20, 0, 1, 20, 0, 1);
    glShadeModel(GL_FLAT);
}

//---------------------------------------------------------------
void quad::bezierSurfaceUpdate()
{
    // TODO: to optimize this try to limit recalculation to cases when it's really needed
   float tmp_ctrlPoints[4][4][3]  =
        {
            {   {bezierPoints[0][0][0]*ofGetWidth(), bezierPoints[0][0][1]*ofGetHeight(), 0}, {bezierPoints[0][1][0]*ofGetWidth(), bezierPoints[0][1][1]*ofGetHeight(), 0}, {bezierPoints[0][2][0]*ofGetWidth(), bezierPoints[0][2][1]*ofGetHeight(), 0}, {bezierPoints[0][3][0]*ofGetWidth(), bezierPoints[0][3][1]*ofGetHeight(), 0} },
            {   {bezierPoints[1][0][0]*ofGetWidth(), bezierPoints[1][0][1]*ofGetHeight(), 0}, {bezierPoints[1][1][0]*ofGetWidth(), bezierPoints[1][1][1]*ofGetHeight(), 0}, {bezierPoints[1][2][0]*ofGetWidth(), bezierPoints[1][2][1]*ofGetHeight(), 0}, {bezierPoints[1][3][0]*ofGetWidth(), bezierPoints[1][3][1]*ofGetHeight(), 0}  },
            {   {bezierPoints[2][0][0]*ofGetWidth(), bezierPoints[2][0][1]*ofGetHeight(), 0}, {bezierPoints[2][1][0]*ofGetWidth(), bezierPoints[2][1][1]*ofGetHeight(), 0}, {bezierPoints[2][2][0]*ofGetWidth(), bezierPoints[2][2][1]*ofGetHeight(), 0}, {bezierPoints[2][3][0]*ofGetWidth(), bezierPoints[2][3][1]*ofGetHeight(), 0}  },
            {   {bezierPoints[3][0][0]*ofGetWidth(), bezierPoints[3][0][1]*ofGetHeight(), 0}, {bezierPoints[3][1][0]*ofGetWidth(), bezierPoints[3][1][1]*ofGetHeight(), 0}, {bezierPoints[3][2][0]*ofGetWidth(), bezierPoints[3][2][1]*ofGetHeight(), 0}, {bezierPoints[3][3][0]*ofGetWidth(), bezierPoints[3][3][1]*ofGetHeight(), 0}  }
        };

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 3; ++k) {
                    bezierCtrlPoints[i][j][k] = tmp_ctrlPoints[i][j][k];
                }
            }
        }

    if(bBezier)
    {
        glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &bezierCtrlPoints[0][0][0]);
        GLfloat texpts [2][2][2] =
        {
            { {0, 0}, {1, 0} }, { {0, 1}, {1, 1} }
        };
        glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts[0][0][0]);
        glEnable(GL_MAP2_TEXTURE_COORD_2);
        glEnable(GL_MAP2_VERTEX_3);
        glEnable(GL_AUTO_NORMAL);
        glMapGrid2f(20, 0, 1, 20, 0, 1);
        glShadeModel(GL_FLAT);
    }

}


//---------------------------------------------------------------
// Grid helpers -----------------------------------
// grid setup -------------------------------------
void quad::gridSurfaceSetup()
{

    gridMesh.clearVertices();
    gridMesh.clearIndices();
    gridMesh.clearTexCoords();

    for(int i=0; i<=gridRows; i++)
    {
        vector<vector<float> > row;
        for(int j=0; j<=gridColumns; j++)
        {
            vector<float> column;
            column.push_back((float)(1.0/gridColumns*j));
            column.push_back((float)(1.0/gridRows*i));
            column.push_back(0.0);
            row.push_back(column);
        }
        gridPoints.push_back(row);
    }


    for(int i=0; i<=gridRows; i++)
    {
        for(int j=0; j<=gridColumns; j++)
        {
            gridMesh.addTexCoord(ofVec2f((gridPoints[i][j][0]*ofGetWidth()), (gridPoints[i][j][1]*ofGetHeight())));
        }
    }


    for(int i=0; i<=gridRows; i++)
    {
        for(int j=0; j<=gridColumns; j++)
        {
            gridMesh.addVertex(ofVec3f((gridPoints[i][j][0]*ofGetWidth()), (gridPoints[i][j][1]*ofGetHeight()), (0.0)));
        }
    }

    for(int i=0; i<gridRows; i++)
    {
        for(int j=0; j<gridColumns; j++)
        {

            gridMesh.addIndex((i*(gridColumns+1)+j)); //a
            gridMesh.addIndex((i*(gridColumns+1)+j+1)); //b
            gridMesh.addIndex(((i+1)*(gridColumns+1)+j+1));  //c
            gridMesh.addIndex(((i+1)*(gridColumns+1)+j+1)); //c
            gridMesh.addIndex(((i+1)*(gridColumns+1)+j)); //d
            gridMesh.addIndex((i*(gridColumns+1)+j)); //a

        }
    }
}

//---------------------------------------------------------------
void quad::gridSurfaceUpdate()
{
    // TODO: to optimize this try to limit recalculation to cases when it's really needed
    //This sets up my Grid Surface
    gridMesh.clearVertices();
    gridMesh.clearIndices();

    if(gridPoints.size() != (gridRows+1) || gridPoints[0].size() != (gridColumns+1))
    {
        gridMesh.clearTexCoords();
        gridPoints.clear();
        for(int i=0; i<=gridRows; i++)
        {
            vector<vector<float> > row;
            for(int j=0; j<=gridColumns; j++)
            {
                vector<float> column;
                column.push_back((float)(1.0/gridColumns*j));
                column.push_back((float)(1.0/gridRows*i));
                column.push_back(0.0);
                row.push_back(column);
            }
            gridPoints.push_back(row);
        }
        for(int i=0; i<=gridRows; i++)
        {
            for(int j=0; j<=gridColumns; j++)
            {
                gridMesh.addTexCoord(ofVec2f((gridPoints[i][j][0]*ofGetWidth()), (gridPoints[i][j][1]*ofGetHeight())));
            }
        }

    }

    for(int i=0; i<=gridRows; i++)
    {
        for(int j=0; j<=gridColumns; j++)
        {
            gridMesh.addVertex(ofVec3f((gridPoints[i][j][0]*ofGetWidth()), (gridPoints[i][j][1]*ofGetHeight()), (0.0)));
        }
    }

    for(int i=0; i<gridRows; i++)
    {
        for(int j=0; j<gridColumns; j++)
        {

            gridMesh.addIndex((i*(gridColumns+1)+j)); //a
            gridMesh.addIndex((i*(gridColumns+1)+j+1)); //b
            gridMesh.addIndex(((i+1)*(gridColumns+1)+j+1));  //c
            gridMesh.addIndex(((i+1)*(gridColumns+1)+j+1)); //c
            gridMesh.addIndex(((i+1)*(gridColumns+1)+j)); //d
            gridMesh.addIndex((i*(gridColumns+1)+j)); //a

        }
    }

    //glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, (gridColumns+1), 0, 1, (gridColumns+1)*3, (gridRows+1), &gridCtrlPoints[0]);

}

// Markers -----------------------------------------------------

// Bezier markers ----------------------------------------------
void quad::drawBezierMarkers()
{
    ofSetColor(220,200,0,255);
    ofSetLineWidth(1.5);
    for(unsigned int i = 0; i < 4; i++)
    {
        for(unsigned int j = 0; j < 4; j++)
        {
            ofVec3f punto;
            punto.x = bezierCtrlPoints[i][j][0];
            punto.y = bezierCtrlPoints[i][j][1];
            punto.z = bezierCtrlPoints[i][j][2];
            punto = findWarpedPoint(dst, src, punto);

            if(bHighlightCtrlPoint && highlightedCtrlPointRow == i && highlightedCtrlPointCol == j)
            {
                ofFill();
            }
            ofDrawCircle(punto.x, punto.y, 3.6);
            ofNoFill();
        }
    }
    ofSetLineWidth(1.2);
    ofVec3f puntoA;
    ofVec3f puntoB;
    //
    puntoA.x = bezierCtrlPoints[0][0][0];
    puntoA.y = bezierCtrlPoints[0][0][1];
    puntoA.z = bezierCtrlPoints[0][0][2];
    puntoB.x = bezierCtrlPoints[0][1][0];
    puntoB.y = bezierCtrlPoints[0][1][1];
    puntoB.z = bezierCtrlPoints[0][1][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[0][0][0];
    puntoA.y = bezierCtrlPoints[0][0][1];
    puntoA.z = bezierCtrlPoints[0][0][2];
    puntoB.x = bezierCtrlPoints[1][0][0];
    puntoB.y = bezierCtrlPoints[1][0][1];
    puntoB.z = bezierCtrlPoints[1][0][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[0][0][0];
    puntoA.y = bezierCtrlPoints[0][0][1];
    puntoA.z = bezierCtrlPoints[0][0][2];
    puntoB.x = bezierCtrlPoints[1][1][0];
    puntoB.y = bezierCtrlPoints[1][1][1];
    puntoB.z = bezierCtrlPoints[1][1][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[0][3][0];
    puntoA.y = bezierCtrlPoints[0][3][1];
    puntoA.z = bezierCtrlPoints[0][3][2];
    puntoB.x = bezierCtrlPoints[1][3][0];
    puntoB.y = bezierCtrlPoints[1][3][1];
    puntoB.z = bezierCtrlPoints[1][3][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[0][3][0];
    puntoA.y = bezierCtrlPoints[0][3][1];
    puntoA.z = bezierCtrlPoints[0][3][2];
    puntoB.x = bezierCtrlPoints[0][2][0];
    puntoB.y = bezierCtrlPoints[0][2][1];
    puntoB.z = bezierCtrlPoints[0][2][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[0][3][0];
    puntoA.y = bezierCtrlPoints[0][3][1];
    puntoA.z = bezierCtrlPoints[0][3][2];
    puntoB.x = bezierCtrlPoints[1][2][0];
    puntoB.y = bezierCtrlPoints[1][2][1];
    puntoB.z = bezierCtrlPoints[1][2][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[3][0][0];
    puntoA.y = bezierCtrlPoints[3][0][1];
    puntoA.z = bezierCtrlPoints[3][0][2];
    puntoB.x = bezierCtrlPoints[3][1][0];
    puntoB.y = bezierCtrlPoints[3][1][1];
    puntoB.z = bezierCtrlPoints[3][1][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[3][0][0];
    puntoA.y = bezierCtrlPoints[3][0][1];
    puntoA.z = bezierCtrlPoints[3][0][2];
    puntoB.x = bezierCtrlPoints[2][0][0];
    puntoB.y = bezierCtrlPoints[2][0][1];
    puntoB.z = bezierCtrlPoints[2][0][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[3][0][0];
    puntoA.y = bezierCtrlPoints[3][0][1];
    puntoA.z = bezierCtrlPoints[3][0][2];
    puntoB.x = bezierCtrlPoints[2][1][0];
    puntoB.y = bezierCtrlPoints[2][1][1];
    puntoB.z = bezierCtrlPoints[2][1][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[3][3][0];
    puntoA.y = bezierCtrlPoints[3][3][1];
    puntoA.z = bezierCtrlPoints[3][3][2];
    puntoB.x = bezierCtrlPoints[3][2][0];
    puntoB.y = bezierCtrlPoints[3][2][1];
    puntoB.z = bezierCtrlPoints[3][2][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[3][3][0];
    puntoA.y = bezierCtrlPoints[3][3][1];
    puntoA.z = bezierCtrlPoints[3][3][2];
    puntoB.x = bezierCtrlPoints[2][3][0];
    puntoB.y = bezierCtrlPoints[2][3][1];
    puntoB.z = bezierCtrlPoints[2][3][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[3][3][0];
    puntoA.y = bezierCtrlPoints[3][3][1];
    puntoA.z = bezierCtrlPoints[3][3][2];
    puntoB.x = bezierCtrlPoints[2][2][0];
    puntoB.y = bezierCtrlPoints[2][2][1];
    puntoB.z = bezierCtrlPoints[2][2][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[1][2][0];
    puntoA.y = bezierCtrlPoints[1][2][1];
    puntoA.z = bezierCtrlPoints[1][2][2];
    puntoB.x = bezierCtrlPoints[2][2][0];
    puntoB.y = bezierCtrlPoints[2][2][1];
    puntoB.z = bezierCtrlPoints[2][2][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[1][2][0];
    puntoA.y = bezierCtrlPoints[1][2][1];
    puntoA.z = bezierCtrlPoints[1][2][2];
    puntoB.x = bezierCtrlPoints[1][1][0];
    puntoB.y = bezierCtrlPoints[1][1][1];
    puntoB.z = bezierCtrlPoints[1][1][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[2][1][0];
    puntoA.y = bezierCtrlPoints[2][1][1];
    puntoA.z = bezierCtrlPoints[2][1][2];
    puntoB.x = bezierCtrlPoints[1][1][0];
    puntoB.y = bezierCtrlPoints[1][1][1];
    puntoB.z = bezierCtrlPoints[1][1][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);
    //
    puntoA.x = bezierCtrlPoints[2][1][0];
    puntoA.y = bezierCtrlPoints[2][1][1];
    puntoA.z = bezierCtrlPoints[2][1][2];
    puntoB.x = bezierCtrlPoints[2][2][0];
    puntoB.y = bezierCtrlPoints[2][2][1];
    puntoB.z = bezierCtrlPoints[2][2][2];
    puntoA = findWarpedPoint(dst, src, puntoA);
    puntoB = findWarpedPoint(dst, src, puntoB);
    ofDrawLine(puntoA,puntoB);

}


// Bezier markers ----------------------------------------------
void quad::drawGridMarkers()
{
    ofSetColor(0,200,220,255);
    ofSetLineWidth(1.5);

    for(int i=0; i<=gridRows; i++)
    {
        for(int j=0; j<=gridColumns; j++)
        {
            ofVec3f punto;
            punto.x = gridPoints[i][j][0]*ofGetWidth();
            punto.y = gridPoints[i][j][1]*ofGetHeight();
            punto.z = 0.0;
            punto = findWarpedPoint(dst, src, punto);
            if(bHighlightCtrlPoint && highlightedCtrlPointRow == i && highlightedCtrlPointCol == j)
            {
                ofFill();
            }
            ofDrawCircle(punto.x, punto.y, 3.0);
            ofNoFill();
        }
    }
}

// Mask markers --------------------------------------
void quad::drawMaskMarkers()
{
    if (m_maskPoints.size() > 0)
    {
        // draw the contour polygon the markers form
        ofPolyline contour;
        for(size_t i = 0; i < m_maskPoints.size(); i++)
        {
            const ofPoint scaledPoint = Util::scalePointToPixel(m_maskPoints[i]);
            const ofPoint warpedPoint = findWarpedPoint(dst, src, scaledPoint);
            contour.addVertex(warpedPoint);
        }
        ofSetHexColor(0x444444); // dark-grey
        ofSetLineWidth(1.6);
        ofEnableSmoothing();
        contour.close();
        contour.draw();
        ofDisableSmoothing();

        // draw the marker handles
        for(size_t i = 0; i < m_maskPoints.size(); i++)
        {
            ofSetColor(100, 139, 150, 255); // blueish grey
            ofSetLineWidth(1.0);

            const ofPoint scaledPoint = Util::scalePointToPixel(m_maskPoints[i]);
            const ofPoint warpedPoint = findWarpedPoint(dst, src, scaledPoint);

            // if the mouse is over the handle fill the inner circle
            if(bHighlightMaskPoint && highlightedMaskPoint == i)
            {
                ofFill();
            }
            ofDrawCircle(warpedPoint, 4);
            ofNoFill();
            ofDrawCircle(warpedPoint, 10);
        }
    }
}


#ifdef WITH_KINECT
void quad::setKinect(kinectManager &kinect)
{
    quadKinect = &kinect;
}
#endif

#ifdef WITH_SYPHON
setSyphone(ofxSyphonClient &syphon)
{
    syphClientTex = &syphon;
}
#endif
