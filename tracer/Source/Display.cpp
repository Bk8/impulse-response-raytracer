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

void Attributes::disable (OpenGLContext& openGLContext)
{
    if (position != nullptr)       openGLContext.extensions.glDisableVertexAttribArray (position->attributeID);
    if (normal != nullptr)         openGLContext.extensions.glDisableVertexAttribArray (normal->attributeID);
    if (sourceColour != nullptr)   openGLContext.extensions.glDisableVertexAttribArray (sourceColour->attributeID);
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

Display::Display():
zoom (1.0)
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
    shape = nullptr;
    shader = nullptr;
    attributes = nullptr;
    uniforms = nullptr;
}

void Display::renderOpenGL()
{
    OpenGLHelpers::clear (Colours::grey);
    
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
        ,   0.5f
        ,   1.0f
        ,   0.5f
        ,   0.1f
        );
    }
    
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    shape->draw (openGLContext, *attributes);
    
    openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
    openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
}

Matrix3D<float> Display::getProjectionMatrix() const
{
    float w = 1.0f / (zoom + 0.1f);
    float h = w * getLocalBounds().toFloat().getAspectRatio (false);
    return Matrix3D<float>::fromFrustum (-w, w, -h, h, 5.0f, 20.0f);
}

Matrix3D<float> Display::getViewMatrix() const
{
    Matrix3D<float> viewMatrix (Vector3D<float> (0.0f, 0.0f, -10.0f));
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
    
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    
    CFURLRef vertURL = CFBundleCopyResourceURL (mainBundle, CFSTR("vert"), CFSTR("glsl"), NULL);
    CFURLRef fragURL = CFBundleCopyResourceURL (mainBundle, CFSTR("frag"), CFSTR("glsl"), NULL);
    
    String newVertexShader = File (CFStringGetCStringPtr (CFURLCopyFileSystemPath (vertURL, kCFURLPOSIXPathStyle),
                                                          kCFStringEncodingMacRoman)).loadFileAsString();
    String newFragmentShader = File (CFStringGetCStringPtr (CFURLCopyFileSystemPath (fragURL, kCFURLPOSIXPathStyle),
                                                            kCFStringEncodingMacRoman)).loadFileAsString();
    
    if
    (   newShader->addVertexShader      (newVertexShader)
    &&  newShader->addFragmentShader    (newFragmentShader)
    &&  newShader->link()
    )
    {
        shape = nullptr;
        attributes = nullptr;
        uniforms = nullptr;
        
        shader = newShader;
        shader->use();
        
        shape = new Shape();
        attributes = new Attributes (openGLContext, *shader);
        uniforms   = new Uniforms (openGLContext, *shader);
    }
    else
    {
        //  oh noes
    }
}

void Display::loadObjFile( const File & file)
{
    objFile.load (file);
    shape->loadObjFile(objFile, openGLContext);
}

void Display::mouseDown (const MouseEvent& e)
{
    draggableOrientation.mouseDown (e.getPosition());
}

void Display::mouseDrag (const MouseEvent& e)
{
    draggableOrientation.mouseDrag (e.getPosition());
}

void Display::mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel)
{
    zoom += wheel.deltaY * 0.1;
}

void Display::setMicPosition (const Vector3D<float> mp)
{
    micLock.enterWrite();
    micPosition = mp;
    micLock.exitWrite();
}

void Display::setSourcePosition (const Vector3D<float> sp)
{
    sourceLock.enterWrite();
    sourcePosition = sp;
    sourceLock.exitWrite();
}

