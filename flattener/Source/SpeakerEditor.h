//
//  SpeakerEditor.h
//  flattener
//
//  Created by Reuben Thomas on 26/01/2014.
//
//

#ifndef __flattener__SpeakerEditor__
#define __flattener__SpeakerEditor__

#include "JuceHeader.h"
#include "SpeakerListBoxModel.h"
#include "Display.h"
#include "Speaker.h"

class SpeakerEditor
:   public Component
,   public Button::Listener
,   public Slider::Listener
,   public SpeakerListBoxModel::Listener
{
public:
    
    SpeakerEditor();
    SpeakerEditor (const File & f);
    virtual ~SpeakerEditor();
    
    void setEnabled (bool shouldBeEnabled);
    
    void buttonClicked (Button * button);
    void sliderValueChanged (Slider * slider);
    
    void addSpeaker();
    void removeSpeaker (const int index);
    
    void speakerSelected (const int row);
    void deleteKeyPressed (const int lastRowSelected);
    
    void resized();
    
    void openSettings (const File & f);
    void saveSettings (const File & f) const;
    
    bool empty() const;
    std::vector<Speaker> getSpeakers() const;
    
private:
    SpeakerListBoxModel speakerListBoxModel;
    ScopedPointer<ListBox> listBox;
    
    ScopedPointer<TextButton> addSpeakerButton;
    ScopedPointer<TextButton> removeSpeakerButton;
    
    ScopedPointer<Slider> inclinationSlider;
    ScopedPointer<Slider> azimuthSlider;
    ScopedPointer<Slider> shapeSlider;
    
    ScopedPointer<Label> inclinationLabel;
    ScopedPointer<Label> azimuthLabel;
    ScopedPointer<Label> shapeLabel;
    
    ScopedPointer<SpeakerDisplay> display;
    
    template<typename T>
    static T wrap (const T min, const T max, T val)
    {
        const T diff = max - min;
        
        while (val < min) val += diff;
        while (max < val) val -= diff;
        
        return val;
    }
    
    static Real radToDeg (const Real rad);
    static Real degToRad (const Real deg);
    
    std::vector<Speaker> speaker;
};

#endif /* defined(__flattener__SpeakerEditor__) */
