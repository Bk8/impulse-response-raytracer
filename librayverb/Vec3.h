//
//  Vec3.h
//  ray
//
//  Created by reuben on 01/08/2012.
//  Copyright (c) 2012 Reuben Thomas. All rights reserved.
//

//  Pretty much stolen from Jacco Bikker's excelent raytracing tutorial found at flipcode.com

#ifndef ray_Vec3_h
#define ray_Vec3_h

#include <cmath>

//#include "JuceHeader.h"

using namespace std;

namespace Rayverb {

template <class T>
class Vec3 {
public:
    union {
        struct {T x, y, z;};
        struct {T r, g, b;};
        struct {T item[3];};
    };
    
//    Vec3(const var & v)
//    {
//        x = v.getProperty("x", 0);
//        y = v.getProperty("y", 0);
//        z = v.getProperty("z", 0);
//    }
    
    Vec3(T xIn, T yIn, T zIn): x(xIn), y(yIn), z(zIn) {}
    Vec3(): Vec3(0, 0, 0) {}
    
    virtual ~Vec3() {}
    
    T       sqrLength   () const    {return x * x + y * y + z * z;}
    T       length      () const    {return sqrt(sqrLength());}
    void    normalize   ()          {T l = 1.0 / length(); x *= l; y *= l; z *= l;}
    Vec3    normalized  () const    {Vec3 v = *this; v.normalize(); return v;}
    T       dot         (const Vec3& vIn) const {return x * vIn.x + y * vIn.y + z * vIn.z;}
    Vec3    cross       (const Vec3& vIn) const {return Vec3(y * vIn.z - vIn.y * z, z * vIn.x - vIn.z * x, x * vIn.y - vIn.x * y);}
    
    void operator += (const Vec3& vIn)  {x += vIn.x;  y += vIn.y;  z += vIn.z;}
    void operator += (Vec3* const vIn)  {x += vIn->x; y += vIn->y; z += vIn->z;}
    
    void operator -= (const Vec3& vIn)  {x -= vIn.x;  y -= vIn.y;  z -= vIn.z;}
    void operator -= (Vec3* const vIn)  {x -= vIn->x; y -= vIn->y; z -= vIn->z;}
    
    void operator *= (const T f)        {x *= f; y *= f; z *= f;}
    void operator *= (const Vec3& vIn)  {x *= vIn.x;  y *= vIn.y;  z *= vIn.z;}
    void operator *= (Vec3* const vIn)  {x *= vIn->x; y *= vIn->y; z *= vIn->z;}
    
    void operator /= (const T f)        {x /= f; y /= f; z /= f;}
    void operator /= (const Vec3& vIn)  {x /= vIn.x;  y /= vIn.y;  z /= vIn.z;}
    void operator /= (Vec3* const vIn)  {x /= vIn->x; y /= vIn->y; z /= vIn->z;}
    
    Vec3 operator - () const            {return Vec3(-x, -y, -z);}
    
    friend Vec3 operator + (const Vec3& v1, const Vec3& v2) {
        return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }
    friend Vec3 operator - (const Vec3& v1, const Vec3& v2) {
        return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }
    friend Vec3 operator + (const Vec3& v1, Vec3* const v2) {
        return Vec3(v1.x + v2->x, v1.y + v2->y, v1.z + v2->z);
    }
    friend Vec3 operator - (const Vec3& v1, Vec3* const v2) {
        return Vec3(v1.x - v2->x, v1.y - v2->y, v1.z - v2->z);
    }
    friend Vec3 operator * (const Vec3& vIn, T f) {
        return Vec3(vIn.x * f, vIn.y * f, vIn.z * f);
    }
    friend Vec3 operator * (T f, const Vec3& vIn) {
        return Vec3(vIn.x * f, vIn.y * f, vIn.z * f);
    }
    friend Vec3 operator * (const Vec3& v1, const Vec3& v2) {
        return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
    }
    friend Vec3 operator / (const Vec3& vIn, const T f) {
        return Vec3(vIn.x / f, vIn.y / f, vIn.z / f);
    }
    friend Vec3 operator / (const T f, const Vec3& vIn) {
        return Vec3(vIn.x / f, vIn.y / f, vIn.z / f);
    }
    friend Vec3 operator / (const Vec3& v1, const Vec3& v2) {
        return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
    }
    
    static T        dot     (const Vec3& a, const Vec3& b) {return a.dot(b);}
    static Vec3     cross   (const Vec3& a, const Vec3& b) {return a.cross(b);}
    static Vec3     reflect (const Vec3& n, const Vec3& d) {return d - (2 * n * dot(n, d));}
    
//    var getVar() const
//    {
//        DynamicObject * obj = new DynamicObject();
//        obj->setProperty ("x", x);
//        obj->setProperty ("y", y);
//        obj->setProperty ("z", z);
//        return var (obj);
//    }
};

typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
    
}

#endif