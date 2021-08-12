#include "ofApp.h"

//--------------------------------------------------------------

void ofApp::saveCurrentSettingsToXMLFile(std::string xmlFilePath)
{
    ofxXmlSettings xmlSettingsFile;

    // update the XML object with the current settings, before saving it
    xmlSettingsFile.setValue("GENERAL:ACTIVE_QUAD", activeQuad);
    xmlSettingsFile.setValue("GENERAL:N_OF_QUADS", nOfQuads);
    xmlSettingsFile.setValue("TIMELINE:USE_TIMELINE", useTimeline);
    xmlSettingsFile.setValue("TIMELINE:DURATION", timelineDurationSeconds);
    for (int j = 0; j < 4; j++) {
        xmlSettingsFile.setValue("SHARED_VIDEOS:VIDEO_" + ofToString(j) + ":PATH", sharedVideosFiles[j]);
    }

    for (int i = 0; i < MAX_QUADS; i++) {
        if (quads[i].initialized) {

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":NUMBER", quads[i].quadNumber);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":LAYER", quads[i].layer);

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":TIMELINE:TINT", quads[i].bTimelineTint);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":TIMELINE:COLOR", quads[i].bTimelineColor);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":TIMELINE:ALPHA", quads[i].bTimelineAlpha);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":TIMELINE:SLIDE", quads[i].bTimelineSlideChange);

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CONTENT:DISPX", quads[i].quadDispX);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CONTENT:DISPY", quads[i].quadDispY);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CONTENT:WIDTH", quads[i].quadW);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CONTENT:HEIGHT", quads[i].quadH);

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:LOADED_IMG", quads[i].loadedImg);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:LOADED_IMG_PATH", quads[i].bgImg);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":VIDEO:LOADED_VIDEO", quads[i].loadedVideo);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":VIDEO:LOADED_VIDEO_PATH", quads[i].bgVideo);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":SLIDESHOW:LOADED_SLIDESHOW", quads[i].bgSlideshow);

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_0:X", quads[i].corners[0].x);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_0:Y", quads[i].corners[0].y);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_1:X", quads[i].corners[1].x);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_1:Y", quads[i].corners[1].y);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_2:X", quads[i].corners[2].x);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_2:Y", quads[i].corners[2].y);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_3:X", quads[i].corners[3].x);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_3:Y", quads[i].corners[3].y);

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IS_ON", quads[i].isOn);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":COLOR:ACTIVE", quads[i].colorBg);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":SLIDESHOW:ACTIVE", quads[i].slideshowBg);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":SLIDESHOW:SPEED", quads[i].slideshowSpeed);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":SLIDESHOW:TRANSITIONS", quads[i].bFadeTransitions);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":CAM:ACTIVE", quads[i].camBg);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:ACTIVE", quads[i].imgBg);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":VIDEO:ACTIVE", quads[i].videoBg);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:MULT_X", quads[i].imgMultX);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:MULT_Y", quads[i].imgMultY);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:FIT", quads[i].imageFit);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:KEEP_ASPECT", quads[i].imageKeepAspect);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:HUE", quads[i].hue);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:SATURATION", quads[i].saturation);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:LUMINANCE", quads[i].luminance);

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":VIDEO:SPEED", quads[i].videoSpeed);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":VIDEO:VOLUME", quads[i].videoVolume);
            ofClamp(quads[i].videoVolume, 0.f, 1.f); // this is to avoid warnings and for backwards compatibility (the value used to be in range 0 - 10)
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":VIDEO:LOOP", quads[i].videoLoop);

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:COLORIZE:R", quads[i].imgColorize.r);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:COLORIZE:G", quads[i].imgColorize.g);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:COLORIZE:B", quads[i].imgColorize.b);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:COLORIZE:A", quads[i].imgColorize.a);

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:FLIP:H", quads[i].imgHFlip);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":IMG:FLIP:V", quads[i].imgVFlip);

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":SHARED_VIDEO:ACTIVE", quads[i].sharedVideoBg);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":SHARED_VIDEO:NUM", quads[i].sharedVideoNum);

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":BLENDING:ON", quads[i].bBlendModes);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":BLENDING:MODE", quads[i].blendMode);

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":GREENSCREEN:ON", quads[i].bUseGreenscreen);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":GREENSCREEN:THRESHOLD", quads[i].thresholdGreenscreen);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":GREENSCREEN:COLOR:R", quads[i].colorGreenscreen.r);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":GREENSCREEN:COLOR:G", quads[i].colorGreenscreen.g);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":GREENSCREEN:COLOR:B", quads[i].colorGreenscreen.b);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":GREENSCREEN:COLOR:A", quads[i].colorGreenscreen.a);

            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:ON", quads[i].bEdgeBlend);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:EXPONENT", quads[i].edgeBlendExponent);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:GAMMA", quads[i].edgeBlendGamma);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:LUMINANCE", quads[i].edgeBlendLuminance);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:AMOUNT:SIN", quads[i].edgeBlendAmountSin);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:AMOUNT:DX", quads[i].edgeBlendAmountDx);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:AMOUNT:TOP", quads[i].edgeBlendAmountTop);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:AMOUNT:BOTTOM", quads[i].edgeBlendAmountBottom);

            //mask stuff
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:ON", quads[i].bMask);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:INVERT_MASK", quads[i].maskInvert);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:MASK_OUTLINE", quads[i].bDrawMaskOutline);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:N_POINTS", (int)quads[i].m_maskPoints.size());
            if (quads[i].m_maskPoints.size() > 0) {
                for (size_t j = 0; j < quads[i].m_maskPoints.size(); j++) {
                    xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:POINTS:POINT_" + ofToString(j) + ":X", quads[i].m_maskPoints[j].x);
                    xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:POINTS:POINT_" + ofToString(j) + ":Y", quads[i].m_maskPoints[j].y);
                }
            }
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:RECTANGULAR:TOP", quads[i].crop[0]);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:RECTANGULAR:RIGHT", quads[i].crop[1]);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:RECTANGULAR:BOTTOM", quads[i].crop[2]);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:RECTANGULAR:LEFT", quads[i].crop[3]);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:CIRCULAR:X", quads[i].circularCrop[0]);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:CIRCULAR:Y", quads[i].circularCrop[1]);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:CIRCULAR:RADIUS", quads[i].circularCrop[2]);
            // deform stuff
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":DEFORM:BEZIER:ON", quads[i].bBezier);
            xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":DEFORM:GRID:ON", quads[i].bGrid);
            // bezier stuff
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":DEFORM:BEZIER:CTRLPOINTS:POINT_" + ofToString(j) + "_" + ofToString(k) + ":X", quads[i].bezierPoints[j][k][0]);
                    xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":DEFORM:BEZIER:CTRLPOINTS:POINT_" + ofToString(j) + "_" + ofToString(k) + ":Y", quads[i].bezierPoints[j][k][1]);
                    xmlSettingsFile.setValue("QUADS:QUAD_" + ofToString(i) + ":DEFORM:BEZIER:CTRLPOINTS:POINT_" + ofToString(j) + "_" + ofToString(k) + ":Z", quads[i].bezierPoints[j][k][2]);
                }
            }
        }
    }

    // Finally save the *.xml settings file
    const bool wasSavedSuccessful = xmlSettingsFile.saveFile(xmlFilePath);

    if (wasSavedSuccessful) {
        ofLogNotice() << "Saved settings file to: " << xmlFilePath;
    } else {
        ofLogError() << "Error saving the settings file to: " << xmlFilePath;
    }
}

void ofApp::loadSettingsFromXMLFile(std::string xmlFilePath)
{
    ofxXmlSettings xmlSettingsFile;
    const bool wasLoadSuccessful = xmlSettingsFile.loadFile(xmlFilePath);

    if (wasLoadSuccessful) {
        ofLogNotice() << "Loaded settings file: " << xmlFilePath;

        // initializes layers array
        for (int i = 0; i < MAX_QUADS; i++) {
            layers[i] = -1;
        }

        nOfQuads = xmlSettingsFile.getValue("GENERAL:N_OF_QUADS", 0);
        activeQuad = xmlSettingsFile.getValue("GENERAL:ACTIVE_QUAD", 0);
        useTimeline = xmlSettingsFile.getValue("TIMELINE:USE_TIMELINE", 0);
        timelineDurationSeconds = xmlSettingsFile.getValue("TIMELINE:DURATION", 100);

        for (int j = 0; j < 4; j++) {
            string sharedVideoPath = xmlSettingsFile.getValue("SHARED_VIDEOS:VIDEO_" + ofToString(j) + ":PATH", "");
            sharedVideosFiles[j] = sharedVideoPath;
            if (sharedVideoPath != "") {
                openSharedVideoFile(sharedVideoPath, j);
            }
        }

        for (int i = 0; i < nOfQuads; i++) {
            float x0 = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_0:X", 0.0);
            float y0 = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_0:Y", 0.0);
            float x1 = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_1:X", 0.0);
            float y1 = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_1:Y", 0.0);
            float x2 = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_2:X", 0.0);
            float y2 = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_2:Y", 0.0);
            float x3 = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_3:X", 0.0);
            float y3 = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CORNERS:CORNER_3:Y", 0.0);

#ifdef WITH_KINECT
#ifdef WITH_SYPHON
            quads[i].setup(ofPoint(x0, y0), ofPoint(x1, y1), ofPoint(x2, y2), ofPoint(x3, y3), edgeBlendShader, quadMaskShader, surfaceShader, crossfadeShader, m_cameras, kinect, syphClient, ttf);
#else
            quads[i].setup(ofPoint(x0, y0), ofPoint(x1, y1), ofPoint(x2, y2), ofPoint(x3, y3), edgeBlendShader, quadMaskShader, surfaceShader, crossfadeShader, m_cameras, ttf);
#endif
#else
#ifdef WITH_SYPHON
            quads[i].setup(ofPoint(x0, y0), ofPoint(x1, y1), ofPoint(x2, y2), ofPoint(x3, y3), edgeBlendShader, quadMaskShader, surfaceShader, crossfadeShader, m_cameras, syphClient, ttf);
#else
            quads[i].setup(ofPoint(x0, y0), ofPoint(x1, y1), ofPoint(x2, y2), ofPoint(x3, y3), edgeBlendShader, quadMaskShader, surfaceShader, crossfadeShader, m_cameras, ttf);
#endif
#endif
            quads[i].quadNumber = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":NUMBER", 0);
            quads[i].layer = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":LAYER", 0);
            layers[quads[i].layer] = quads[i].quadNumber;

            quads[i].bTimelineTint = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":TIMELINE:TINT", 0);
            quads[i].bTimelineColor = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":TIMELINE:COLOR", 0);
            quads[i].bTimelineAlpha = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":TIMELINE:ALPHA", 0);
            quads[i].bTimelineSlideChange = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":TIMELINE:SLIDE", 0);

            quads[i].quadDispX = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CONTENT:DISPX", 0);
            quads[i].quadDispY = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CONTENT:DISPY", 0);
            quads[i].quadW = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CONTENT:WIDTH", 0);
            quads[i].quadH = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CONTENT:HEIGHT", 0);

            quads[i].imgBg = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:ACTIVE", 0);
            quads[i].loadedImg = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:LOADED_IMG", "", 0);
            quads[i].bgImg = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:LOADED_IMG_PATH", "", 0);
            if ((quads[i].imgBg) && (quads[i].bgImg != "")) {
                quads[i].loadImageFromFile(quads[i].loadedImg, quads[i].bgImg);
            }
            quads[i].imageFit = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:FIT", 0);
            quads[i].imageKeepAspect = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:KEEP_ASPECT", 0);
            quads[i].hue = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:HUE", 0.0);
            quads[i].saturation = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:SATURATION", 0.0);
            quads[i].luminance = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:LUMINANCE", 0.0);

            quads[i].imgHFlip = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:FLIP:H", 0);
            quads[i].imgVFlip = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:FLIP:V", 0);

            quads[i].videoBg = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":VIDEO:ACTIVE", 0);
            quads[i].loadedVideo = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":VIDEO:LOADED_VIDEO", "", 0);
            quads[i].bgVideo = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":VIDEO:LOADED_VIDEO_PATH", "", 0);
            if ((quads[i].videoBg) && (quads[i].bgVideo != "")) {
                quads[i].loadVideoFromFile(quads[i].loadedVideo, quads[i].bgVideo);
            }

            quads[i].sharedVideoBg = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":SHARED_VIDEO:ACTIVE", 0);
            quads[i].sharedVideoNum = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":SHARED_VIDEO:NUM", 1);

            quads[i].bgSlideshow = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":SLIDESHOW:LOADED_SLIDESHOW", 0);

            quads[i].colorBg = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":COLOR:ACTIVE", 0);

            quads[i].slideshowBg = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":SLIDESHOW:ACTIVE", 0);
            quads[i].slideshowSpeed = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":SLIDESHOW:SPEED", 1.0);
            quads[i].bFadeTransitions = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":SLIDESHOW:TRANSITIONS", 0);

            quads[i].camBg = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":CAM:ACTIVE", 0);

            quads[i].imgMultX = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:MULT_X", 1.0);
            quads[i].imgMultY = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:MULT_Y", 1.0);
            quads[i].videoSpeed = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":VIDEO:SPEED", 1.0);
            quads[i].videoVolume = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":VIDEO:VOLUME", 0);
            quads[i].videoLoop = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":VIDEO:LOOP", 1);

            quads[i].imgColorize.r = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:COLORIZE:R", 1.0);
            quads[i].imgColorize.g = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:COLORIZE:G", 1.0);
            quads[i].imgColorize.b = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:COLORIZE:B", 1.0);
            quads[i].imgColorize.a = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IMG:COLORIZE:A", 1.0);

            quads[i].bBlendModes = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":BLENDING:ON", 0);
            quads[i].blendMode = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":BLENDING:MODE", 0);

            quads[i].bUseGreenscreen = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":GREENSCREEN:ON", 0);
            quads[i].thresholdGreenscreen = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":GREENSCREEN:THRESHOLD", 0);
            quads[i].colorGreenscreen.r = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":GREENSCREEN:COLOR:R", 1.0f);
            quads[i].colorGreenscreen.g = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":GREENSCREEN:COLOR:G", 1.0f);
            quads[i].colorGreenscreen.b = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":GREENSCREEN:COLOR:B", 1.0f);
            quads[i].colorGreenscreen.a = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":GREENSCREEN:COLOR:A", 1.0f);

            quads[i].bEdgeBlend = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:ON", 0);
            quads[i].edgeBlendExponent = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:EXPONENT", 1.0);
            quads[i].edgeBlendGamma = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:GAMMA", 1.8);
            quads[i].edgeBlendLuminance = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:LUMINANCE", 0.0);
            quads[i].edgeBlendAmountSin = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:AMOUNT:SIN", 0.3);
            quads[i].edgeBlendAmountDx = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:AMOUNT:DX", 0.3);
            quads[i].edgeBlendAmountTop = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:AMOUNT:TOP", 0.0);
            quads[i].edgeBlendAmountBottom = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":EDGE_BLENDING:AMOUNT:BOTTOM", 0.0);

            //mask stuff
            quads[i].bMask = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:ON", 0);
            quads[i].maskInvert = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:INVERT_MASK", 0);
            quads[i].bDrawMaskOutline = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:MASK_OUTLINE", 0);
            const int numberOfMaskPoints = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:N_POINTS", 0);
            quads[i].m_maskPoints.clear();
            if (numberOfMaskPoints > 0) {
                for (size_t j = 0; j < numberOfMaskPoints; j++) {
                    ofPoint tempMaskPoint;
                    tempMaskPoint.x = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:POINTS:POINT_" + ofToString(j) + ":X", 0.0);
                    tempMaskPoint.y = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:POINTS:POINT_" + ofToString(j) + ":Y", 0.0);
                    quads[i].m_maskPoints.push_back(tempMaskPoint);
                }
            }
            quads[i].crop[0] = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:RECTANGULAR:TOP", 0.0);
            quads[i].crop[1] = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:RECTANGULAR:RIGHT", 0.0);
            quads[i].crop[2] = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:RECTANGULAR:BOTTOM", 0.0);
            quads[i].crop[3] = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:RECTANGULAR:LEFT", 0.0);
            quads[i].circularCrop[0] = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:CIRCULAR:X", 0.5);
            quads[i].circularCrop[1] = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:CIRCULAR:Y", 0.5);
            quads[i].circularCrop[2] = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":MASK:CROP:CIRCULAR:RADIUS", 0.0);
            // deform stuff
            quads[i].bBezier = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":DEFORM:BEZIER:ON", 0);
            quads[i].bGrid = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":DEFORM:GRID:ON", 0);
            // bezier stuff
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    quads[i].bezierPoints[j][k][0] = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":DEFORM:BEZIER:CTRLPOINTS:POINT_" + ofToString(j) + "_" + ofToString(k) + ":X", 0.0);
                    quads[i].bezierPoints[j][k][1] = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":DEFORM:BEZIER:CTRLPOINTS:POINT_" + ofToString(j) + "_" + ofToString(k) + ":Y", 0.0);
                    quads[i].bezierPoints[j][k][2] = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":DEFORM:BEZIER:CTRLPOINTS:POINT_" + ofToString(j) + "_" + ofToString(k) + ":Z", 0.0);
                }
            }

            quads[i].isOn = xmlSettingsFile.getValue("QUADS:QUAD_" + ofToString(i) + ":IS_ON", 0);
            quads[i].isActive = false;
        }
        quads[activeQuad].isActive = true;
        m_gui.updatePages(quads[activeQuad]);
        m_gui.showPage(2);

        if(!bGui) {
            m_gui.toggleDraw();
            bGui = !bGui;
        }
        glDisable(GL_DEPTH_TEST);        
    } else {
        // load was not succesful, maybe the xml is malformatted
        std::cout << "Error loading the settings file: \"" << xmlFilePath << "\" (make sure it is a properly formatted *.xml file)" << std::endl;
    }
}
