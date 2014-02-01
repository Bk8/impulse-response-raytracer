//
//  Display.h
//  flattener
//
//  Created by Reuben Thomas on 26/01/2014.
//
//

#ifndef __flattener__Display__
#define __flattener__Display__

#include "JuceHeader.h"
#include "Speaker.h"

class SpeakerDisplay
:   public Component
,   public OpenGLRenderer
{
public:
    SpeakerDisplay();
    virtual ~SpeakerDisplay();
    
    void newOpenGLContextCreated();
    void openGLContextClosing();
    void renderOpenGL();
    
    void paint (Graphics & g);
    void resized();
    
    void mouseDown (const MouseEvent & e);
    void mouseDrag (const MouseEvent & e);
    
    void setSpeaker (const vector<Speaker> & s);
    void setSelectedIndex (const int s);
        
private:
    Draggable3DOrientation draggableOrientation;
    OpenGLContext openGLContext;
    
    Matrix3D<float> getProjectionMatrix() const;
    Matrix3D<float> getViewMatrix() const;
    
    const float radius = 1.5f;
    
    std::vector<Speaker> speaker;
    int selectedIndex;
    
    ReadWriteLock speakerLock;
    
    void drawSpeaker (const Speaker & s, const bool selected) const;
};

#endif /* defined(__flattener__Display__) */
