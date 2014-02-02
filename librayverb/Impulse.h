//
//  Impulse.h
//  ultraray
//
//  Created by Reuben Thomas on 07/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#ifndef __ultraray__Impulse__
#define __ultraray__Impulse__

#include "Numerical.h"
#include "Material.h"
#include "Reflection.h"
#include "Primitive.h"

namespace Rayverb {

class Impulse {
public:
    Real samplePosition;
    VolumeCollection amplitude;
    
    Impulse(const Real samplePosition, const VolumeCollection & amplitude);
    Impulse(const Sphere * source, const Reflection & reflection);
    Impulse();
    
    uint64_t getPositionInSamples (const Real sampleRate) const;
    Real getPositionInSeconds() const;
};

const Real      SPEED_OF_SOUND      = 340;
const Real      SECONDS_PER_METRE   = 1 / SPEED_OF_SOUND;

}

#endif /* defined(__ultraray__Impulse__) */