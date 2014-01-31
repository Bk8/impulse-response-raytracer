//
//  Primitive.h
//  ultraray
//
//  Created by Reuben Thomas on 06/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#ifndef ultraray_Primitive_h
#define ultraray_Primitive_h

#include "Material.h"
#include "Maybe.h"
#include "Ray.h"
#include "Numerical.h"

namespace Rayverb {

class Ray;

class Primitive {
public:
    Surface         material;
    bool            isSource;
    
    Primitive(const Surface & material, const bool isSource);
    virtual ~Primitive() {}
    
    Ray                     reflect         (const Ray & ray)       const;
    
    virtual Vec             findNormal      (const Vec  & point)    const = 0;
    virtual Maybe<Real>     intersection    (const Ray & ray)       const = 0;
};

class Sphere: public Primitive {
public:
    Vec             origin;
    Real            radius;
    
    Sphere(const Surface & material,
           const bool isSource,
           const Vec & origin,
           const Real radius);
    
    Vec             findNormal      (const Vec & point)     const;
    Maybe<Real>     intersection    (const Ray & ray)       const;
};

class Plane: public Primitive {
public:
    Vec             normal;
    Real            distance;
    
    Plane(const Surface &   material,
          const Vec &       normal,
          const Real        distance);
    
    Vec             findNormal      (const Vec & point)     const;
    Maybe<Real>     intersection    (const Ray & ray)       const;
};
    
class Triangle: public Primitive {
public:
    Vec v0, v1, v2;
    
    Triangle(const Surface & material,
             const bool isSource,
             const Vec & v0,
             const Vec & v1,
             const Vec & v2);
    
    Vec             findNormal      (const Vec & point)     const;
    Maybe<Real>     intersection    (const Ray & ray)       const;
    
private:
    Vec edge1, edge2, normal;
};
    
}

#endif