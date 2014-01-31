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

class Display
:   public Component
,   public OpenGLRenderer
{
public:
    Display (std::vector<Speaker> & speaker);
    virtual ~Display();
    
    void newOpenGLContextCreated();
    void openGLContextClosing();
    void renderOpenGL();
    
    void paint (Graphics & g);
    void resized();
    
    void mouseDown (const MouseEvent & e);
    void mouseDrag (const MouseEvent & e);
        
private:
    Draggable3DOrientation draggableOrientation;
    OpenGLContext openGLContext;
    
    Matrix3D<float> getProjectionMatrix() const;
    Matrix3D<float> getViewMatrix() const;
    
    const float radius = 1.5f;
    
    std::vector<Speaker> & speaker;
    
    void drawSpeaker (const Speaker & s) const;
};

#endif /* defined(__flattener__Display__) */
