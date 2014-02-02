//
//  WriteThread.cpp
//  rayverb
//
//  Created by Reuben Thomas on 08/12/2013.
//
//

#include "WriteThread.h"

WriteThread::WriteThread (std::vector<Rayverb::RayTrace> & raytrace):
Thread ("rayverb write"),
bitDepth(16),
sampleRate(44100),
raytrace (raytrace)
{
    
}

WriteThread::~WriteThread()
{
    
}

void WriteThread::run()
{
//    Rayverb::Scene::writeRayTrace (raytrace, speaker, path);
    
    vector<vector<double> > channelSamples = Rayverb::Scene::getChannelSamples (raytrace, speaker, sampleRate, midpoint);
    
    const unsigned int channels = channelSamples.size();
    const unsigned int samples = channelSamples.front().size();
    
    vector<vector<float> > floatSamples;
    for (auto i = channelSamples.begin(); i != channelSamples.end(); ++i)
        floatSamples.push_back (vector<float> (i->begin(), i->end()));
    
    float ** floatPointerSamples = new float*[channels];
    
    for (int i = 0; i != channels; ++i)
        floatPointerSamples[i] = floatSamples[i].data();
    
    std::remove (file.getFullPathName().toRawUTF8());
    
    AiffAudioFormat aiffAudioFormat;
    FileOutputStream * fileStream = file.createOutputStream();
    ScopedPointer<AudioFormatWriter> writer = aiffAudioFormat.createWriterFor
    (   fileStream
    ,   sampleRate
    ,   channels
    ,   bitDepth
    ,   StringPairArray()
    ,   0
    );
    
    if (writer != nullptr)
        writer->writeFromFloatArrays (floatPointerSamples, channels, samples);
    
    delete[] floatPointerSamples;
}

void WriteThread::setSpeakers (const std::vector<Rayverb::Speaker> s)
{
    speaker = s;
}

void WriteThread::setPath (const File & f)
{
    file = f;
}

void WriteThread::setBitDepth (const int b)
{
    bitDepth = b;
}

void WriteThread::setSampleRate (const int s)
{
    sampleRate = s;
}

void WriteThread::setMidpoint(const array<Real, BANDS - 1> &m)
{
    midpoint = m;
}

void WriteThread::setParameters (const std::vector<Rayverb::Speaker> s,
                                 const File & f,
                                 const int bitDepth,
                                 const int sampleRate,
                                 const array<Real, BANDS - 1> m)
{
    setSpeakers (s);
    setPath (f);
    setBitDepth (bitDepth);
    setSampleRate (sampleRate);
    setMidpoint(m);
}