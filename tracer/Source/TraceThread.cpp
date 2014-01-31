//
//  TraceThread.cpp
//  rayverb
//
//  Created by Reuben Thomas on 08/12/2013.
//
//

#include "TraceThread.h"

TraceThread::TraceThread (double & progress,
                          std::vector<Rayverb::RayTrace> & raytrace):
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
    raytrace = Rayverb::Scene::traceMic (primitive,
                                         mic,
                                         rays,
                                         progress);
    progress = 0;
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

void TraceThread::setProgress (double & d)
{
    progress = d;
}

void TraceThread::setParameters (const std::vector<Primitive *> p,
                                 const Rayverb::Mic & m,
                                 const int r)
{
    setPrimitives (p);
    setMic (m);
    setRays (r);
}