#include "GUI.hpp"
#include "ofApp.h"

GUI::GUI(ofApp* app) : m_app(app),
                         m_dummyBool(false),
                         m_dummyFloat(0.f),
                         m_dummyInt(0)
{
    // initializing gui (sets up header page etc.)
    m_gui.setup();
    m_gui.setAutoSave(false); // turn auto save off

    // overriding default theme
    m_config.toggleHeight = 16;
    m_config.buttonHeight = 18;
    m_config.sliderTextHeight = 18;
    m_config.titleHeight = 18;

    m_config.fullActiveColor = 0x648B96; // bluish-grey
    m_config.textColor = 0xFFFFFF;       // white
    m_config.textBGOverColor = 0x888888; // grey

    defaultSimpleGuiConfig = m_config;
}


/**
 * This method sets up the GUI pages structure and initializes the quad specific values with dummies.
 * The loading flags are set here as well.
 * The data binding to the currently selected quad happens in updatePages()
 */
void GUI::setupPages()
{
    // general page
    m_gui.addTitle("Project");
    m_gui.addButton("Load Project", m_app->m_loadProjectFlag);
    m_gui.addButton("Save Project", m_app->m_saveProjectFlag);

    m_gui.addTitle("show/hide quads");
    // toggling surfaces on/off
    for(int i = 0; i < MAX_QUADS; i++)
    {
        m_gui.addToggle("surface " + ofToString(i), m_app->quads[i].isOn);
    }
    m_gui.addTitle("General controls").setNewColumn(true);
    m_gui.addToggle("surfaces corner snap", m_app->bSnapOn);
    m_gui.addTitle("Shared videos");
    m_gui.addButton("load shared video 1", m_app->m_loadSharedVideo0Flag);
    m_gui.addButton("load shared video 2", m_app->m_loadSharedVideo1Flag);
    m_gui.addButton("load shared video 3", m_app->m_loadSharedVideo2Flag);
    m_gui.addButton("load shared video 4", m_app->m_loadSharedVideo3Flag);
    m_gui.addButton("load shared video 5", m_app->m_loadSharedVideo4Flag);
    m_gui.addButton("load shared video 6", m_app->m_loadSharedVideo5Flag);
    m_gui.addButton("load shared video 7", m_app->m_loadSharedVideo6Flag);
    m_gui.addButton("load shared video 8", m_app->m_loadSharedVideo7Flag);
    m_gui.addTitle("Timeline");
    m_gui.addToggle("use timeline", m_app->useTimeline);
    m_gui.addSlider("timeline seconds", m_app->timelineDurationSeconds, 10.0, 1200.0);

    // Page One
    m_gui.addPage("Page 1");
    m_gui.addTitle("surface");
    m_gui.addToggle("show/hide", m_dummyBool);
    m_gui.addToggle("use timeline", m_app->useTimeline);
    m_gui.addSlider("timeline seconds", m_app->timelineDurationSeconds, 10.0, 1200.0);
    m_gui.addToggle("use timeline tint", m_dummyBool);
    m_gui.addToggle("use timeline color", m_dummyBool);
    m_gui.addToggle("use timeline alpha", m_dummyBool);
    m_gui.addToggle("use timeline for slides", m_dummyBool);
    #ifdef WITH_SYPHON
    m_gui.addToggle("use Syphon", m_dummyBool);
    m_gui.addSlider("syphon origin X", m_dummyFloat, -1600, 1600);
    m_gui.addSlider("syphon origin Y", m_dummyFloat, -1600, 1600);
    m_gui.addSlider("syphon scale X", m_dummyFloat, 0.1, 10.0);
    m_gui.addSlider("syphon scale Y", m_dummyFloat, 0.1, 10.0);
    #endif
    //ofFloatColor& f = ofFloatColor(0,0,0);
    m_gui.addSlider("scale X", m_dummyFloat, 0.1, 10.0);
    m_gui.addSlider("scale Y", m_dummyFloat, 0.1, 10.0);
    m_gui.addToggle("fit to quad size", m_dummyBool);
    m_gui.addToggle("keep aspect ratio", m_dummyBool);
    m_gui.addToggle("H mirror", m_dummyBool);
    m_gui.addToggle("V mirror", m_dummyBool);
    m_gui.addColorPicker("colorize",&m_dummyFloat);
    m_gui.addSlider("hue", m_dummyFloat, 0.0f, 1.0f);
    m_gui.addSlider("saturation", m_dummyFloat, 0.0f, 1.0f);
    m_gui.addSlider("luminance", m_dummyFloat, 0.0, 1.0f);
    m_gui.addTitle("Blending modes");
    m_gui.addToggle("blending on/off", m_dummyBool);
    std::string blendModesArray[] = {"None", "Normal Alpha-Blending", "Additive (with Alpha)", "Subtractive (with Alpha)", "Multiply", "Screen"};
    m_gui.addComboBox("blending mode", m_dummyInt, 6, blendModesArray);

    m_gui.addTitle("Solid color").setNewColumn(true);
    m_gui.addToggle("solid bg on/off", m_dummyBool);
    m_gui.addColorPicker("Color", &m_dummyFloat);
    m_gui.addToggle("transition color",m_dummyBool);
    m_gui.addColorPicker("second Color", &m_dummyFloat);
    m_gui.addSlider("trans duration", m_dummyFloat, 0.1, 60.0);

    m_gui.addTitle("Image");
    m_gui.addToggle("image on/off", m_dummyBool);
    m_gui.addButton("load image", m_app->m_loadImageFlag);

    if (m_app->m_cameras.size() > 0)
    {
        m_gui.addTitle("Camera");
        m_gui.addToggle("cam on/off", m_dummyBool);
        if(m_app->m_cameras.size() > 1)
        {
            m_gui.addComboBox("select camera", m_dummyInt, m_app->m_cameras.size(), &(m_app->m_cameraIds[0]));
        }        
    }

    m_gui.addTitle("Video");
    m_gui.addToggle("video on/off", m_dummyBool);
    m_gui.addButton("load video", m_app->m_loadVideoFlag);
    m_gui.addSlider("video volume", m_dummyFloat, 0.0, 1.0);
    m_gui.addSlider("video speed", m_dummyFloat, -2.0, 4.0);
    m_gui.addToggle("video loop", m_dummyBool);    

    m_gui.addTitle("Shared Videos");
    m_gui.addToggle("shared video on/off", m_dummyBool);
    m_gui.addToggle("shared video tiling", m_dummyBool);
    m_gui.addSlider("shared video", m_dummyInt, 1, 8);

    m_gui.addTitle("Slideshow").setNewColumn(true);
    m_gui.addToggle("slideshow on/off", m_dummyBool);
    m_gui.addButton("load slideshow", m_app->m_loadSlideshowFlag);
    m_gui.addSlider("slide duration", m_dummyFloat, 0.1, 15.0);
    m_gui.addToggle("fade transitions", m_dummyBool);

    m_gui.addTitle("Greenscreen").setNewColumn(false);
    m_gui.addToggle("greenscreen on/off", m_dummyBool);
    m_gui.addSlider("g-screen threshold", m_dummyFloat, 0.0, 255.0);
    m_gui.addColorPicker("greenscreen col", &m_dummyFloat);

    m_gui.addTitle("Mask");
    m_gui.addToggle("mask on/off", m_dummyBool);
    m_gui.addToggle("invert mask", m_dummyBool);
    m_gui.addToggle("draw mask outline", m_dummyBool);


    // Page Two
    m_gui.addPage("Page 2");
    m_gui.addTitle("Edge blending");
    m_gui.addToggle("edge blend on/off", m_dummyBool);
    m_gui.addSlider("power", m_dummyFloat, 0.0, 4.0);
    m_gui.addSlider("gamma", m_dummyFloat, 0.0, 4.0);
    m_gui.addSlider("luminosity", m_dummyFloat, -4.0, 4.0);
    m_gui.addSlider("left edge", m_dummyFloat, 0.0, 0.5);
    m_gui.addSlider("right edge", m_dummyFloat, 0.0, 0.5);
    m_gui.addSlider("top edge", m_dummyFloat, 0.0, 0.5);
    m_gui.addSlider("bottom edge", m_dummyFloat, 0.0, 0.5);

    m_gui.addTitle("Content placement");
    m_gui.addSlider("X displacement", m_dummyInt, -1600, 1600);
    m_gui.addSlider("Y displacement", m_dummyInt, -1600, 1600);
    m_gui.addSlider("Width", m_dummyInt, 0, 2400);
    m_gui.addSlider("Height", m_dummyInt, 0, 2400);
    m_gui.addButton("Reset", m_app->m_resetCurrentQuadFlag);

    m_gui.addTitle("Surface deformation").setNewColumn(true);
    m_gui.addToggle("surface deform.", m_dummyBool);
    m_gui.addToggle("use bezier", m_dummyBool);
    m_gui.addToggle("use grid", m_dummyBool);
    m_gui.addSlider("grid rows", m_dummyInt, 2, 15);
    m_gui.addSlider("grid columns", m_dummyInt, 2, 20);
    m_gui.addButton("spherize light", m_app->m_bezierSpherizeQuadFlag);
    m_gui.addButton("spherize strong", m_app->m_bezierSpherizeQuadStrongFlag);
    m_gui.addButton("reset bezier warp", m_app->m_bezierResetQuadFlag);


    #ifdef WITH_KINECT
    if(m_app->m_isKinectInitialized)
    {
        m_gui.addTitle("Kinect").setNewColumn(true);
        m_gui.addToggle("use kinect", m_dummyBool);
        m_gui.addToggle("show kinect image", m_dummyBool);
        m_gui.addToggle("show kinect gray image", m_dummyBool);
        m_gui.addToggle("use kinect as mask", m_dummyBool);
        m_gui.addToggle("kinect blob detection", m_dummyBool);
        m_gui.addToggle("blob curved contour", m_dummyBool);
        m_gui.addSlider("kinect scale X", m_dummyFloat, 0.1, 10.0);
        m_gui.addSlider("kinect scale Y", m_dummyFloat, 0.1, 10.0);
        m_gui.addColorPicker("kinect color", &m_dummyFloat);
        m_gui.addSlider("near threshold", m_dummyInt, 0, 255);
        m_gui.addSlider("far threshold", m_dummyInt, 0, 255);
        m_gui.addSlider("kinect tilt angle", m_app->kinect.kinectAngle, -30, 30);
        m_gui.addSlider("kinect image blur", m_dummyInt, 0, 10);
        m_gui.addSlider("blob min area", m_dummyFloat, 0.01, 1.0);
        m_gui.addSlider("blob max area", m_dummyFloat, 0.0, 1.0);
        m_gui.addSlider("blob contour smooth", m_dummyInt, 0, 20);
        m_gui.addSlider("blob simplify", m_dummyFloat, 0.0, 2.0);
        m_gui.addButton("close connection", m_dummyBool);
        m_gui.addButton("reopen connection", m_dummyBool);
    }
    #endif

    // Page Three
    m_gui.addPage("Page 3");
    m_gui.addTitle("Corner 0");
    m_gui.addSlider("corner 0 X", m_dummyFloat, -1.0, 2.0);
    m_gui.addSlider("corner 0 Y", m_dummyFloat, -1.0, 2.0);
    m_gui.addTitle("Corner 3");
    m_gui.addSlider("corner 3 X", m_dummyFloat, -1.0, 2.0);
    m_gui.addSlider("corner 3 Y", m_dummyFloat, -1.0, 2.0);

    m_gui.addTitle("Corner 1").setNewColumn(true);
    m_gui.addSlider("corner 1 X", m_dummyFloat, -1.0, 2.0);
    m_gui.addSlider("corner 1 Y", m_dummyFloat, -1.0, 2.0);
    m_gui.addTitle("Corner 2");
    m_gui.addSlider("corner 2 X", m_dummyFloat, -1.0, 2.0);
    m_gui.addSlider("corner 2 Y", m_dummyFloat, -1.0, 2.0);

    m_gui.addTitle("Crop").setNewColumn(true);
    m_gui.addToggle("mask on/off", m_dummyBool);
    m_gui.addTitle("Rectangular crop");
    m_gui.addSlider("top", m_dummyFloat, 0, 1.0);
    m_gui.addSlider("right", m_dummyFloat, 0, 1.0);
    m_gui.addSlider("bottom", m_dummyFloat, 0, 1.0);
    m_gui.addSlider("left", m_dummyFloat, 0, 1.0);
    m_gui.addTitle("Circular crop");
    m_gui.addSlider("center X", m_dummyFloat, 0, 1.0);
    m_gui.addSlider("center Y", m_dummyFloat, 0, 1.0);
    m_gui.addSlider("radius", m_dummyFloat, 0, 2.0);

    // then we set displayed gui page to the one corresponding to active quad and show the gui
//    m_gui.setPage((activeQuad*3)+2);
    m_gui.show();
}


void GUI::updatePages(quad& activeQuad)
{
    ofxSimpleGuiPage& firstPage = m_gui.page("Page 1");

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("show/hide"))->value = &activeQuad.isOn;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("use timeline tint"))->value = &activeQuad.bTimelineTint;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("use timeline color"))->value = &activeQuad.bTimelineColor;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("use timeline alpha"))->value = &activeQuad.bTimelineAlpha;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("use timeline for slides"))->value = &activeQuad.bTimelineSlideChange;
    #ifdef WITH_SYPHON
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("use Syphon"))->value = &activeQuad.bSyphon;
    dynamic_cast<ofxSimpleGuiSliderInt*>(firstPage.findControlByName("syphon origin X"))->value = &activeQuad.syphonPosX;
    dynamic_cast<ofxSimpleGuiSliderInt*>(firstPage.findControlByName("syphon origin Y"))->value = &activeQuad.syphonPosY;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("syphon scale X"))->value = &activeQuad.syphonScaleX;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("syphon scale Y"))->value = &activeQuad.syphonScaleY;no
    #endif
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("image on/off"))->value = &activeQuad.imgBg;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("scale X"))->value = &activeQuad.imgMultX;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("scale Y"))->value = &activeQuad.imgMultY;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("fit to quad size"))->value = &activeQuad.imageFit;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("keep aspect ratio"))->value = &activeQuad.imageKeepAspect;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("H mirror"))->value = &activeQuad.imgHFlip;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("V mirror"))->value = &activeQuad.imgVFlip;
    dynamic_cast<ofxSimpleGuiColorPicker*>(firstPage.findControlByName("colorize"))->value = &activeQuad.imgColorize.r;

    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("hue"))->value = &activeQuad.hue;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("saturation"))->value = &activeQuad.saturation;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("luminance"))->value = &activeQuad.luminance;

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("blending on/off"))->value = &activeQuad.bBlendModes;
    dynamic_cast<ofxSimpleGuiComboBox*>(firstPage.findControlByName("blending mode"))->m_selectedChoice = &activeQuad.blendMode;

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("solid bg on/off"))->value = &activeQuad.colorBg;
    dynamic_cast<ofxSimpleGuiColorPicker*>(firstPage.findControlByName("Color"))->value = &activeQuad.bgColor.r;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("transition color"))->value = &activeQuad.transBg;
    dynamic_cast<ofxSimpleGuiColorPicker*>(firstPage.findControlByName("second Color"))->value = &activeQuad.secondColor.r;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("trans duration"))->value = &activeQuad.transDuration;

    if (m_app->m_cameras.size() > 0)
    {
        dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("cam on/off"))->value = &activeQuad.camBg;
        if(m_app->m_cameras.size() > 1)
        {
            dynamic_cast<ofxSimpleGuiComboBox*>(firstPage.findControlByName("select camera"))->m_selectedChoice = &activeQuad.camNumber;
        }
    }

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("video on/off"))->value = &activeQuad.videoBg;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("video volume"))->value = &activeQuad.videoVolume;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("video speed"))->value = &activeQuad.videoSpeed;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("video loop"))->value = &activeQuad.videoLoop;   
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("shared video on/off"))->value = &activeQuad.sharedVideoBg;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("shared video tiling"))->value = &activeQuad.sharedVideoTiling;
    dynamic_cast<ofxSimpleGuiSliderInt*>(firstPage.findControlByName("shared video"))->value = &activeQuad.sharedVideoNum;

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("slideshow on/off"))->value = &activeQuad.slideshowBg;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("slide duration"))->value = &activeQuad.slideshowSpeed;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("fade transitions"))->value = &activeQuad.bFadeTransitions;

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("greenscreen on/off"))->value = &activeQuad.useGreenscreen;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("g-screen threshold"))->value = &activeQuad.thresholdGreenscreen;
    dynamic_cast<ofxSimpleGuiColorPicker*>(firstPage.findControlByName("greenscreen col"))->value = &activeQuad.colorGreenscreen.r;


    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("mask on/off"))->value = &activeQuad.bMask;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("invert mask"))->value = &activeQuad.maskInvert;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("draw mask outline"))->value = &activeQuad.bDrawMaskOutline;



    // Second Page
    ofxSimpleGuiPage& secondPage = m_gui.page("Page 2");
    dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("edge blend on/off"))->value = &activeQuad.edgeBlendBool;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("power"))->value = &activeQuad.edgeBlendExponent;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("gamma"))->value = &activeQuad.edgeBlendGamma;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("luminosity"))->value = &activeQuad.edgeBlendLuminance;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("left edge"))->value = &activeQuad.edgeBlendAmountSin;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("right edge"))->value = &activeQuad.edgeBlendAmountDx;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("top edge"))->value = &activeQuad.edgeBlendAmountTop;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("bottom edge"))->value = &activeQuad.edgeBlendAmountBottom;

    dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("X displacement"))->value = &activeQuad.quadDispX;
    dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("Y displacement"))->value = &activeQuad.quadDispY;
    dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("Width"))->value = &activeQuad.quadW;
    dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("Height"))->value = &activeQuad.quadH;

    dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("surface deform."))->value = &activeQuad.bDeform;
    dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("use bezier"))->value = &activeQuad.bBezier;
    dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("use grid"))->value = &activeQuad.bGrid;
    dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("grid rows"))->value = &activeQuad.gridRows;
    dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("grid columns"))->value = &activeQuad.gridColumns;

    #ifdef WITH_KINECT
    if(m_app->m_isKinectInitialized)
    {
        dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("use kinect"))->value = &activeQuad.kinectBg;
        dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("show kinect image"))->value = &activeQuad.kinectImg;
        dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("show kinect gray image"))->value = &activeQuad.getKinectGrayImage;
        dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("use kinect as mask"))->value = &activeQuad.kinectMask;
        dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("kinect blob detection"))->value = &activeQuad.getKinectContours;
        dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("blob curved contour"))->value = &activeQuad.kinectContourCurved;
        dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("kinect scale X"))->value = &activeQuad.kinectMultX;
        dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("kinect scale Y"))->value = &activeQuad.kinectMultY;
        dynamic_cast<ofxSimpleGuiColorPicker*>(secondPage.findControlByName("kinect color"))->value = &activeQuad.kinectColorize.r;
        dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("near threshold"))->value = &activeQuad.nearDepthTh;
        dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("far threshold"))->value = &activeQuad.farDepthTh;
        dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("kinect image blur"))->value = &activeQuad.kinectBlur;
        dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("blob min area"))->value = &activeQuad.kinectContourMin;
        dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("blob max area"))->value = &activeQuad.kinectContourMax;
        dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("blob contour smooth"))->value = &activeQuad.kinectContourSmooth;
        dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("blob simplify"))->value = &activeQuad.kinectContourSimplify;
    }
    #endif

    ofxSimpleGuiPage& thirdPage = m_gui.page("Page 3");
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 0 X"))->value = &activeQuad.corners[0].x;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 0 Y"))->value = &activeQuad.corners[0].y;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 3 X"))->value = &activeQuad.corners[3].x;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 3 Y"))->value = &activeQuad.corners[3].y;

    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 1 X"))->value = &activeQuad.corners[1].x;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 1 Y"))->value = &activeQuad.corners[1].y;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 2 X"))->value = &activeQuad.corners[2].x;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 2 Y"))->value = &activeQuad.corners[2].y;

    dynamic_cast<ofxSimpleGuiToggle*>(thirdPage.findControlByName("mask on/off"))->value = &activeQuad.bMask;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("top"))->value = &activeQuad.crop[0];
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("right"))->value = &activeQuad.crop[1];
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("bottom"))->value = &activeQuad.crop[2];
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("left"))->value = &activeQuad.crop[3];
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("center X"))->value = &activeQuad.circularCrop[0];
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("center Y"))->value = &activeQuad.circularCrop[1];
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("radius"))->value = &activeQuad.circularCrop[2];
}


void GUI::nextPage()
{
    m_gui.nextPage();
}


void GUI::prevPage()
{
    m_gui.prevPage();
}


void GUI::showPage(int i)
{
    m_gui.setPage(i);
}


void GUI::toggleDraw()
{
	m_gui.toggleDraw();
}


void GUI::show()
{
	m_gui.show();
}


void GUI::hide()
{
	m_gui.hide();
}


bool GUI::isOn()
{
	return m_gui.isOn();
}


std::vector<ofxSimpleGuiPage*>&	GUI::getPages()
{
	return m_gui.getPages();
}

void GUI::draw()
{
    m_gui.draw();
}

