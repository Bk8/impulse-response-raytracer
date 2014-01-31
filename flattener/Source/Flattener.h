//
//  Flattener.h
//  rayverb
//
//  Created by Reuben Thomas on 18/01/2014.
//
//

#ifndef __rayverb__Flattener__
#define __rayverb__Flattener__

#include "JuceHeader.h"
#include "WriteThread.h"
#include "LoadThread.h"
#include "ThreadWithCallbacks.h"
#include "SpeakerEditor.h"

class Flattener
:   public Component
,   public Button::Listener
,   public Slider::Listener
,   public ComboBox::Listener
,   public ThreadWithListener::Listener
{
public:
    Flattener (std::vector<Rayverb::RayTrace> & raytrace);
    virtual ~Flattener();
    
    void loadTrace (const File & f);
    
    void writeAudio (const File & f) const;
    
    bool canFlatten() const;
    bool hasLoaded() const;
    
    void buttonClicked (Button * b);
    void sliderValueChanged (Slider * s);
    void comboBoxChanged (ComboBox * c);
    
    void threadStopped (ThreadWithListener * const);
    
    void paint (Graphics & g);
    void resized();
    
private:
    ScopedPointer<TextButton> flattenButton;
    ScopedPointer<ProgressBar> progressBar;
    double progress;

    std::vector<Rayverb::RayTrace> & raytrace;

    ScopedPointer<JSONLoadThread> loadThread;
    ScopedPointer<WriteThread> writeThread;
    
    ScopedPointer<GroupComponent> speakerEditorGroup;
    ScopedPointer<SpeakerEditor> speakerEditor;
    
    ScopedPointer<GroupComponent> filterEditorGroup;
    ScopedPointer<Slider> loCrossoverSlider;
    ScopedPointer<Slider> hiCrossoverSlider;
    ScopedPointer<Label> loCrossoverLabel;
    ScopedPointer<Label> hiCrossoverLabel;
    
    ScopedPointer<GroupComponent> writeEditorGroup;
    ScopedPointer<ComboBox> bitDepthBox;
    ScopedPointer<ComboBox> sampleRateBox;
    ScopedPointer<Label> bitDepthLabel;
    ScopedPointer<Label> sampleRateLabel;
    
    int bitDepth;
    int sampleRate;
};

#endif /* defined(__rayverb__Flattener__) */