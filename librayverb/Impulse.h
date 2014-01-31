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
    uint64_t  samplePosition;
    VolumeCollection amplitude;
    
//    Impulse(const var & v);
    Impulse(const uint64_t samplePosition, const VolumeCollection & amplitude);
    Impulse(const Sphere * source, const Reflection & reflection);
    Impulse();
    
//    var getVar() const;
    
private:
//    static var volumeCollectionToVar (const VolumeCollection & vc);
//    static VolumeCollection varToVolumeCollection (const var & vc);
};

const Real      SAMPLE_RATE         = 44100;
const Real      SPEED_OF_SOUND      = 340;
const Real      SAMPLES_PER_UNIT    = SAMPLE_RATE / SPEED_OF_SOUND;

}

#endif /* defined(__ultraray__Impulse__) */