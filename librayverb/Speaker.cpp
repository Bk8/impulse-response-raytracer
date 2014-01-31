//
//  Speaker.cpp
//  ultraray
//
//  Created by Reuben Thomas on 09/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#include "Speaker.h"

Speaker::Speaker(const Real inclination, const Real azimuth, const Real directionality)
:   direction (polarToCartesian (inclination, azimuth))
,   directionality (directionality) {
    
}

Speaker::Speaker(const Vec & direction, const Real directionality):
direction(direction), directionality(directionality) {}

Speaker::Speaker(): Speaker(Vec(1, 0, 0), 0) {}

float getAttenuation(const Vec & SPEAKER_DIRECTION,
                     const Vec & RAY_DIRECTION,
                     const Real DIRECTIONALITY) {
    const float ATTENUATION = ((1.0 - DIRECTIONALITY) +
                               (DIRECTIONALITY *
                                Vec::dot(RAY_DIRECTION      .normalized(),
                                         SPEAKER_DIRECTION  .normalized())));
//    return ATTENUATION < 0 ? 0 : ATTENUATION;
    return ATTENUATION;
}

Real Speaker::attenuation(const Vec & RAY_DIRECTION) const {
    return getAttenuation(direction, RAY_DIRECTION, directionality);
}

Real Speaker::getInclination() const {
    return acos (direction.y);
}

Real Speaker::getAzimuth() const {
    return atan2 (direction.z, direction.x);
}

void Speaker::setDirection (const Real inclination, const Real azimuth) {
    direction = polarToCartesian (inclination, azimuth);
}

Vec Speaker::polarToCartesian (const Real inclination, const Real azimuth)
{
    return Vec (sin (inclination) * cos (azimuth),
                cos (inclination),
                sin (inclination) * sin (azimuth));
}