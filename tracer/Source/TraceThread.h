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
    TraceThread (double & progress);
    virtual ~TraceThread();
    
    virtual void run();
    
    virtual void setFile (const File & f);
    virtual void setPrimitives (const std::vector<Primitive *> p);
    virtual void setMic (const Rayverb::Mic & m);
    virtual void setRays (const int r);
    virtual void setVolumeThreshold (const double d);
    
    virtual void setProgress (double & d);
    
    virtual void setParameters (const File & f,
                                const std::vector<Primitive *> p,
                                const Rayverb::Mic & m,
                                const int r,
                                const double d);
        
private:
    std::vector<Primitive *> primitive;
    Rayverb::Mic mic;
    File file;
    int rays;
    double & progress;
    double volumeThreshold;
    
    std::vector<Rayverb::RayTrace> raytrace;
};

#endif /* defined(__rayverb__TraceThread__) */