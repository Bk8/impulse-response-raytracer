//
//  Display.cpp
//  rayverb
//
//  Created by Reuben Thomas on 19/01/2014.
//
//

#include "Display.h"

Attributes::Attributes
(   OpenGLContext& openGLContext
,   OpenGLShaderProgram& shader
)
:   position        (createAttribute (openGLContext, shader, "position"))
,   normal          (createAttribute (openGLContext, shader, "normal"))
,   sourceColour    (createAttribute (openGLContext, shader, "sourceColour"))
{
    
}
    
void Attributes::enable (OpenGLContext& openGLContext)
{
    if (position != nullptr)
    {
        openGLContext.extensions.glVertexAttribPointer (position->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), 0);
        openGLContext.extensions.glEnableVertexAttribArray (position->attributeID);
    }
    
    if (normal != nullptr)
    {
        openGLContext.extensions.glVertexAttribPointer (normal->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 3));
        openGLContext.extensions.glEnableVertexAttribArray (normal->attributeID);
    }
    
    if (sourceColour != nullptr)
    {
        openGLContext.extensions.glVertexAttribPointer (sourceColour->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 6));
        openGLContext.extensions.glEnableVertexAttribArray (sourceColour->attributeID);
    }
}

OpenGLShaderProgram::Attribute* Attributes::createAttribute
(   OpenGLContext& openGLContext
,   OpenGLShaderProgram& shader
,   const char* attributeName
)
{
    if (openGLContext.extensions.glGetAttribLocation (shader.programID, attributeName) < 0)
        return nullptr;
    
    return new OpenGLShaderProgram::Attribute (shader, attributeName);
}

Uniforms::Uniforms (OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
:   projectionMatrix    (createUniform (openGLContext, shader, "projectionMatrix"))
,   viewMatrix          (createUniform (openGLContext, shader, "viewMatrix"))
,   lightPosition       (createUniform (openGLContext, shader, "lightPosition"))
{
    
}
    
OpenGLShaderProgram::Uniform* Uniforms::createUniform
(   OpenGLContext& openGLContext
,   OpenGLShaderProgram& shader
,   const char* uniformName
)
{
    if (openGLContext.extensions.glGetUniformLocation (shader.programID, uniformName) < 0)
        return nullptr;
    
    return new OpenGLShaderProgram::Uniform (shader, uniformName);
}

Display::Display()
{
    setOpaque (true);
    
    openGLContext.setRenderer (this);
    openGLContext.attachTo (*this);
    openGLContext.setContinuousRepainting (true);
}

Display::~Display()
{
    openGLContext.detach();
}

void Display::newOpenGLContextCreated()
{
    loadShader();
}

void Display::openGLContextClosing()
{
    shader = nullptr;
    attributes = nullptr;
    uniforms = nullptr;
}

void Display::renderOpenGL()
{
    OpenGLHelpers::clear (Colours::black);
    
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    const float desktopScale = (float) openGLContext.getRenderingScale();
    glViewport
    (   0
    ,   0
    ,   roundToInt (desktopScale * getWidth())
    ,   roundToInt (desktopScale * getHeight())
    );
    
    shader->use();
    
    if (OpenGLShaderProgram::Uniform* uni = uniforms->projectionMatrix)
    {
        openGLContext.extensions.glUniformMatrix4fv
        (   uni->uniformID
        ,   1
        ,   0
        ,   getProjectionMatrix().mat
        );
    }
    
    if (OpenGLShaderProgram::Uniform* uni = uniforms->viewMatrix)
    {
        openGLContext.extensions.glUniformMatrix4fv
        (   uni->uniformID
        ,   1
        ,   0
        ,   getViewMatrix().mat
        );
    }
    
    if (OpenGLShaderProgram::Uniform* uni = uniforms->lightPosition)
    {
        openGLContext.extensions.glUniform4f
        (   uni->uniformID
        ,   -15.0f
        ,   10.0f
        ,   15.0f
        ,   0.0f
        );
    }
    
    //  draw here
    
    openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
    openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
}

Matrix3D<float> Display::getProjectionMatrix() const
{
    float scale = 1.0f;
    float w = 1.0f / (scale + 0.1f);
    float h = w * getLocalBounds().toFloat().getAspectRatio (false);
    return Matrix3D<float>::fromFrustum (-w, w, -h, h, 4.0f, 30.0f);
}

Matrix3D<float> Display::getViewMatrix() const
{
    Matrix3D<float> viewMatrix (Vector3D<float> (0.0f, 1.0f, -10.0f));
    viewMatrix *= draggableOrientation.getRotationMatrix();
    return viewMatrix;
}

void Display::paint (Graphics&) {}

void Display::resized()
{
    draggableOrientation.setViewport (getLocalBounds());
}

void Display::loadShader()
{
    ScopedPointer<OpenGLShaderProgram> newShader (new OpenGLShaderProgram (openGLContext));
        
    String newVertexShader = File ("./rayverb.app/Contents/Resources/vert.glsl").loadFileAsString();
    String newFragmentShader = File ("./rayverb.app/Contents/Resources/frag.glsl").loadFileAsString();
    
    if
    (   newShader->addVertexShader      (newVertexShader)
    &&  newShader->addFragmentShader    (newFragmentShader)
    &&  newShader->link()
    )
    {
        attributes = nullptr;
        uniforms = nullptr;
        
        shader = newShader;
        shader->use();
        
        attributes = new Attributes (openGLContext, *shader);
        uniforms   = new Uniforms (openGLContext, *shader);
    }
    else
    {
        //  oh noes
    }
}