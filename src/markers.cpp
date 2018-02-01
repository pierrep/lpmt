#include "quad.h"
#include "Util.hpp"

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
