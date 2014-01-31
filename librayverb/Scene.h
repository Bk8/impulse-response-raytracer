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
                                     double & ratioDone);
    
    static vector<vector<Real> > getChannelSamples(const vector<RayTrace> & raytrace,
                                                   const vector<Speaker> & speaker,
                                                   const array<Real, BANDS - 1> midpoint);
    
    virtual ~Scene();
    
    void addPrimitive(Primitive * p);
    void clearPrimitives();
    
    void setMic(const Mic & m);
    
    vector<RayTrace> trace(const int rays, double & ratioDone) const;
    
private:
    vector<Primitive *> primitive;
    Mic mic;
};
    
}

#endif /* defined(__ultraray__Scene__) */