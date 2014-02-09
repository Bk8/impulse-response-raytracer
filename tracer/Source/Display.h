//
//  Display.h
//  rayverb
//
//  Created by Reuben Thomas on 19/01/2014.
//
//

#ifndef __rayverb__Display__
#define __rayverb__Display__

#include "JuceHeader.h"
#include "WavefrontObjParser.h"
#include "Primitive.h"

class Display:
public Component,
private OpenGLRenderer
{
public:
    Display();
    virtual ~Display();
    
    void newOpenGLContextCreated();
    void openGLContextClosing();
    void renderOpenGL();
    
    void paint (Graphics & g);
    void resized();
    
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);
//    void mouseMove (const MouseEvent& e);
    void mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel);
    
    void setMicPosition (const Vector3D<double> mp);
    void setSourcePosition (const Vector3D<double> sp);
    
    void setScene (const vector<Triangle> & s);
    
    bool isWireframe() const;
    void setWireframe (bool b);
    
//    Ray generateRay (const double xpos, const double ypos);
    
    void drawOrb (const Vec3f & position, const double size, const double r, const double g, const double b);
    void drawCircle (const Vec3f & position, const Vec3f & normal, const double size);
    
private:
    Draggable3DOrientation draggableOrientation;
    OpenGLContext openGLContext;
    
    Matrix3D<float> getProjectionMatrix() const;
    Matrix3D<float> getViewMatrix() const;
    
    Vector3D<double> micPosition;
    ReadWriteLock micLock;
    
    Vector3D<double> sourcePosition;
    ReadWriteLock sourceLock;
    
    float radius = 1.0f;
    float zoom;
    
    std::vector<Triangle> scene;
    ReadWriteLock sceneLock;
    
    bool wireframe;
};

#endif /* defined(__rayverb__Display__) */