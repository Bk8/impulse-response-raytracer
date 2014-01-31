//
//  Ray.cpp
//  ultraray
//
//  Created by Reuben Thomas on 06/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#include "Ray.h"

Ray Ray::reflect(Vec normal, Vec intersection) const {
    return Ray(intersection, Vec::reflect(normal, direction));
}

typedef pair<Real, const Primitive *> DistPrimPair;

Maybe<const Primitive *> Ray::closest(const vector<Primitive *> & primitive) const {
    
    vector<DistPrimPair> intersection;
    
    const Real EPSILON = 0.00001;
    
    for (auto i = primitive.cbegin(); i != primitive.cend(); ++i) {
        const Maybe<Real> DIST = (*i)->intersection(*this);
        if (DIST.isJust() && DIST.fromJust() > EPSILON)
            intersection.push_back(DistPrimPair(DIST.fromJust(), (*i)));
    }
    
    if (intersection.empty())
        return Maybe<const Primitive *>();
    
    DistPrimPair RET = intersection.front();
    
    for (auto i = intersection.begin(); i != intersection.end(); ++i)
        if (i->first < RET.first && i->first > 0)
            RET = *i;
    
    return Maybe<const Primitive *>(RET.second);
}