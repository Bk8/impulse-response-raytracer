//
//  Primitive.cpp
//  ultraray
//
//  Created by Reuben Thomas on 06/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#include "Primitive.h"

Primitive::Primitive(const Surface & material, const bool isSource):
material(material), isSource(isSource) {}

Ray Primitive::reflect(const Ray & ray) const {    
    const Maybe<Real> INTER = intersection(ray);
    const Real DIST = INTER.isJust() ? INTER.fromJust() : 0;
    
    const Vec POS = (ray.direction * DIST) + ray.position;
    const Vec NOR = findNormal(POS);
    
    return ray.reflect(NOR, POS);
}

Sphere::Sphere(const Surface & material,
               const bool isSource,
               const Vec & origin,
               const Real radius):
Primitive(material, isSource), origin(origin), radius(radius) {}

Vec Sphere::findNormal (const Vec & point) const {
    return (point - origin).normalized();
}

Maybe<Real> Sphere::intersection (const Ray & ray) const {
    const Vec D = ray.direction;
    const Vec P = ray.position;
    
    const Vec P_SUB_ORIGIN = P - origin;
    
    const Real A = Vec::dot(D, D);
    const Real B = 2 * Vec::dot(D, P_SUB_ORIGIN);
    const Real C = Vec::dot(P_SUB_ORIGIN, P_SUB_ORIGIN) - (radius * radius);
    
    const Real DISC = (B * B) - (4 * A * C);
    
    if (DISC > 0) return Maybe<Real>(-(B + sqrt(DISC)) / (2 * A));
    return Maybe<Real>();
}

Plane::Plane(const Surface & material,
             const Vec & normal,
             const Real distance):
Primitive(material, false), normal(normal.normalized()), distance(distance) {}

Vec Plane::findNormal (const Vec & point) const {
    return normal;
}

Maybe<Real> Plane::intersection (const Ray & ray) const {
    const Real DET = Vec::dot(normal, ray.direction);
    const Real E = (distance - Vec::dot(normal, ray.position)) / DET;
    if (DET && E > 0) return Maybe<Real>(E);
    return Maybe<Real>();
}

Triangle::Triangle(const Surface & material,
                   const bool isSource,
                   const Vec & v0,
                   const Vec & v1,
                   const Vec & v2):
Primitive(material, isSource),
v0(v0),
v1(v1),
v2(v2),
edge1(v1 - v0),
edge2(v2 - v0),
normal(Vec::cross(edge1, edge2).normalized()) {
    
}

Vec Triangle::findNormal(const Vec &point) const {
    return normal;
}

Maybe<Real> Triangle::intersection(const Rayverb::Ray & ray) const {
    Vec tvec, pvec, qvec;
	Real det;
	Real u, v;
	const Real EPSILON = 0.000001;
	
	pvec = ray.direction.cross(edge2);
	det = edge1.dot(pvec);
	
#if 0 // we don't want to backface cull
	if (det < EPSILON)
		return Maybe<Real>();
	tvec = ray.position - v0;
	
	u = tvec.dot(pvec);
	if (u < 0.0 ||  u > det)
		return Maybe<Real>();
	
	qvec = tvec.cross(edge1);
	v = ray.direction.dot(qvec);
	if (v < 0.0 || u + v > det)
		return Maybe<Real>();
    
    return Maybe<Real>(edge2.dot(qvec) / det);
#else
	if(det > -EPSILON && det < EPSILON)
		return Maybe<Real>();
    
	Real inv_det = 1.0 / det;
	tvec = ray.position - v0;
	u = tvec.dot(pvec) * inv_det;
	if (u < 0.0 || u > 1.0)
		return Maybe<Real>();
    
	qvec = tvec.cross(edge1);
    
	v = ray.direction.dot(qvec) * inv_det;
	if (v < 0.0 || u + v > 1.0)
		return Maybe<Real>();
    
//    Real dist = edge2.dot(qvec) * inv_det;
//    if (dist < 0)
//        return Maybe<Real>();
//    
//    return Maybe<Real>(dist);
    
    return Maybe<Real>(edge2.dot(qvec) * inv_det);
#endif
}