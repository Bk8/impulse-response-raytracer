//
//  Reflection.cpp
//  ultraray
//
//  Created by Reuben Thomas on 06/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#include "Reflection.h"

Reflection::Reflection(const Surface &          surface,
                       const Vec &              position,
                       const Vec &              normal,
                       const Real               distance,
                       const VolumeCollection & volume,
                       const bool               direct)
:   surface     (surface)
,   position    (position)
,   normal      (normal)
,   distance    (distance)
,   volume      (volume)
,   direct      (direct)
{
    
}