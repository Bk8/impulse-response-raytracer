//
//  TraceThread.h
//  rayverb
//
//  Created by Reuben Thomas on 08/12/2013.
//
//

#ifndef __rayverb__TraceThread__
#define __rayverb__TraceThread__

#include "JuceHeader.h"

#include "ThreadWithCallbacks.h"

#include "Scene.h"

class TraceThread:
public ThreadWithListener
{
public:
    TraceThread (double & progress,
                 std::vector<Rayverb::RayTrace> & raytrace);
    virtual ~TraceThread();
    
    virtual void run();
    
    virtual void setPrimitives (const std::vector<Primitive *> p);
    virtual void setMic (const Rayverb::Mic & m);
    virtual void setRays (const int r);
    
    virtual void setProgress (double & d);
    
    virtual void setParameters (const std::vector<Primitive *> p,
                                const Rayverb::Mic & m,
                                const int r);
        
private:
    std::vector<Primitive *> primitive;
    Rayverb::Mic mic;
    int rays;
    double & progress;
    
    std::vector<Rayverb::RayTrace> & raytrace;
};

#endif /* defined(__rayverb__TraceThread__) */