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
#include "Display.h"
#include "VectorEditorComponent.h"

class Tracer:
public Component,
public Button::Listener,
public ThreadWithListener::Listener,
public VectorEditor<double>::Listener
{
public:
    Tracer ();
    virtual ~Tracer();
    
    void loadObjFile (const File & f);
    void doTrace (const File & f);
    
    void buttonClicked (Button *);
            
    void resized();
    void paint (Graphics & g);
    
    void clearPrimitives();
    
    bool canTrace() const;
    bool isTracing() const;
    
    var getTraceVar() const;
    
    void writeTrace (const File & f);
    
    bool isWireframe() const;
    void setWireframe (bool b);
    
    void vectorEditorValueChanged (VectorEditor<double> * const ve);
    
private:
    void threadStopped (ThreadWithListener * const);

    WavefrontObjFile objFile;
    
    std::vector<Primitive *> primitive;
    
    ScopedPointer<TextButton> traceButton;
    ScopedPointer<ProgressBar> progressBar;
    double progress;
    
    std::vector<Rayverb::RayTrace> raytrace;
    
    ScopedPointer<TraceThread> traceThread;
    
    ScopedPointer<Display> display;
    
    ScopedPointer<GroupComponent> micPositionGroup;
    ScopedPointer<GroupComponent> sourcePositionGroup;
    
    ScopedPointer<VectorEditor<double> > micPositionEditor;
    ScopedPointer<VectorEditor<double> > sourcePositionEditor;
    
    ScopedPointer<GroupComponent> raytraceSettingsGroup;
    
    ScopedPointer<ValueEditor<int> > rayNumberEditor;
    ScopedPointer<ValueEditor<double> > volumeThresholdEditor;
    
    ScopedPointer<Label> rayNumberLabel;
    ScopedPointer<Label> volumeThresholdLabel;
    
//    ScopedPointer<GroupComponent> materialSettingsGroup;
};
#endif /* defined(__rayverb__Renderer__) */
