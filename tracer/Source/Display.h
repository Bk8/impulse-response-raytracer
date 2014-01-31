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

struct Vertex
{
    float position[3];
    float normal[3];
    float colour[4];
};

struct Attributes
{
    Attributes (OpenGLContext& openGLContext, OpenGLShaderProgram& shader);
    
    void enable (OpenGLContext& openGLContext);
    
    ScopedPointer<OpenGLShaderProgram::Attribute> position, normal, sourceColour;
private:
    static OpenGLShaderProgram::Attribute* createAttribute
    (   OpenGLContext& openGLContext
    ,   OpenGLShaderProgram& shader
    ,   const char* attributeName
    );
};

struct Uniforms
{
    Uniforms (OpenGLContext& openGLContext, OpenGLShaderProgram& shader);
    
    ScopedPointer<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, lightPosition;
    
private:
    static OpenGLShaderProgram::Uniform* createUniform
    (   OpenGLContext& openGLContext
    ,   OpenGLShaderProgram& shader
    ,   const char* uniformName
    );
};

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
    
    void loadShader();

private:
    Draggable3DOrientation draggableOrientation;
    OpenGLContext openGLContext;
    
    ScopedPointer<OpenGLShaderProgram> shader;
    ScopedPointer<Attributes> attributes;
    ScopedPointer<Uniforms> uniforms;
    
    Matrix3D<float> getProjectionMatrix() const;
    Matrix3D<float> getViewMatrix() const;
};

#endif /* defined(__rayverb__Display__) */