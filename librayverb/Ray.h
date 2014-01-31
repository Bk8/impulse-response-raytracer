//
//  Ray.h
//  ultraray
//
//  Created by Reuben Thomas on 06/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#ifndef ultraray_Ray_h
#define ultraray_Ray_h

#include "Primitive.h"
#include "Numerical.h"
#include "Maybe.h"

#include <vector>

using namespace std;

namespace Rayverb {

class Primitive;

class Ray {
public:
    Vec position;
    Vec direction;
    
    Ray(Vec position, Vec direction):
    position(position), direction(direction) {}
    
    Ray reflect(Vec normal, Vec intersection) const;
    
    Maybe<const Primitive *> closest(const vector<Primitive *> & primitive) const;
};
    
}

#endif