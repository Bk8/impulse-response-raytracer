//
//  WriteThread.h
//  rayverb
//
//  Created by Reuben Thomas on 08/12/2013.
//
//

#ifndef __rayverb__WriteThread__
#define __rayverb__WriteThread__

#include "JuceHeader.h"
#include "Scene.h"

class WriteThread:
public Thread
{
public:
    WriteThread (std::vector<Rayverb::RayTrace> & raytrace);
    virtual ~WriteThread();
    
    virtual void run();
    
    virtual void setSpeakers (const std::vector<Rayverb::Speaker> s);
    virtual void setPath (const File & f);
    virtual void setBitDepth (const int);
    virtual void setSampleRate (const int);
    virtual void setMidpoint (const array<Real, BANDS - 1> & m);

    virtual void setParameters (const std::vector<Rayverb::Speaker> s,
                                const File & f,
                                const int bitDepth,
                                const int sampleRate,
                                const array<Real, BANDS - 1> midpoint);
    
    
private:
    std::vector<Rayverb::Speaker> speaker;
    File file;
    int bitDepth;
    int sampleRate;
    std::vector<Rayverb::RayTrace> & raytrace;
    array<Real, BANDS - 1> midpoint;
};

#endif /* defined(__rayverb__WriteThread__) */