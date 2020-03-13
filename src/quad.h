#pragma once

#include "ofMain.h"

//#define WITH_KINECT

#ifdef WITH_KINECT
#include "kinectManager.h"
#include "ofxOpenCv.h"
#endif

#ifdef WITH_SYPHON
#include "ofxSyphon.h"
#endif

class quad {

public:
    quad();
    void reset();
    void setup(ofPoint point1, ofPoint point2, ofPoint point3, ofPoint point4, ofShader& edgeBlendShader, ofShader& quadMaskShader, ofShader& surfaceShader, ofShader& fadeShader, vector<ofVideoGrabber>& cameras, ofTrueTypeFont& font);
    void update();
    void draw(vector<ofVideoPlayer>& sharedVideos);
    void drawSurface(vector<ofVideoPlayer>& sharedVideos);
    void drawDeformation(ofTexture& tex, bool alphaFix);
    void applyBlendmode();

    void gaussian_elimination(float* input, int n);
    void findHomography(ofPoint src[4], ofPoint dst[4], float homography[16]);
    ofMatrix4x4 findVectorHomography(ofPoint src[4], ofPoint dst[4]);
    ofPoint findWarpedPoint(ofPoint src[4], ofPoint dst[4], ofPoint point);
    ofPoint getWarpedPoint(ofPoint point);

    void loadImageFromFile(string imgName, string imgPath);
    void loadVideoFromFile(string videoName, string videoPath);
    void allocateFbo(int w, int h);
    void maskAddPoint(ofPoint point);
    void drawMaskMarkers();
    void drawGridMarkers();
    void drawBezierMarkers();
    void gridSurfaceSetup();
    void gridSurfaceUpdate(bool bRefresh);
    void bezierSurfaceSetup();
    void bezierSurfaceUpdate();
    //int getdir (string dir, vector<string> &files); // a func for reading a dir content to a vector of strings
    bool isValidContent(vector<ofVideoPlayer>& sharedVideos);
    void drawContent(float w, float h, vector<ofVideoPlayer>& sharedVideos);

#ifdef WITH_KINECT
    void setKinect(kinectManager* kinect);
#endif

#ifdef WITH_SYPHON
    void setSyphon(ofxSyphonClient& syphon);
#endif

    ofPoint corners[4];
    ofPoint center;

    ofPoint src[4];
    ofPoint dst[4];
    float crop[4];
    float circularCrop[3];

    GLfloat matrix[16]; //lets make a matrix for openGL - this will be the matrix that peforms the transformation

    /* img and video stuff */
    ofImage img;
    ofVideoPlayer video;
    ofTexture blank;
    ofTrueTypeFont ttf;

    int videoWidth;
    int videoHeight;

    ofFloatColor imgColorize;
    ofFloatColor kinectColorize;
    ofFloatColor timelineColor;

    float timelineRed;
    float timelineGreen;
    float timelineBlue;
    float timelineAlpha;

    bool bTimelineColor;
    bool bTimelineTint;
    bool bTimelineAlpha;
    bool bTimelineSlideChange;

    int bgAlpha;

    /* camera stuff */
    bool camAvailable;
    int camNumber;
    int prevCamNumber;

    int layer;

    float screenFactorX;
    float screenFactorY;
    float imgMultX;
    float imgMultY;
    float kinectMultX;
    float kinectMultY;
    float videoSpeed;
    float previousSpeed;
    float slideshowSpeed;
    float transDuration;
    float edgeBlendExponent;
    float edgeBlendAmountSin;
    float edgeBlendAmountDx;
    float edgeBlendAmountTop;
    float edgeBlendAmountBottom;
    float edgeBlendGamma;
    float edgeBlendLuminance;

    int quadNumber;

    bool initialized;
    bool isActive;
    bool isEditMode;
    bool isOn;
    bool isMaskSetup;
    bool colorBg;
    bool camBg;
    bool imgBg;
    bool videoBg;
    bool videoSound;
    bool videoLoop;

    //Shared videos
    bool sharedVideoBg;
    bool sharedVideoTiling;
    int sharedVideoNum;
    int sharedVideoId;

    bool slideshowBg;
    bool bFadeTransitions;
    bool imageFit;
    bool imageKeepAspect;
    bool kinectBg;
    bool kinectImg;
    bool kinectMask;
    int nearDepthTh;
    int farDepthTh;
    int kinectBlur;
    float kinectContourMin;
    float kinectContourMax;
    float kinectContourSimplify;
    int kinectContourSmooth;
    bool getKinectContours;
    bool getKinectGrayImage;
    bool kinectContourCurved;

    bool imgHFlip;
    bool imgVFlip;

    bool bEdgeBlend;

    bool bBlendModes;
    int blendMode;

    bool bMask;
    bool maskInvert;
    int maskMode;
    bool bDrawMaskOutline;

    bool bHighlightCorner;
    int highlightedCorner;

    bool bHighlightCenter;
    bool bHighlightRotation;

    float videoVolume;

    // Greenscreen
    bool bUseGreenscreen;
    float thresholdGreenscreen;
    ofFloatColor colorGreenscreen;

    int bgSlideshow;
    unsigned int currentSlideId;
    int fps;
    int slideFramesDuration;
    int slideTimer;

    vector<string> videos;
    vector<ofImage> slides;
    vector<ofVideoGrabber> cams;

    string bgImg;
    string bgVideo;
    string loadedImg;
    string loadedVideo;
    string loadedSlideshow;
    string slideshowName;

    // FBO's
    ofFbo quadFbo;
    ofFbo maskFbo;
    ofFbo targetFbo;
    ofFbo::Settings settings;
    ofFbo::Settings maskFboSettings;

    // Shaders
    ofShader* edgeBlendShader;
    ofShader* maskShader;
    ofShader* surfaceShader;
    ofShader* crossfadeShader;

    float hue;
    float saturation;
    float luminance;

    int quadDispX;
    int quadDispY;
    int quadW;
    int quadH;

    std::vector<ofPoint> m_maskPoints;
    bool bHighlightMaskPoint;
    int highlightedMaskPoint;

    bool isBezierSetup;
    bool bHighlightCtrlPoint;
    int highlightedCtrlPointRow;
    int highlightedCtrlPointCol;
    float bezierPoints[4][4][3];
    bool bBezier;
    float bezierCtrlPoints[4][4][3];

    bool bGrid;
    vector<vector<vector<float>>> gridPoints;
    //vector<GLfloat> gridCtrlPoints;
    int gridRows;
    int gridColumns;
    ofMesh gridMesh;

#ifdef WITH_KINECT
    kinectManager* quadKinect;
    ofxCvGrayscaleImage kinectThreshImage;
    ofxCvGrayscaleImage kinectContourImage;
    ofxCvContourFinder kinectContourFinder;
    ofPath kinectPath;
#endif

#ifdef WITH_SYPHON
    ofxSyphonClient* syphClientTex;
    bool bSyphon;
    float syphonPosX;
    float syphonPosY;
    float syphonScaleX;
    float syphonScaleY;
#endif
};
