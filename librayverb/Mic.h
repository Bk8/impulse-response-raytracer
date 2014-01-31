//
//  Mic.h
//  ultraray
//
//  Created by Reuben Thomas on 06/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#ifndef ultraray_Mic_h
#define ultraray_Mic_h

#include "Ray.h"

namespace Rayverb {

class Mic {
public:
    Vec location;
    
    Mic();
    Mic(const Vec & location);
    
    Ray createRay() const;
};

}

#endif