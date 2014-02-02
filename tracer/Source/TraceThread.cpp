//
//  TraceThread.cpp
//  rayverb
//
//  Created by Reuben Thomas on 08/12/2013.
//
//

#include "TraceThread.h"
#include "Adapters.h"

TraceThread::TraceThread (double & progress):
ThreadWithListener ("rayverb trace"),
rays (0),
progress (progress),
raytrace (raytrace)
{
    
}

TraceThread::~TraceThread()
{
    
}

void TraceThread::run()
{
    progress = 0;
    raytrace = Rayverb::Scene::traceMic (primitive,
                                         mic,
                                         rays,
                                         volumeThreshold,
                                         progress);
    progress = 2;
    
    FileOutputStream fos (file);
    JSON::writeToStream(fos, getVar (raytrace));
}

void TraceThread::setFile (const File & f)
{
    file = f;
}

void TraceThread::setPrimitives (const std::vector<Primitive *> p)
{
    primitive = p;
}

void TraceThread::setMic (const Rayverb::Mic & m)
{
    mic = m;
}

void TraceThread::setRays (const int r)
{
    rays = r;
}

void TraceThread::setVolumeThreshold(const double d)
{
    volumeThreshold = d;
}

void TraceThread::setProgress (double & d)
{
    progress = d;
}

void TraceThread::setParameters (const File & f,
                                 const std::vector<Primitive *> p,
                                 const Rayverb::Mic & m,
                                 const int r,
                                 const double d)
{
    setFile (f);
    setPrimitives (p);
    setMic (m);
    setRays (r);
    setVolumeThreshold(d);
}