//
//  Speaker.h
//  ultraray
//
//  Created by Reuben Thomas on 09/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#ifndef __ultraray__Speaker__
#define __ultraray__Speaker__

#include "Numerical.h"

namespace Rayverb {

class Speaker {
public:
    Vec direction;
    Real directionality;
    
    Speaker();
    Speaker(const Vec & direction, const Real directionality);
    Speaker(const Real inclination, const Real azimuth, const Real directionality);
    
    Real attenuation(const Vec & RAY_DIRECTION) const;
    
    Real getInclination() const;
    Real getAzimuth() const;
    
    void setDirection (const Real inclination, const Real azimuth);
    
    static Vec polarToCartesian (const Real inclination, const Real azimuth);
};

}

#endif /* defined(__ultraray__Speaker__) */