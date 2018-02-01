#ifndef GUI_INCLUDE
#define GUI_INCLUDE

#include "ofxSimpleGuiToo.h"
#include "quad.h"

class ofApp;

class GUI
{
public:
    GUI(ofApp* app);

    void setupPages();
    void updatePages(quad& activeQuad);

    void nextPage();
    void prevPage();
    void showPage(int i);               // 1 based index of page

    void toggleDraw();
    void show();
    void hide();
    bool isOn();

    std::vector<ofxSimpleGuiPage*>&    getPages();

    void draw();


private:
    ofxSimpleGuiToo       m_gui;
    ofxSimpleGuiConfig    m_config;
    ofApp*                m_app;
    bool                  m_dummyBool;
    float                 m_dummyFloat;
    int                   m_dummyInt;
};

#endif // GUI_INCLUDE
