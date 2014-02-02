//
//  Scene.h
//  ultraray
//
//  Created by Reuben Thomas on 07/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#ifndef __ultraray__Scene__
#define __ultraray__Scene__

#include "Impulse.h"
#include "Reflection.h"
#include "Mic.h"
#include "Speaker.h"

//#include "JuceHeader.h"

#include <vector>

using namespace std;

namespace Rayverb {

typedef pair<Vec, vector<Impulse> > RayTrace;

class Scene {
public:
    static vector<RayTrace> traceMic(const vector<Primitive *> & primitive,
                                     Mic mic,
                                     const int rays,
                                     const Real volumeThreshold,
                                     double & ratioDone);
    
    static vector<vector<Real> > getChannelSamples(const vector<RayTrace> & raytrace,
                                                   const vector<Speaker> & speaker,
                                                   const Real sampleRate,
                                                   const array<Real, BANDS - 1> midpoint);
};
    
}

#endif /* defined(__ultraray__Scene__) */