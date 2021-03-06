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
	m_gui.getConfig()->toggleHeight = 16;
	m_gui.getConfig()->buttonHeight = 18;
	m_gui.getConfig()->sliderTextHeight = 18;
	m_gui.getConfig()->titleHeight = 18;

	m_gui.getConfig()->fullActiveColor = 0x648B96; // bluish-grey
	m_gui.getConfig()->textColor = 0xFFFFFF;       // white
	m_gui.getConfig()->textBGOverColor = 0x777788; // grey

	/* set up font */
	m_gui.getConfig()->bUseFont = true;
	m_gui.getConfig()->ttf.load("type/Roboto-Regular.ttf", 9);

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
    m_gui.addTitle("Shared videos",true);
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
    m_gui.addPage("PAGE 1");
    m_gui.addTitle("Surface");
    m_gui.addToggle("Enable", m_dummyBool);
    m_gui.addToggle("Use Timeline", m_app->useTimeline);
    m_gui.addSlider("Timeline seconds", m_app->timelineDurationSeconds, 10, 600);
    m_gui.addToggle("Use Timeline tint", m_dummyBool);
    m_gui.addToggle("Use Timeline color", m_dummyBool);
    m_gui.addToggle("Use Timeline alpha", m_dummyBool);
    m_gui.addToggle("Use Timeline for slides", m_dummyBool);
    #ifdef WITH_SYPHON
    m_gui.addToggle("Use Syphon", m_dummyBool);
    m_gui.addSlider("Syphon origin X", m_dummyFloat, -1600, 1600);
    m_gui.addSlider("Syphon origin Y", m_dummyFloat, -1600, 1600);
    m_gui.addSlider("Syphon scale X", m_dummyFloat, 0.1, 10.0);
    m_gui.addSlider("Syphon scale Y", m_dummyFloat, 0.1, 10.0);
    #endif

    m_gui.addSlider("Scale X", m_dummyFloat, 0.1, 5.0);
    m_gui.addSlider("Scale Y", m_dummyFloat, 0.1, 5.0);
    m_gui.addToggle("Fit to quad size", m_dummyBool);
    m_gui.addToggle("Keep aspect ratio", m_dummyBool);
    m_gui.addToggle("H mirror", m_dummyBool);
    m_gui.addToggle("V mirror", m_dummyBool);
    m_gui.addColorPicker("Colorize",&m_dummyFloat);
    m_gui.addSlider("Hue", m_dummyFloat, 0.0f, 1.0f);
    m_gui.addSlider("Saturation", m_dummyFloat, 0.0f, 1.0f);
    m_gui.addSlider("Luminance", m_dummyFloat, 0.0, 1.0f);
    m_gui.addTitle("Blending modes");
    m_gui.addToggle("Blending on/off", m_dummyBool);
    std::string blendModesArray[] = {"None", "Normal Alpha-Blending", "Additive (with Alpha)", "Subtractive (with Alpha)", "Multiply", "Screen"};
    m_gui.addComboBox("Blending mode", m_dummyInt, 6, blendModesArray);

    m_gui.addTitle("Solid colour").setNewColumn(true);
    m_gui.addToggle("Solid colour on/off", m_dummyBool);

    m_gui.addTitle("Image");
    m_gui.addToggle("Image on/off", m_dummyBool);
    m_gui.addButton("Load image", m_app->m_loadImageFlag);

    if (m_app->m_cameras.size() > 0)
    {
        m_gui.addTitle("Camera");
        m_gui.addToggle("Camera on/off", m_dummyBool);
        if(m_app->m_cameras.size() > 1)
        {     
            m_gui.addComboBox("Select Camera", m_dummyInt, m_app->m_cameras.size(), &(m_app->m_cameraIds[0]));
        }        
    }

    m_gui.addTitle("Video");
    m_gui.addToggle("Video on/off", m_dummyBool);
    m_gui.addButton("Load Video", m_app->m_loadVideoFlag);
    m_gui.addSlider("Video Volume", m_dummyFloat, 0.0, 1.0);
    m_gui.addSlider("Video Speed", m_dummyFloat, -2.0, 4.0);
    m_gui.addToggle("Video Loop", m_dummyBool);

    m_gui.addTitle("Shared Videos");
    m_gui.addToggle("Shared Video on/off", m_dummyBool);
    m_gui.addToggle("Shared Video tiling", m_dummyBool);
    m_gui.addSlider("Shared Video", m_dummyInt, 1, 8);

    m_gui.addTitle("Slideshow").setNewColumn(true);
    m_gui.addToggle("Slideshow on/off", m_dummyBool);
    m_gui.addButton("Load slideshow", m_app->m_loadSlideshowFlag);
    m_gui.addSlider("Slide duration", m_dummyFloat, 0.1, 15.0);
    m_gui.addToggle("Fade transitions", m_dummyBool);

    m_gui.addTitle("Greenscreen").setNewColumn(false);
    m_gui.addToggle("Greenscreen on/off", m_dummyBool);
    m_gui.addSlider("Greenscreen threshold", m_dummyFloat, 0.0, 255.0);
    m_gui.addColorPicker("Greenscreen colour", &m_dummyFloat);

    m_gui.addTitle("Mask");
    m_gui.addToggle("Mask on/off", m_dummyBool);
    m_gui.addToggle("Invert mask", m_dummyBool);
    m_gui.addToggle("Draw mask outline", m_dummyBool);


    // Page Two
    m_gui.addPage("PAGE 2");
    m_gui.addTitle("Edge blending");
    m_gui.addToggle("Edge blend on/off", m_dummyBool);
    m_gui.addSlider("Power", m_dummyFloat, 0.0, 4.0);
    m_gui.addSlider("Gamma", m_dummyFloat, 0.0, 4.0);
    m_gui.addSlider("Luminosity", m_dummyFloat, -4.0, 4.0);
    m_gui.addSlider("Left edge", m_dummyFloat, 0.0, 0.5);
    m_gui.addSlider("Right edge", m_dummyFloat, 0.0, 0.5);
    m_gui.addSlider("Top edge", m_dummyFloat, 0.0, 0.5);
    m_gui.addSlider("Bottom edge", m_dummyFloat, 0.0, 0.5);

    m_gui.addTitle("Content placement");
    m_gui.addSlider("X displacement", m_dummyInt, -1600, 1600);
    m_gui.addSlider("Y displacement", m_dummyInt, -1600, 1600);
    m_gui.addSlider("Width", m_dummyInt, 0, 2400);
    m_gui.addSlider("Height", m_dummyInt, 0, 2400);
    m_gui.addButton("Reset", m_app->m_resetCurrentQuadFlag);

    m_gui.addTitle("Grid Deformation").setNewColumn(true);
    m_gui.addToggle("Use grid", m_dummyBool);
    m_gui.addSlider("Grid rows", m_dummyInt, 2, 15);
    m_gui.addSlider("Grid columns", m_dummyInt, 2, 20);

    m_gui.addTitle("Bezier Deformation");
    m_gui.addToggle("Use bezier", m_dummyBool);
    m_gui.addButton("Spherize light", m_app->m_bezierSpherizeQuadFlag);
    m_gui.addButton("Spherize strong", m_app->m_bezierSpherizeQuadStrongFlag);
    m_gui.addButton("Reset bezier warp", m_app->m_bezierResetQuadFlag);


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
    m_gui.addPage("PAGE 3");
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
    m_gui.addToggle("Mask on/off", m_dummyBool);
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
    ofxSimpleGuiPage& firstPage = m_gui.page("PAGE 1");

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Enable"))->value = &activeQuad.isOn;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Use Timeline tint"))->value = &activeQuad.bTimelineTint;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Use Timeline color"))->value = &activeQuad.bTimelineColor;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Use Timeline alpha"))->value = &activeQuad.bTimelineAlpha;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Use Timeline for slides"))->value = &activeQuad.bTimelineSlideChange;
    #ifdef WITH_SYPHON
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Use Syphon"))->value = &activeQuad.bSyphon;
    dynamic_cast<ofxSimpleGuiSliderInt*>(firstPage.findControlByName("Syphon origin X"))->value = &activeQuad.syphonPosX;
    dynamic_cast<ofxSimpleGuiSliderInt*>(firstPage.findControlByName("Syphon origin Y"))->value = &activeQuad.syphonPosY;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("Syphon scale X"))->value = &activeQuad.syphonScaleX;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("Syphon scale Y"))->value = &activeQuad.syphonScaleY;no
    #endif

    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("Scale X"))->value = &activeQuad.imgMultX;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("Scale Y"))->value = &activeQuad.imgMultY;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Fit to quad size"))->value = &activeQuad.imageFit;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Keep aspect ratio"))->value = &activeQuad.imageKeepAspect;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("H mirror"))->value = &activeQuad.imgHFlip;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("V mirror"))->value = &activeQuad.imgVFlip;
    dynamic_cast<ofxSimpleGuiColorPicker*>(firstPage.findControlByName("Colorize"))->value = &activeQuad.imgColorize.r;

    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("Hue"))->value = &activeQuad.hue;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("Saturation"))->value = &activeQuad.saturation;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("Luminance"))->value = &activeQuad.luminance;

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Blending on/off"))->value = &activeQuad.bBlendModes;
    dynamic_cast<ofxSimpleGuiComboBox*>(firstPage.findControlByName("Blending mode"))->m_selectedChoice = &activeQuad.blendMode;

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Solid colour on/off"))->value = &activeQuad.colorBg;

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Image on/off"))->value = &activeQuad.imgBg;

    if (m_app->m_cameras.size() > 0)
    {
        dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Camera on/off"))->value = &activeQuad.camBg;
        if(m_app->m_cameras.size() > 1)
        {
            dynamic_cast<ofxSimpleGuiComboBox*>(firstPage.findControlByName("Select Camera"))->m_selectedChoice = &activeQuad.camNumber;
        }
    }

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Video on/off"))->value = &activeQuad.videoBg;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("Video Volume"))->value = &activeQuad.videoVolume;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("Video Speed"))->value = &activeQuad.videoSpeed;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Video Loop"))->value = &activeQuad.videoLoop;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Shared Video on/off"))->value = &activeQuad.sharedVideoBg;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Shared Video tiling"))->value = &activeQuad.sharedVideoTiling;
    dynamic_cast<ofxSimpleGuiSliderInt*>(firstPage.findControlByName("Shared Video"))->value = &activeQuad.sharedVideoNum;

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Slideshow on/off"))->value = &activeQuad.slideshowBg;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("Slide duration"))->value = &activeQuad.slideshowSpeed;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Fade transitions"))->value = &activeQuad.bFadeTransitions;

    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Greenscreen on/off"))->value = &activeQuad.bUseGreenscreen;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(firstPage.findControlByName("Greenscreen threshold"))->value = &activeQuad.thresholdGreenscreen;
    dynamic_cast<ofxSimpleGuiColorPicker*>(firstPage.findControlByName("Greenscreen colour"))->value = &activeQuad.colorGreenscreen.r;


    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Mask on/off"))->value = &activeQuad.bMask;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Invert mask"))->value = &activeQuad.maskInvert;
    dynamic_cast<ofxSimpleGuiToggle*>(firstPage.findControlByName("Draw mask outline"))->value = &activeQuad.bDrawMaskOutline;



    // Second Page
    ofxSimpleGuiPage& secondPage = m_gui.page("PAGE 2");
    dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("Edge blend on/off"))->value = &activeQuad.bEdgeBlend;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("Power"))->value = &activeQuad.edgeBlendExponent;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("Gamma"))->value = &activeQuad.edgeBlendGamma;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("Luminosity"))->value = &activeQuad.edgeBlendLuminance;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("Left edge"))->value = &activeQuad.edgeBlendAmountSin;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("Right edge"))->value = &activeQuad.edgeBlendAmountDx;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("Top edge"))->value = &activeQuad.edgeBlendAmountTop;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(secondPage.findControlByName("Bottom edge"))->value = &activeQuad.edgeBlendAmountBottom;

    dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("X displacement"))->value = &activeQuad.quadDispX;
    dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("Y displacement"))->value = &activeQuad.quadDispY;
    dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("Width"))->value = &activeQuad.quadW;
    dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("Height"))->value = &activeQuad.quadH;

    dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("Use bezier"))->value = &activeQuad.bBezier;
    dynamic_cast<ofxSimpleGuiToggle*>(secondPage.findControlByName("Use grid"))->value = &activeQuad.bGrid;
    dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("Grid rows"))->value = &activeQuad.gridRows;
    dynamic_cast<ofxSimpleGuiSliderInt*>(secondPage.findControlByName("Grid columns"))->value = &activeQuad.gridColumns;

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

    ofxSimpleGuiPage& thirdPage = m_gui.page("PAGE 3");
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 0 X"))->value = &activeQuad.corners[0].x;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 0 Y"))->value = &activeQuad.corners[0].y;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 3 X"))->value = &activeQuad.corners[3].x;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 3 Y"))->value = &activeQuad.corners[3].y;

    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 1 X"))->value = &activeQuad.corners[1].x;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 1 Y"))->value = &activeQuad.corners[1].y;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 2 X"))->value = &activeQuad.corners[2].x;
    dynamic_cast<ofxSimpleGuiSliderFloat*>(thirdPage.findControlByName("corner 2 Y"))->value = &activeQuad.corners[2].y;

    dynamic_cast<ofxSimpleGuiToggle*>(thirdPage.findControlByName("Mask on/off"))->value = &activeQuad.bMask;
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
    ofPushStyle();
    m_gui.draw();
    ofPopStyle();
}

