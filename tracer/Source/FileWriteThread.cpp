//
//  FileWriteThread.cpp
//  rayverb
//
//  Created by Reuben Thomas on 12/01/2014.
//
//

#include "FileWriteThread.h"

JSONWriteThread::JSONWriteThread (const File & f, const var & v)
:   Thread ("json write")
,   file (f)
,   thisVar (v)
{
    
}

JSONWriteThread::JSONWriteThread():
JSONWriteThread (File(), var())
{
    
}

JSONWriteThread::~JSONWriteThread()
{
    
}

void JSONWriteThread::run()
{
    FileOutputStream fos (file);
    JSON::writeToStream (fos, thisVar);
}

void JSONWriteThread::setVar (const juce::var &v)
{
    thisVar = v;
}

void JSONWriteThread::setFile (const File & f)
{
    file = f;
}

void JSONWriteThread::setParameters (const juce::File &f, const juce::var &v)
{
    setFile (f);
    setVar (v);
}