#include "Util.hpp"
#include "ofAppRunner.h"


const float Util::EPSILON = 0.0001f;


bool Util::pointInTriangle2D(const ofPoint& vertex1, const ofPoint& vertex2, const ofPoint& vertex3, const ofPoint& point)
{
    const float denominator = ((vertex2.y - vertex3.y) * (vertex1.x - vertex3.x) + (vertex3.x - vertex2.x) * (vertex1.y - vertex3.y));
    if(denominator == 0.f)
    {
        // if the denominator is zero, two points of the triangle are on top of each other
        // that makes this test obsolete, because the triangle has no area
        // this also prevents devision by zero
        return false;
    }

    const float a = ((vertex2.y - vertex3.y) * (point.x - vertex3.x) + (vertex3.x - vertex2.x) * (point.y - vertex3.y)) / denominator;
    const float b = ((vertex3.y - vertex1.y) * (point.x - vertex3.x) + (vertex1.x - vertex3.x) * (point.y - vertex3.y)) / denominator;
    const float c = 1.f - a - b;

    return (0 - EPSILON) <= a && a <= (1 + EPSILON) && (0 - EPSILON) <= b && b <= (1 + EPSILON) && (0 - EPSILON) <= c && c <= (1 + EPSILON);
}


ofPoint Util::scalePointToPixel(const ofPoint& point)
{
    return ofPoint(point.x * ofGetWidth(), point.y * ofGetHeight());
}


ofPoint Util::normalizePoint(const ofPoint& point)
{
    return ofPoint(point.x / ofGetWidth(), point.y / ofGetHeight());
}
