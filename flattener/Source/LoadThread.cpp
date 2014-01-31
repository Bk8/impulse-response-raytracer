//
//  LoadThread.cpp
//  rayverb
//
//  Created by Reuben Thomas on 18/01/2014.
//
//

#include "LoadThread.h"

JSONLoadThread::JSONLoadThread (const File & f)
:   ThreadWithListener ("json load")
,   file (f)
{
    
}

JSONLoadThread::JSONLoadThread()
:   JSONLoadThread (File())
{
    
}

JSONLoadThread::~JSONLoadThread()
{
    
}

void JSONLoadThread::run()
{
    thisVar = JSON::parse (file);
}

void JSONLoadThread::setFile (const File & f)
{
    file = f;
}

var JSONLoadThread::getVar() const
{
    return thisVar;
}

std::vector<RayTrace> JSONLoadThread::getRaytrace() const
{
    return fromVar <std::vector <RayTrace> > () (getVar());
}