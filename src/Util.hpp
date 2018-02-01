#ifndef UTIL_INCLUDE
#define UTIL_INCLUDE

#include "ofTypes.h"

class Util
{
public:
    static const float EPSILON;

    static bool pointInTriangle2D(const ofPoint& vertex1, const ofPoint& vertex2, const ofPoint& vertex3, const ofPoint& point);

    static ofPoint scalePointToPixel(const ofPoint& point);
    static ofPoint normalizePoint(const ofPoint& point);
};

#endif // UTIL_INCLUDE
