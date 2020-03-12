#include "quad.h"
#include "Util.hpp"

#include <errno.h>
#include <string>
#include <sys/types.h>
#include <vector>

quad::quad()
{
    // sets default variables
    reset();
}

//---------------------------------------------------------------
void quad::reset()
{
    // sets default variables
    initialized = false;
    isActive = false;
    isOn = false;
    layer = -1;
    isMaskSetup = false;
    colorBg = false;
    camBg = false;
    imgBg = false;
    videoBg = false;
    videoLoop = true;
    sharedVideoBg = false;
    sharedVideoTiling = false;
    sharedVideoNum = 1;
    sharedVideoId = sharedVideoNum - 1;
    slideshowBg = false;
    bFadeTransitions = false;
    imageFit = false;
    imageKeepAspect = false;
    useGreenscreen = false;

    hue = 0.0f;
    saturation = 0.0f;
    luminance = 0.0f;

    imgHFlip = false;
    imgVFlip = false;

    camNumber = prevCamNumber = 0;

    imgMultX = 1.0;
    imgMultY = 1.0;

    videoSpeed = 1.0;
    previousSpeed = 1.0;
    videoVolume = 0;

    currentSlideId = 0;
    slideshowSpeed = 1.0;
    slideTimer = ofGetElapsedTimef();

    transDuration = 1.0;
    fps = ofGetFrameRate();

    // initialize some colors with solid white
    imgColorize = ofFloatColor(1, 1, 1, 1);

    // greenscreen setup
    colorGreenscreen = ofFloatColor(0, 0, 0, 0);
    thresholdGreenscreen = 0;

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

    bEdgeBlend = false;
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
    bDrawMaskOutline = false;

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

    if (video.isLoaded()) {
        video.stop();
        video.close();
    }
}

//---------------------------------------------------------------
void quad::setup(ofPoint point1, ofPoint point2, ofPoint point3, ofPoint point4, ofShader& _edgeBlendShader, ofShader& quadMaskShader, ofShader& _surfaceShader, ofShader& _crossfadeShader, vector<ofVideoGrabber>& cameras, ofTrueTypeFont& font)
{
    reset();

    isEditMode = true;
    isOn = true;

    edgeBlendShader = &_edgeBlendShader;
    maskShader = &quadMaskShader;
    surfaceShader = &_surfaceShader;
    crossfadeShader = &_crossfadeShader;

    ttf = font;
    cams = cameras;
    if (cams.size() > 0) {
        camAvailable = true;
    } else {
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

    allocateFbo(ofGetWidth(), ofGetHeight());

    // create a 1x1 white texture
    ofPixels texpixels;
    texpixels.allocate(1, 1, OF_PIXELS_RGBA);
    texpixels[0] = 255;
    texpixels[1] = 255;
    texpixels[2] = 255;
    texpixels[3] = 255;
    blank.allocate(texpixels);
    blank.loadData(texpixels);

    //calculates screen ratio factor for window and fullscreen
    screenFactorX = (ofGetWidth() / (float)ofGetScreenWidth());
    screenFactorY = (ofGetHeight() / (float)ofGetScreenHeight());

    initialized = true;
}

//---------------------------------------------------------------
void quad::update()
{
    if (isOn) {
        sharedVideoId = sharedVideoNum - 1;

        if (camAvailable && camNumber != prevCamNumber) {
            prevCamNumber = camNumber;
        }
        //recalculates center of quad
        center = (corners[0] + corners[1] + corners[2] + corners[3]) / 4;

        // video --------------------------------------------------------------------
        // loads video
        if (videoBg) {
            if (video.isPaused()) {
                video.setPaused(false);
            }
            video.setVolume(videoVolume);
            // check for looping config parameter of video and sets loopstate - OF_LOOP_NORMAL = cycles / OF_LOOP_NONE = stops at the end
            if (videoLoop) {
                video.setLoopState(OF_LOOP_NORMAL);
            } else {
                video.setLoopState(OF_LOOP_NONE);
            }

            if (video.isLoaded()) {
                video.update();
            }

            // changevideo speed
            if (previousSpeed != videoSpeed) {
                video.setSpeed(videoSpeed);
                previousSpeed = videoSpeed;
            }
        } else {
            if (video.isLoaded()) {
                video.setPaused(true);
            }
        }

        // slideshow -----------------------------------------------------------------
        if (slideshowBg) {
            // put it to off while loading images
            slideshowBg = false;
            // if a different slideshow has been chosen in gui we do load its images
            if ((slideshowName != loadedSlideshow) && slideshowName != "") {
                // we exclude "." and ".." directories if present
                if (slideshowName != "." && slideshowName != "..") {
                    // we scan the img dir for images
                    ofDirectory slidesDir(slideshowName);
                    slidesDir.allowExt("png");
                    slidesDir.allowExt("tiff");
                    slidesDir.allowExt("jpg");
                    slidesDir.allowExt("bmp");
                    slidesDir.listDir();
                    slides.clear();

                    for (int i = 0; i < slidesDir.size(); i++) {
                        ofImage slide;
                        bool bValid = slide.load(slidesDir.getPath(i));
                        if (bValid) {
                            slides.push_back(slide);
                            ofLogNotice() << "Loaded: " << slidesDir.getPath(i);
                        }
                    }

                    loadedSlideshow = slideshowName;
                    currentSlideId = 0;
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
        if (kinectBg) {
            kinectThreshImage.clear();
            kinectContourImage.clear();
            kinectThreshImage.allocate(quadKinect->kinect.width, quadKinect->kinect.height);
            kinectContourImage.allocate(quadKinect->kinect.width, quadKinect->kinect.height);
            kinectThreshImage = quadKinect->getThresholdDepthImage(nearDepthTh, farDepthTh, kinectBlur);
            kinectContourImage = kinectThreshImage;
            kinectContourFinder.findContours(kinectContourImage, (quadKinect->kinect.width * quadKinect->kinect.height) * kinectContourMin, (quadKinect->kinect.width * quadKinect->kinect.height) * kinectContourMax, 20, true);
            // clear kinect path if any
            kinectPath.clear();
            kinectPath.setFilled(true);
            for (int i = 0; i < (int)kinectContourFinder.blobs.size(); i++) {
                ofPolyline poly(kinectContourFinder.blobs[i].pts);
                poly.close();
                poly.simplify(kinectContourSimplify);
                ofPolyline polySmoothed = poly.getSmoothed(kinectContourSmooth);
                //polySmoothed.close();
                vector<ofPoint> points = polySmoothed.getVertices();

                for (size_t j = 0; j < points.size(); j++) {
                    if (kinectContourCurved) {
                        kinectPath.curveTo(points[j]);
                    } else {
                        kinectPath.lineTo(points[j]);
                    }
                }
                kinectPath.close();
            }
        }
#endif

        //we set matrix to the default - 0 translation
        //and 1.0 scale for x y z and w
        for (int i = 0; i < 16; i++) {
            if (i % 5 != 0)
                matrix[i] = 0.0;
            else
                matrix[i] = 1.0;
        }

        //we set the warp coordinates
        //source coordinates as the dimensions of our window
        src[0] = ofPoint(0, 0);
        src[1] = ofPoint(ofGetWidth(), 0);
        src[2] = ofPoint(ofGetWidth(), ofGetHeight());
        src[3] = ofPoint(0, ofGetHeight());

        //corners are in 0.0 - 1.0 range
        //so we scale up so that they are at the window's scale
        for (int i = 0; i < 4; i++) {
            dst[i].x = corners[i].x * (float)ofGetWidth();
            dst[i].y = corners[i].y * (float)ofGetHeight();
        }
    }
}

//---------------------------------------------------------------
void quad::draw(vector<ofVideoPlayer>& sharedVideos)
{
    if (isOn) {
        // recalculates bezier surface
        if (bBezier) {
            bezierSurfaceUpdate();
        }
        if (bGrid) {
            gridSurfaceUpdate(false);
        }

        quadFbo.begin();
        ofClear(0.0, 0.0, 0.0, 0.0);
        ofEnableAlphaBlending();

        // -- NOW LETS DRAW!!!!!!
        drawSurface(sharedVideos);

        ofDisableAlphaBlending();
        quadFbo.end();

        //mask on mask FBO
        maskFbo.begin();
        ofPushStyle();
        ofClear(0.0, 0.0, 0.0, 0.0);
        ofEnableAlphaBlending();
        ofFill();
        ofEnableSmoothing();

        // crop rectangular mask
        ofSetColor(255, 255, 255);
        ofDrawRectangle(0, 0, ofGetWidth(), crop[0] * ofGetHeight());
        ofDrawRectangle(ofGetWidth() * (1 - crop[1]), 0, ofGetWidth() * crop[1], ofGetHeight());
        ofDrawRectangle(0, ofGetHeight() * (1 - crop[2]), ofGetWidth(), crop[2] * ofGetHeight());
        ofDrawRectangle(0, 0, ofGetWidth() * crop[3], ofGetHeight());

        // crop circular mask
        if (circularCrop[2] > 0.0) {
            ofSetCircleResolution(64);
            ofDrawCircle(circularCrop[0] * ofGetWidth(), circularCrop[1] * ofGetHeight(), circularCrop[2] * ofGetWidth());
            ofSetCircleResolution(22);
        }

        // draw the user-defined mask on the mask FBO
        if (m_maskPoints.size() > 0) {
            ofSetColor(255, 255, 255); // white
            ofBeginShape();
            for (size_t i = 0; i < m_maskPoints.size(); i++) {
                ofPoint scaledPoint = Util::scalePointToPixel(m_maskPoints[i]);
                ofVertex(scaledPoint);
            }
            ofEndShape(true);
        }
#ifdef WITH_KINECT
        if (kinectBg && kinectMask) {
            ofSetColor(255, 255, 255);
            if (getKinectContours) {
                ofPushStyle();
                glPushMatrix();
                glScalef(kinectMultX, kinectMultY, 0.0);
                // ----------- draw the kinect path made of detected blobs
                ofColor pathColor(255, 255, 255, 255);
                kinectPath.setFillColor(pathColor);
                kinectPath.draw();
                glPopMatrix();
                ofPopStyle();
            } else {
                kinectThreshImage.draw(0, 0, quadKinect->grayImage.getWidth() * kinectMultX, quadKinect->grayImage.getHeight() * kinectMultY);
            }
        }
#endif
        ofDisableSmoothing();
        ofNoFill();
        ofDisableAlphaBlending();
        ofPopStyle();
        maskFbo.end();

        // save actual GL coordinates
        ofPushMatrix();
        // find transformation matrix
        findHomography(src, dst, matrix);
        //finally lets multiply our matrix
        glMultMatrixf(matrix);

        if (bEdgeBlend) {
            if (quadFbo.getWidth() > 0) {
                ofEnableAlphaBlending();
                edgeBlendShader->begin();
                edgeBlendShader->setUniformTexture("tex", quadFbo.getTexture(), 0);
                edgeBlendShader->setUniform1f("exponent", edgeBlendExponent);
                edgeBlendShader->setUniform1f("userGamma", edgeBlendGamma);
                edgeBlendShader->setUniform3f("userLuminance", edgeBlendLuminance, edgeBlendLuminance, edgeBlendLuminance);
                edgeBlendShader->setUniform4f("amount", edgeBlendAmountSin, edgeBlendAmountTop, edgeBlendAmountDx, edgeBlendAmountBottom);
                edgeBlendShader->setUniform1i("w", ofGetWidth());
                edgeBlendShader->setUniform1i("h", ofGetHeight());

                ofSetColor(255, 255, 255);
                if (bBlendModes) {
                    glEnable(GL_BLEND);
                    applyBlendmode();
                }
                if (!bDeform) {
                    quadFbo.draw(0 + quadDispX, 0 + quadDispY, quadW, quadH);
                    edgeBlendShader->end();
                } else {
                    targetFbo.begin();
                    ofClear(0.0, 0.0, 0.0, 0.0);
                    quadFbo.draw(0 + quadDispX, 0 + quadDispY, quadW, quadH);
                    edgeBlendShader->end();
                    targetFbo.end();

                    drawDeformation(targetFbo.getTexture(), true);
                }

                if (bBlendModes) {
                    glDisable(GL_BLEND);
                }
                ofDisableAlphaBlending();
            }
        } else {
            if (bMask) {
                if (quadFbo.getWidth() > 0) {
                    ofEnableAlphaBlending();
                    if (maskInvert) {
                        maskMode = 1;
                    } else {
                        maskMode = 0;
                    }
                    maskShader->begin();
                    maskShader->setUniformTexture("tex", quadFbo.getTexture(), 0);
                    maskShader->setUniformTexture("mask", maskFbo.getTexture(), 1);
                    maskShader->setUniform1i("mode", maskMode);

                    ofSetColor(255, 255, 255);
                    if (bBlendModes) {
                        glEnable(GL_BLEND);
                        applyBlendmode();
                    }
                    if (!bDeform) {
                        quadFbo.draw(0 + quadDispX, 0 + quadDispY, quadW, quadH);
                        maskShader->end();
                    } else {
                        targetFbo.begin();
                        ofClear(0.0, 0.0, 0.0, 0.0);
                        quadFbo.draw(0 + quadDispX, 0 + quadDispY, quadW, quadH);
                        maskShader->end();
                        targetFbo.end();

                        drawDeformation(targetFbo.getTexture(), true);
                    }

                    if (bBlendModes) {
                        glDisable(GL_BLEND);
                    }
                    ofDisableAlphaBlending();
                }
            } else {
                if (quadFbo.getWidth() > 0) {
                    ofEnableAlphaBlending();

                    ofSetColor(255, 255, 255);
                    if (bBlendModes) {
                        glEnable(GL_BLEND);
                        applyBlendmode();
                    }

                    if (!bDeform) {
                        quadFbo.draw(quadDispX, quadDispY, quadW, quadH);
                    } else {
                        drawDeformation(quadFbo.getTexture(), false);
                    }

                    if (bBlendModes) {
                        glDisable(GL_BLEND);
                    }
                    ofDisableAlphaBlending();
                }
            }
        }

        // draw mask hatch if we are in mask setup mode
        if (isActive && isMaskSetup) {
            ofEnableAlphaBlending();
            //set ofColor to red with alpha
            //ofSetColor(255,100,100,180);
            ofSetColor(100, 139, 150, 160);
            maskFbo.draw(0 + quadDispX, 0 + quadDispY, quadW, quadH);
            ofDisableAlphaBlending();
        }

        // in Edit mode draw the border of the quad and if it is active draw the helper grid
        if (isEditMode) {
            ofPushStyle();
            ofNoFill();
            ofSetLineWidth(1.0);
            ofEnableSmoothing();
            if (isActive) {
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
            } else {
                // if the quad is not active draw a grey border
                ofSetHexColor(0x666666); // light-grey
                ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            }
            ofDisableSmoothing();
            ofPopStyle();
        }

        ofPopMatrix();

        if (isActive) {
            // draws markers for bezier deform setup
            if (isBezierSetup) {
                if (bBezier) {
                    drawBezierMarkers();
                } else if (bGrid) {
                    drawGridMarkers();
                }
            }
            // draws mask markers and contour in mask-setup mode
            if (isMaskSetup) {
                drawMaskMarkers();
            }
        }

        if (bDrawMaskOutline) {
            if (m_maskPoints.size() > 0) {
                ofPushStyle();
                ofPolyline contour;
                for (size_t i = 0; i < m_maskPoints.size(); i++) {
                    const ofPoint scaledPoint = Util::scalePointToPixel(m_maskPoints[i]);
                    const ofPoint warpedPoint = findWarpedPoint(dst, src, scaledPoint);
                    contour.addVertex(warpedPoint);
                }
                ofSetHexColor(0x444444); // dark-grey
                ofSetLineWidth(3);
                ofEnableSmoothing();
                contour.close();
                contour.draw();
                ofDisableSmoothing();
                ofPopStyle();
            }
        }

        if (isEditMode) {
            // transform the normalized coordinates into window pixel coordinates
            const ofPoint centerInPixel = Util::scalePointToPixel(center);

            // draw the a string with the surface number a little offset in black, as a drop shadow
            ofSetHexColor(0x000000); // black
            ttf.drawString("surface " + ofToString(quadNumber), centerInPixel.x + 10, centerInPixel.y - 10);

            if (isActive) {

                if (bHighlightCorner && highlightedCorner >= 0) {
                    // if the mouse is over a corner, draw two orange circles around it, to show it is draggable
                    ofSetColor(219, 104, 0, 255);
                    ofEnableAlphaBlending();
                    const ofPoint highlightedCornerInPixel = Util::scalePointToPixel(corners[highlightedCorner]);
                    ofDrawCircle(highlightedCornerInPixel, 5);
                    ofDrawCircle(highlightedCornerInPixel, 20);
                    ofDisableAlphaBlending();
                }

                if (bHighlightCenter) {
                    // if the mouse is over the center handle, fill it with a little semi-transparent orange square
                    ofPushStyle();
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
                    ofPopStyle();
                }

                if (bHighlightRotation) {
                    // if the mouse is over the rotation handle, fill it with semi-transparent orange
                    ofPushStyle();
                    ofFill();
                    ofEnableAlphaBlending();
                    ofSetColor(219, 104, 0, 120); // semi-transparent orange
                    ofDrawCircle((center.x + 0.1) * ofGetWidth(), centerInPixel.y, 5);
                    ofDisableAlphaBlending();
                    ofSetHexColor(0x444444);
                    ofNoFill();
                    ofPopStyle();
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

            } else {
                // draw a string with the surface number in white if the quad is not the active one
                ofSetHexColor(0xFFFFFF); // white
                ttf.drawString("surface " + ofToString(quadNumber), centerInPixel.x + 8, centerInPixel.y - 12);
            }
        }
    }
}

//--------------------------------------------------------------
void quad::drawDeformation(ofTexture& tex, bool bAlphaFix)
{
    if (bBezier) {
        tex.bind();

        glMatrixMode(GL_TEXTURE);
        glPushMatrix(); //to scale the texture
        glLoadIdentity();

        ofTextureData texData = tex.getTextureData();
        if (texData.textureTarget == GL_TEXTURE_RECTANGLE_ARB) {
            glScalef(tex.getWidth(), tex.getHeight(), 1.0f);
        } else {
            glScalef(tex.getWidth() / texData.tex_w, tex.getHeight() / texData.tex_h, 1.0f);
        }
        glMatrixMode(GL_MODELVIEW);
        //        glEnable(GL_MAP2_VERTEX_3);
        //        glEnable(GL_AUTO_NORMAL);

        if (bAlphaFix) {
            // this tries to prevent the double alpha problem
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        }

        //draw the bezier shape
        glEvalMesh2(GL_FILL, 0, 20, 0, 20);

        if (bAlphaFix) {
            glDisable(GL_BLEND);
        }

        //        glDisable(GL_MAP2_VERTEX_3);
        //        glDisable(GL_AUTO_NORMAL);

        tex.unbind();
        glMatrixMode(GL_TEXTURE);
        glPopMatrix(); // texture scale pop matrix
        glMatrixMode(GL_MODELVIEW);
    } else if (bGrid) {
        tex.bind();
        gridMesh.drawFaces();
        tex.unbind();
        if (isActive && isBezierSetup) {
            ofPushStyle();
            ofSetColor(255, 255, 255, 200);
            gridMesh.drawWireframe();
            ofPopStyle();
        }
    }
}

//--------------------------------------------------------------
void quad::drawSurface(vector<ofVideoPlayer>& sharedVideos)
{
    if (isValidContent(sharedVideos)) {

        bool bUseSurfaceShader = true;
        float srcWidth = 0;
        float srcHeight = 0;
        ofTexture imageTex;
        ofTexture imageTex2;
        ofVec2f ratio = ofVec2f(1, 1);
        float fade = 0.0f;

        if (colorBg) {
            srcWidth = ofGetScreenWidth();
            srcHeight = ofGetScreenHeight();
            imageTex = imageTex2 = blank;
        } else if (imgBg && (img.getWidth() > 0)) {
            srcWidth = img.getWidth();
            srcHeight = img.getHeight();
            imageTex = imageTex2 = img.getTexture();
        } else if (camAvailable && camBg && cams[camNumber].getWidth() > 0) {
            srcWidth = cams[camNumber].getWidth();
            srcHeight = cams[camNumber].getHeight();
            imageTex = imageTex2 = cams[camNumber].getTexture();
        } else if (videoBg && video.isLoaded()) {
            srcWidth = video.getWidth();
            srcHeight = video.getHeight();
            imageTex = imageTex2 = video.getTexture();

        } else if (sharedVideoBg && sharedVideos[sharedVideoId].isLoaded()) {
            srcWidth = sharedVideos[sharedVideoId].getWidth();
            srcHeight = sharedVideos[sharedVideoId].getHeight();
            imageTex = imageTex2 = sharedVideos[sharedVideoId].getTexture();
        } else if (slideshowBg && (slides.size() > 0)) {
            // if we reached the end of slides vector, it loops back to first slide
            if (currentSlideId >= slides.size()) {
                currentSlideId = 0;
            }
            ofSetColor(imgColorize.r * 255 * timelineRed, imgColorize.g * 255 * timelineGreen, imgColorize.b * 255 * timelineBlue, imgColorize.a * 255 * timelineAlpha);
            srcWidth = slides[currentSlideId].getWidth();
            srcHeight = slides[currentSlideId].getHeight();
            imageTex = imageTex2 = slides[currentSlideId].getTexture();

            int nextSlideId = 0;
            if (bFadeTransitions && (slides.size() > 1)) {
                if ((currentSlideId + 1) >= slides.size()) {
                    nextSlideId = 0;
                } else {
                    nextSlideId = currentSlideId + 1;
                }
                imageTex2 = slides[nextSlideId].getTexture();
                fade = (float)slideTimer / (float)slideFramesDuration;
            }
            ratio = ofVec2f(slides[nextSlideId].getWidth() / srcWidth, slides[nextSlideId].getHeight() / srcHeight);
        }

        if (imageFit) {
            float multX = 1.0;
            float multY = 1.0;
            float fitX = ofGetWidth() / srcWidth;
            float fitY = ofGetHeight() / srcHeight;
            if (imageKeepAspect) {
                // we calculate the factor for fitting the image in quad respecting img aspect ratio
                if (fitX >= fitY) {
                    multX = fitY;
                    multY = fitY;
                } else {
                    multX = fitX;
                    multY = fitX;
                }
            } else {
                // this is for stretching image to whole quad size
                multX = fitX;
                multY = fitY;
            }

            if (imgHFlip || imgVFlip) {
                glPushMatrix();
                if (imgHFlip && !imgVFlip) {
                    ofTranslate(srcWidth * multX, 0);
                    glScalef(-1, 1, 1);
                } else if (imgVFlip && !imgHFlip) {
                    ofTranslate(0, srcHeight * multY);
                    glScalef(1, -1, 1);
                } else {
                    ofTranslate(srcWidth * multX, srcHeight * multY);
                    glScalef(-1, -1, 1);
                }
            }

            if (bUseSurfaceShader) {
                surfaceShader->begin();
                surfaceShader->setUniformTexture("tex", imageTex, 0);
                surfaceShader->setUniformTexture("tex2", imageTex2, 1);
                surfaceShader->setUniform1f("crossfade", fade);
                surfaceShader->setUniform2f("ratio", ratio);
                surfaceShader->setUniform1f("hue", hue);
                surfaceShader->setUniform1f("sat", saturation);
                surfaceShader->setUniform1f("luminance", luminance);
                surfaceShader->setUniform1f("tintR", imgColorize.r * timelineRed);
                surfaceShader->setUniform1f("tintG", imgColorize.g * timelineGreen);
                surfaceShader->setUniform1f("tintB", imgColorize.b * timelineBlue);
                surfaceShader->setUniform1f("tintA", imgColorize.a * timelineAlpha);
                surfaceShader->setUniform1f("greenscreenR", colorGreenscreen.r);
                surfaceShader->setUniform1f("greenscreenG", colorGreenscreen.g);
                surfaceShader->setUniform1f("greenscreenB", colorGreenscreen.b);
                surfaceShader->setUniform1f("greenscreenT", thresholdGreenscreen / 255.0f);

                drawContent(srcWidth * multX, srcHeight * multY, sharedVideos);

                surfaceShader->end();
            } else {
                drawContent(srcWidth * multX, srcHeight * multY, sharedVideos);
            }

        } else {
            if (imgHFlip || imgVFlip) {
                glPushMatrix();
                if (imgHFlip && !imgVFlip) {
                    ofTranslate(srcWidth * imgMultX * screenFactorX, 0);
                    glScalef(-1, 1, 1);
                } else if (imgVFlip && !imgHFlip) {
                    ofTranslate(0, srcHeight * imgMultY * screenFactorY);
                    glScalef(1, -1, 1);
                } else {
                    ofTranslate(srcWidth * imgMultX * screenFactorX, srcHeight * imgMultY * screenFactorY);
                    glScalef(-1, -1, 1);
                }
            }

            if (bUseSurfaceShader) {
                surfaceShader->begin();
                surfaceShader->setUniformTexture("tex", imageTex, 0);
                surfaceShader->setUniformTexture("tex2", imageTex2, 1);
                surfaceShader->setUniform1f("crossfade", fade);
                surfaceShader->setUniform2f("ratio", ratio);
                surfaceShader->setUniform1f("hue", hue);
                surfaceShader->setUniform1f("sat", saturation);
                surfaceShader->setUniform1f("luminance", luminance);
                surfaceShader->setUniform1f("tintR", imgColorize.r * timelineRed);
                surfaceShader->setUniform1f("tintG", imgColorize.g * timelineGreen);
                surfaceShader->setUniform1f("tintB", imgColorize.b * timelineBlue);
                surfaceShader->setUniform1f("tintA", imgColorize.a * timelineAlpha);
                surfaceShader->setUniform1f("greenscreenR", colorGreenscreen.r);
                surfaceShader->setUniform1f("greenscreenG", colorGreenscreen.g);
                surfaceShader->setUniform1f("greenscreenB", colorGreenscreen.b);
                surfaceShader->setUniform1f("greenscreenT", thresholdGreenscreen / 255.0f);

                drawContent(srcWidth * imgMultX * screenFactorX, srcHeight * imgMultY * screenFactorY, sharedVideos);

                surfaceShader->end();
            } else {
                drawContent(srcWidth * imgMultX * screenFactorX, srcHeight * imgMultY * screenFactorY, sharedVideos);
            }
        }

        if (imgHFlip || imgVFlip) {
            glPopMatrix();
        }
    }

// kinect stuff ------------------------------------------------------------------------------
#ifdef WITH_KINECT
    if (kinectBg && kinectImg) {
        ofSetColor(kinectColorize.r * 255 * timelineRed, kinectColorize.g * 255 * timelineGreen, kinectColorize.b * 255 * timelineBlue, kinectColorize.a * 255 * timelineAlpha);
        //quadKinect->grayImage.draw(0,0,quadKinect->grayImage.getWidth()*kinectMultX,quadKinect->grayImage.getHeight()*kinectMultY);
        if (getKinectContours) {
            ofPushStyle();
            glPushMatrix();
            glScalef(kinectMultX, kinectMultY, 0.0);
            // ----------- draw the kinect path made of detected blobs
            ofColor pathColor(kinectColorize.r * 255 * timelineRed, kinectColorize.g * 255 * timelineGreen, kinectColorize.b * 255 * timelineBlue, kinectColorize.a * 255 * timelineAlpha);
            kinectPath.setFillColor(pathColor);
            kinectPath.draw();
            glPopMatrix();
            ofPopStyle();
        } else if (getKinectGrayImage) {
            quadKinect->grayImage.draw(0, 0, quadKinect->grayImage.getWidth() * kinectMultX, quadKinect->grayImage.getHeight() * kinectMultY);
        } else {
            kinectThreshImage.draw(0, 0, quadKinect->grayImage.getWidth() * kinectMultX, quadKinect->grayImage.getHeight() * kinectMultY);
        }
    }
#endif

#ifdef WITH_SYPHON
    // syphon stuff
    if (bSyphon) {
        ofSetColor(255, 255, 255);
        syphClientTex->draw(syphonPosX, syphonPosY, syphonScaleX * syphClientTex->getWidth(), syphonScaleY * syphClientTex->getHeight());
    }
#endif
}

//--------------------------------------------------------------
bool quad::isValidContent(vector<ofVideoPlayer>& sharedVideos)
{
    if (colorBg)
        return true;
    else if (imgBg && img.getWidth() > 0)
        return true;
    else if (camAvailable && camBg && cams[camNumber].getWidth() > 0)
        return true;
    else if (videoBg && video.isLoaded())
        return true;
    else if (sharedVideoBg && sharedVideos[sharedVideoId].isLoaded())
        return true;
    else if (slideshowBg && slides.size() > 0)
        return true;

    return false;
}

//--------------------------------------------------------------
void quad::drawContent(float w, float h, vector<ofVideoPlayer>& sharedVideos)
{
    if (colorBg) {
        blank.draw(0, 0, w, h);
    } else if (imgBg && img.getWidth() > 0) {
        img.draw(0, 0, w, h);
    } else if (camAvailable && camBg && cams[camNumber].getWidth() > 0) {
        cams[camNumber].getTexture().draw(0, 0, w, h);
    } else if (videoBg && video.isLoaded()) {
        video.draw(0, 0, w, h);
    } else if (sharedVideoBg && sharedVideos[sharedVideoId].isLoaded()) {
        float x1 = ofGetWidth();
        float y1 = ofGetHeight();
        float x2 = 0.0f;
        float y2 = 0.0f;
        for (int i = 0; i < 4; i++) {
            if (corners[i].x < x1) {
                x1 = corners[i].x;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (corners[i].y < y1) {
                y1 = corners[i].y;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (corners[i].x > x2) {
                x2 = corners[i].x;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (corners[i].y > y2) {
                y2 = corners[i].y;
            }
        }
        if (sharedVideoTiling) {
            float w1 = sharedVideos[sharedVideoId].getWidth();
            float h1 = sharedVideos[sharedVideoId].getHeight();
            sharedVideos[sharedVideoId].getTexture().drawSubsection(0, 0, ofGetWidth(), ofGetHeight(), x1 * w1, y1 * h1, (x2 - x1) * w1, (y2 - y1) * h1);
        } else {
            sharedVideos[sharedVideoId].draw(0, 0, w, h);
        }
    } else if (slideshowBg && (slides.size() > 0)) {

        slides[currentSlideId].draw(0, 0, w, h);

        if (slideTimer > slideFramesDuration) {
            if (!bTimelineSlideChange) {
                currentSlideId += 1;
            }
            slideTimer = 0;
        }
        slideTimer += 1;
    }
}

void quad::applyBlendmode()
{
    switch (blendMode) {
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
    settings.numSamples = 0;
    settings.useDepth = false;
    settings.useStencil = false;
    settings.width = w;
    settings.height = h;
    quadFbo.allocate(settings);
    maskFbo.allocate(settings);
    targetFbo.allocate(settings);

    if (!quadFbo.isAllocated()) {
        ofLogError() << "Unable to alocate quad FBO!";
    }
}

//---------------------------------------------------------------
// adds a new point to the mask point vector
// the given point has to be in non-normalized window pixel coordinates
void quad::maskAddPoint(ofPoint point)
{
    const ofPoint warpedPoint = getWarpedPoint(point);
    const ofPoint normalizedPoint(warpedPoint.x / ofGetWidth(), warpedPoint.y / ofGetHeight());

    if ((m_maskPoints.empty()) || (m_maskPoints.size() == 1)) {
        m_maskPoints.push_back(normalizedPoint);
    } else {
        list<ofVec3f> pts;

        float min_diff = RAND_MAX;
        for (unsigned int i = 0; i < m_maskPoints.size(); i++) {
            float diff = (m_maskPoints.at(i) - normalizedPoint).lengthSquared();
            if (diff < min_diff) {
                pts.push_front(m_maskPoints.at(i));
                min_diff = diff;
            } else {
                pts.push_back(m_maskPoints.at(i));
            }
        }

        ofVec3f min = pts.front();

        vector<ofVec3f>::iterator itr;
        for (itr = m_maskPoints.begin(); itr != m_maskPoints.end(); itr++) {
            if (*itr == min) {
                break;
            }
        }
        m_maskPoints.insert(itr, normalizedPoint);
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
    imgBg = true;
    bool bLoaded = img.load(imgPath);
    if (bLoaded) {
        bgImg = imgPath;
        loadedImg = imgName;
    }
}

//---------------------------------------------------------------
void quad::loadVideoFromFile(string videoName, string videoPath)
{
    videoBg = true;
    if (video.isLoaded()) {
        video.closeMovie();
    }
    video.load(videoPath);
    if (video.isLoaded()) {
        bgVideo = videoPath;
        loadedVideo = videoName;
        videoWidth = video.getWidth();
        videoHeight = video.getHeight();
        video.play();
    }
}

//---------------------------------------------------------------
// Bezier helpers -----------------------------------
// Bezier setup -------------------------------------
void quad::bezierSurfaceSetup()
{
    float tmp_bezierPoints[4][4][3] = {
        { { 0, 0, 0 }, { 0.333, 0, 0 }, { 0.667, 0, 0 }, { 1.0, 0, 0 } },
        { { 0, 0.333, 0 }, { 0.333, 0.333, 0 }, { 0.667, 0.333, 0 }, { 1.0, 0.333, 0 } },
        { { 0, 0.667, 0 }, { 0.333, 0.667, 0 }, { 0.667, 0.667, 0 }, { 1.0, 0.667, 0 } },
        { { 0, 1.0, 0 }, { 0.333, 1.0, 0 }, { 0.667, 1.0, 0 }, { 1.0, 1.0, 0 } }
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
        { { bezierPoints[0][0][0] * ofGetWidth(), bezierPoints[0][0][1] * ofGetHeight(), 0 }, { bezierPoints[0][1][0] * ofGetWidth(), bezierPoints[0][1][1] * ofGetHeight(), 0 }, { bezierPoints[0][2][0] * ofGetWidth(), bezierPoints[0][2][1] * ofGetHeight(), 0 }, { bezierPoints[0][3][0] * ofGetWidth(), bezierPoints[0][3][1] * ofGetHeight(), 0 } },
        { { bezierPoints[1][0][0] * ofGetWidth(), bezierPoints[1][0][1] * ofGetHeight(), 0 }, { bezierPoints[1][1][0] * ofGetWidth(), bezierPoints[1][1][1] * ofGetHeight(), 0 }, { bezierPoints[1][2][0] * ofGetWidth(), bezierPoints[1][2][1] * ofGetHeight(), 0 }, { bezierPoints[1][3][0] * ofGetWidth(), bezierPoints[1][3][1] * ofGetHeight(), 0 } },
        { { bezierPoints[2][0][0] * ofGetWidth(), bezierPoints[2][0][1] * ofGetHeight(), 0 }, { bezierPoints[2][1][0] * ofGetWidth(), bezierPoints[2][1][1] * ofGetHeight(), 0 }, { bezierPoints[2][2][0] * ofGetWidth(), bezierPoints[2][2][1] * ofGetHeight(), 0 }, { bezierPoints[2][3][0] * ofGetWidth(), bezierPoints[2][3][1] * ofGetHeight(), 0 } },
        { { bezierPoints[3][0][0] * ofGetWidth(), bezierPoints[3][0][1] * ofGetHeight(), 0 }, { bezierPoints[3][1][0] * ofGetWidth(), bezierPoints[3][1][1] * ofGetHeight(), 0 }, { bezierPoints[3][2][0] * ofGetWidth(), bezierPoints[3][2][1] * ofGetHeight(), 0 }, { bezierPoints[3][3][0] * ofGetWidth(), bezierPoints[3][3][1] * ofGetHeight(), 0 } }
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 3; ++k) {
                bezierCtrlPoints[i][j][k] = tmp_ctrlPoints[i][j][k];
            }
        }
    }

    //This sets up my Texture Surface
    GLfloat texpts[2][2][2] = {
        { { 0, 0 }, { 1, 0 } }, { { 0, 1 }, { 1, 1 } }
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
    float tmp_ctrlPoints[4][4][3] = {
        { { bezierPoints[0][0][0] * ofGetWidth(), bezierPoints[0][0][1] * ofGetHeight(), 0 }, { bezierPoints[0][1][0] * ofGetWidth(), bezierPoints[0][1][1] * ofGetHeight(), 0 }, { bezierPoints[0][2][0] * ofGetWidth(), bezierPoints[0][2][1] * ofGetHeight(), 0 }, { bezierPoints[0][3][0] * ofGetWidth(), bezierPoints[0][3][1] * ofGetHeight(), 0 } },
        { { bezierPoints[1][0][0] * ofGetWidth(), bezierPoints[1][0][1] * ofGetHeight(), 0 }, { bezierPoints[1][1][0] * ofGetWidth(), bezierPoints[1][1][1] * ofGetHeight(), 0 }, { bezierPoints[1][2][0] * ofGetWidth(), bezierPoints[1][2][1] * ofGetHeight(), 0 }, { bezierPoints[1][3][0] * ofGetWidth(), bezierPoints[1][3][1] * ofGetHeight(), 0 } },
        { { bezierPoints[2][0][0] * ofGetWidth(), bezierPoints[2][0][1] * ofGetHeight(), 0 }, { bezierPoints[2][1][0] * ofGetWidth(), bezierPoints[2][1][1] * ofGetHeight(), 0 }, { bezierPoints[2][2][0] * ofGetWidth(), bezierPoints[2][2][1] * ofGetHeight(), 0 }, { bezierPoints[2][3][0] * ofGetWidth(), bezierPoints[2][3][1] * ofGetHeight(), 0 } },
        { { bezierPoints[3][0][0] * ofGetWidth(), bezierPoints[3][0][1] * ofGetHeight(), 0 }, { bezierPoints[3][1][0] * ofGetWidth(), bezierPoints[3][1][1] * ofGetHeight(), 0 }, { bezierPoints[3][2][0] * ofGetWidth(), bezierPoints[3][2][1] * ofGetHeight(), 0 }, { bezierPoints[3][3][0] * ofGetWidth(), bezierPoints[3][3][1] * ofGetHeight(), 0 } }
    };

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 3; ++k) {
                bezierCtrlPoints[i][j][k] = tmp_ctrlPoints[i][j][k];
            }
        }
    }

    if (bBezier) {
        glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &bezierCtrlPoints[0][0][0]);
        GLfloat texpts[2][2][2] = {
            { { 0, 0 }, { 1, 0 } }, { { 0, 1 }, { 1, 1 } }
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

void quad::gridSurfaceSetup()
{
    gridMesh.clearVertices();
    gridMesh.clearIndices();
    gridMesh.clearTexCoords();

    for (int i = 0; i <= gridRows; i++) {
        vector<vector<float>> row;
        for (int j = 0; j <= gridColumns; j++) {
            vector<float> column;
            column.push_back((float)(1.0 / gridColumns * j));
            column.push_back((float)(1.0 / gridRows * i));
            column.push_back(0.0);
            row.push_back(column);
        }
        gridPoints.push_back(row);
    }

    for (int i = 0; i <= gridRows; i++) {
        for (int j = 0; j <= gridColumns; j++) {
            gridMesh.addTexCoord(ofVec2f((gridPoints[i][j][0] * ofGetWidth()), (gridPoints[i][j][1] * ofGetHeight())));
        }
    }

    for (int i = 0; i <= gridRows; i++) {
        for (int j = 0; j <= gridColumns; j++) {
            gridMesh.addVertex(ofVec3f((gridPoints[i][j][0] * ofGetWidth()), (gridPoints[i][j][1] * ofGetHeight()), (0.0)));
        }
    }

    for (int i = 0; i < gridRows; i++) {
        for (int j = 0; j < gridColumns; j++) {

            gridMesh.addIndex((i * (gridColumns + 1) + j)); //a
            gridMesh.addIndex((i * (gridColumns + 1) + j + 1)); //b
            gridMesh.addIndex(((i + 1) * (gridColumns + 1) + j + 1)); //c
            gridMesh.addIndex(((i + 1) * (gridColumns + 1) + j + 1)); //c
            gridMesh.addIndex(((i + 1) * (gridColumns + 1) + j)); //d
            gridMesh.addIndex((i * (gridColumns + 1) + j)); //a
        }
    }
}

//---------------------------------------------------------------
void quad::gridSurfaceUpdate(bool bRefresh)
{
    // TODO: to optimize this try to limit recalculation to cases when it's really needed
    //This sets up my Grid Surface
    gridMesh.clearVertices();
    gridMesh.clearIndices();

    if (bRefresh || (gridPoints.size() != (gridRows + 1)) || (gridPoints[0].size() != (gridColumns + 1))) {
        gridMesh.clearTexCoords();
        gridPoints.clear();
        for (int i = 0; i <= gridRows; i++) {
            vector<vector<float>> row;
            for (int j = 0; j <= gridColumns; j++) {
                vector<float> column;
                column.push_back((float)(1.0 / gridColumns * j));
                column.push_back((float)(1.0 / gridRows * i));
                column.push_back(0.0);
                row.push_back(column);
            }
            gridPoints.push_back(row);
        }
        for (int i = 0; i <= gridRows; i++) {
            for (int j = 0; j <= gridColumns; j++) {
                gridMesh.addTexCoord(ofVec2f((gridPoints[i][j][0] * ofGetWidth()), (gridPoints[i][j][1] * ofGetHeight())));
            }
        }
    }

    for (int i = 0; i <= gridRows; i++) {
        for (int j = 0; j <= gridColumns; j++) {
            gridMesh.addVertex(ofVec3f((gridPoints[i][j][0] * ofGetWidth()), (gridPoints[i][j][1] * ofGetHeight()), (0.0)));
        }
    }

    for (int i = 0; i < gridRows; i++) {
        for (int j = 0; j < gridColumns; j++) {

            gridMesh.addIndex((i * (gridColumns + 1) + j)); //a
            gridMesh.addIndex((i * (gridColumns + 1) + j + 1)); //b
            gridMesh.addIndex(((i + 1) * (gridColumns + 1) + j + 1)); //c
            gridMesh.addIndex(((i + 1) * (gridColumns + 1) + j + 1)); //c
            gridMesh.addIndex(((i + 1) * (gridColumns + 1) + j)); //d
            gridMesh.addIndex((i * (gridColumns + 1) + j)); //a
        }
    }
}

// Markers -----------------------------------------------------

// Bezier markers ----------------------------------------------
void quad::drawBezierMarkers()
{
    ofPushStyle();
    ofSetColor(220, 200, 0, 255);
    ofSetLineWidth(1.5);
    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            ofVec3f point;
            point.x = bezierCtrlPoints[i][j][0];
            point.y = bezierCtrlPoints[i][j][1];
            point.z = bezierCtrlPoints[i][j][2];
            point = findWarpedPoint(dst, src, point);

            if (bHighlightCtrlPoint && highlightedCtrlPointRow == i && highlightedCtrlPointCol == j) {
                ofFill();
            }
            ofDrawCircle(point.x, point.y, 3.6);
            ofNoFill();
        }
    }
    ofSetLineWidth(1.2);
    ofVec3f pointA;
    ofVec3f pointB;
    //
    pointA.x = bezierCtrlPoints[0][0][0];
    pointA.y = bezierCtrlPoints[0][0][1];
    pointA.z = bezierCtrlPoints[0][0][2];
    pointB.x = bezierCtrlPoints[0][1][0];
    pointB.y = bezierCtrlPoints[0][1][1];
    pointB.z = bezierCtrlPoints[0][1][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[0][0][0];
    pointA.y = bezierCtrlPoints[0][0][1];
    pointA.z = bezierCtrlPoints[0][0][2];
    pointB.x = bezierCtrlPoints[1][0][0];
    pointB.y = bezierCtrlPoints[1][0][1];
    pointB.z = bezierCtrlPoints[1][0][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[0][0][0];
    pointA.y = bezierCtrlPoints[0][0][1];
    pointA.z = bezierCtrlPoints[0][0][2];
    pointB.x = bezierCtrlPoints[1][1][0];
    pointB.y = bezierCtrlPoints[1][1][1];
    pointB.z = bezierCtrlPoints[1][1][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[0][3][0];
    pointA.y = bezierCtrlPoints[0][3][1];
    pointA.z = bezierCtrlPoints[0][3][2];
    pointB.x = bezierCtrlPoints[1][3][0];
    pointB.y = bezierCtrlPoints[1][3][1];
    pointB.z = bezierCtrlPoints[1][3][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[0][3][0];
    pointA.y = bezierCtrlPoints[0][3][1];
    pointA.z = bezierCtrlPoints[0][3][2];
    pointB.x = bezierCtrlPoints[0][2][0];
    pointB.y = bezierCtrlPoints[0][2][1];
    pointB.z = bezierCtrlPoints[0][2][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[0][3][0];
    pointA.y = bezierCtrlPoints[0][3][1];
    pointA.z = bezierCtrlPoints[0][3][2];
    pointB.x = bezierCtrlPoints[1][2][0];
    pointB.y = bezierCtrlPoints[1][2][1];
    pointB.z = bezierCtrlPoints[1][2][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[3][0][0];
    pointA.y = bezierCtrlPoints[3][0][1];
    pointA.z = bezierCtrlPoints[3][0][2];
    pointB.x = bezierCtrlPoints[3][1][0];
    pointB.y = bezierCtrlPoints[3][1][1];
    pointB.z = bezierCtrlPoints[3][1][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[3][0][0];
    pointA.y = bezierCtrlPoints[3][0][1];
    pointA.z = bezierCtrlPoints[3][0][2];
    pointB.x = bezierCtrlPoints[2][0][0];
    pointB.y = bezierCtrlPoints[2][0][1];
    pointB.z = bezierCtrlPoints[2][0][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[3][0][0];
    pointA.y = bezierCtrlPoints[3][0][1];
    pointA.z = bezierCtrlPoints[3][0][2];
    pointB.x = bezierCtrlPoints[2][1][0];
    pointB.y = bezierCtrlPoints[2][1][1];
    pointB.z = bezierCtrlPoints[2][1][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[3][3][0];
    pointA.y = bezierCtrlPoints[3][3][1];
    pointA.z = bezierCtrlPoints[3][3][2];
    pointB.x = bezierCtrlPoints[3][2][0];
    pointB.y = bezierCtrlPoints[3][2][1];
    pointB.z = bezierCtrlPoints[3][2][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[3][3][0];
    pointA.y = bezierCtrlPoints[3][3][1];
    pointA.z = bezierCtrlPoints[3][3][2];
    pointB.x = bezierCtrlPoints[2][3][0];
    pointB.y = bezierCtrlPoints[2][3][1];
    pointB.z = bezierCtrlPoints[2][3][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[3][3][0];
    pointA.y = bezierCtrlPoints[3][3][1];
    pointA.z = bezierCtrlPoints[3][3][2];
    pointB.x = bezierCtrlPoints[2][2][0];
    pointB.y = bezierCtrlPoints[2][2][1];
    pointB.z = bezierCtrlPoints[2][2][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[1][2][0];
    pointA.y = bezierCtrlPoints[1][2][1];
    pointA.z = bezierCtrlPoints[1][2][2];
    pointB.x = bezierCtrlPoints[2][2][0];
    pointB.y = bezierCtrlPoints[2][2][1];
    pointB.z = bezierCtrlPoints[2][2][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[1][2][0];
    pointA.y = bezierCtrlPoints[1][2][1];
    pointA.z = bezierCtrlPoints[1][2][2];
    pointB.x = bezierCtrlPoints[1][1][0];
    pointB.y = bezierCtrlPoints[1][1][1];
    pointB.z = bezierCtrlPoints[1][1][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[2][1][0];
    pointA.y = bezierCtrlPoints[2][1][1];
    pointA.z = bezierCtrlPoints[2][1][2];
    pointB.x = bezierCtrlPoints[1][1][0];
    pointB.y = bezierCtrlPoints[1][1][1];
    pointB.z = bezierCtrlPoints[1][1][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    //
    pointA.x = bezierCtrlPoints[2][1][0];
    pointA.y = bezierCtrlPoints[2][1][1];
    pointA.z = bezierCtrlPoints[2][1][2];
    pointB.x = bezierCtrlPoints[2][2][0];
    pointB.y = bezierCtrlPoints[2][2][1];
    pointB.z = bezierCtrlPoints[2][2][2];
    pointA = findWarpedPoint(dst, src, pointA);
    pointB = findWarpedPoint(dst, src, pointB);
    ofDrawLine(pointA, pointB);
    ofPopStyle();
}

// Bezier markers ----------------------------------------------
void quad::drawGridMarkers()
{
    ofPushStyle();
    ofSetColor(0, 200, 220, 255);
    ofSetLineWidth(1.5);

    for (int i = 0; i <= gridRows; i++) {
        for (int j = 0; j <= gridColumns; j++) {
            ofVec3f point;
            point.x = gridPoints[i][j][0] * ofGetWidth();
            point.y = gridPoints[i][j][1] * ofGetHeight();
            point.z = 0.0;
            point = findWarpedPoint(dst, src, point);
            if (bHighlightCtrlPoint && highlightedCtrlPointRow == i && highlightedCtrlPointCol == j) {
                ofFill();
            }
            ofDrawCircle(point.x, point.y, 3.0);
            ofNoFill();
        }
    }
    ofPopStyle();
}

// Mask markers --------------------------------------
void quad::drawMaskMarkers()
{
    if (m_maskPoints.size() > 0) {
        ofPushStyle();

        // draw the contour polygon the markers form
        ofPolyline contour;
        for (size_t i = 0; i < m_maskPoints.size(); i++) {
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
        for (size_t i = 0; i < m_maskPoints.size(); i++) {
            ofPushStyle();
            ofSetColor(100, 139, 150, 255); // blueish grey
            ofSetLineWidth(1.0);

            const ofPoint scaledPoint = Util::scalePointToPixel(m_maskPoints[i]);
            const ofPoint warpedPoint = findWarpedPoint(dst, src, scaledPoint);

            // if the mouse is over the handle fill the inner circle
            if (bHighlightMaskPoint && highlightedMaskPoint == i) {
                ofFill();
            }
            ofDrawCircle(warpedPoint, 4);
            ofNoFill();
            ofDrawCircle(warpedPoint, 10);
            ofPopStyle();
        }

        ofPopStyle();
    }
}

#ifdef WITH_KINECT
void quad::setKinect(kinectManager& kinect)
{
    quadKinect = &kinect;
}
#endif

#ifdef WITH_SYPHON
setSyphone(ofxSyphonClient& syphon)
{
    syphClientTex = &syphon;
}
#endif
