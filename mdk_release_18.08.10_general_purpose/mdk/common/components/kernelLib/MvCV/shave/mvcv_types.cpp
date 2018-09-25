///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     MvCV types
///
/// This is the implementation of MvCV types
///

#include <mvcv_types.h>

mvCvSize::mvCvSize()
{
}

mvCvSize::mvCvSize(int width0, int height0)
{
    width = width0;
    height = height0;
}

mvCvSize mvcvSize( int width, int height )
{
    mvCvSize s;

    s.width = width;
    s.height = height;

    return s;
}

ClSize clSize( int width, int height )
{
    ClSize s;

    s.x = width;
    s.y = height;

    return s;
}

ClSizeW clSizeW( int width, int height )
{
    ClSizeW s;

    s.x = width;
    s.y = height;

    return s;
}

mvScalar mvcvScalar(float s0, float s1, float s2, float s3)
{
    mvScalar s;

    s[0] = s0;
    s[1] = s1;
    s[2] = s2;
    s[3] = s3;

    return s;
}

mvCvPoint mvcvPoint( int x, int y )
{
    mvCvPoint p;

    p.x = x;
    p.y = y;

    return p;
}

mvCvPointW mvcvPointW( int x, int y )
{
    mvCvPointW p;

    p.x = x;
    p.y = y;

    return p;
}

mvCvPoint2D32f mvcvPoint2D32f( float x, float y )
{
    mvCvPoint2D32f p;

    p.x = (float)x;
    p.y = (float)y;

    return p;
}

mvCvPoint2D32fW mvcvPoint2D32fW( float x, float y )
{
    mvCvPoint2D32fW p;

    p.x = (float)x;
    p.y = (float)y;

    return p;
}

mvCvRect mvcvRect( int x, int y, int width, int height )
{
    mvCvRect r;

    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;

    return r;
}

mvCvTermCriteria mvcvTermCriteria( int type, int max_iter, float epsilon )
{
    mvCvTermCriteria t;

    t.type = type;
    t.max_iter = max_iter;
    t.epsilon = (float)epsilon;

    return t;
}

int mvcvRound( float value )
{
   int a;
   a =   (int)(value + (value >= 0 ? 0.5 : -0.5));
   return a;
}

int mvcvFloor( float value )
{
    int i = mvcvRound(value);
    mvCv32suf diff;
    diff.f = (float)(value - i);
    return i - (diff.i < 0);
}

int mvcvCeil(float value)
{
    int i = mvcvRound(value);
    mvCv32suf diff;
    diff.f = (float)(i - value);
    return i + (diff.i < 0);
}

int mvcvAlign( int size, int align )
{
    return (size + align - 1) & -align;
}
