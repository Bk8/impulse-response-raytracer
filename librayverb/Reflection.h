//
//  Reflection.h
//  ultraray
//
//  Created by Reuben Thomas on 06/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#ifndef __ultraray__Reflection__
#define __ultraray__Reflection__

#include "Material.h"
#include "Numerical.h"

namespace Rayverb {

class Reflection {
public:
    Surface             surface;
    Vec                 position;
    Vec                 normal;
    Real                distance;
    VolumeCollection    volume;
    bool                direct;
    
    Reflection(const Surface &          surface,
               const Vec &              position,
               const Vec &              normal,
               const Real               distance,
               const VolumeCollection & volume,
               const bool               direct);
};
    
}

#endif /* defined(__ultraray__Reflection__) */