//
//  Renderer.h
//  rayverb
//
//  Created by Reuben Thomas on 05/01/2014.
//
//

#ifndef __rayverb__Renderer__
#define __rayverb__Renderer__

#include "JuceHeader.h"
#include "WavefrontObjParser.h"
#include "TraceThread.h"
#include "FileWriteThread.h"
#include "Display.h"

class Tracer:
public Component,
public Button::Listener,
public ThreadWithListener::Listener
{
public:
    Tracer ();
    virtual ~Tracer();
    
    void loadObjFile (const File & f);
    void doTrace();
    
    void buttonClicked (Button *);
            
    void resized();
    void paint (Graphics & g);
    
    void clearPrimitives();
    
    bool canTrace() const;
    bool canWrite() const;
    bool isTracing() const;
    bool isWriting() const;
    
    var getTraceVar() const;
    
    void writeTrace (const File & f);
    
private:
    void threadStopped (ThreadWithListener * const);

    WavefrontObjFile objFile;
    
    std::vector<Primitive *> primitive;
    
    ScopedPointer<TextButton> traceButton;
    ScopedPointer<ProgressBar> progressBar;
    double progress;
    
    std::vector<Rayverb::RayTrace> raytrace;
    
    ScopedPointer<TraceThread> traceThread;
    ScopedPointer<JSONWriteThread> writeThread;
    
    ScopedPointer<Display> display;
};
#endif /* defined(__rayverb__Renderer__) */
