//
//  Display.cpp
//  flattener
//
//  Created by Reuben Thomas on 26/01/2014.
//
//

#include "Display.h"

Display::Display (std::vector<Speaker> & speaker):
speaker (speaker)
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
    
}

void Display::openGLContextClosing()
{
    
}

void Display::renderOpenGL()
{
    OpenGLHelpers::clear (Colours::black);
    
    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_LINE_SMOOTH);
    
    const float desktopScale = (float) openGLContext.getRenderingScale();
    glViewport
    (   0
    ,   0
    ,   roundToInt (desktopScale * getWidth())
    ,   roundToInt (desktopScale * getHeight())
    );
    
//    glPushMatrix();
    
    glMatrixMode (GL_PROJECTION);
    glLoadMatrixf (getProjectionMatrix().mat);
    
    glMatrixMode (GL_MODELVIEW);
    glLoadMatrixf (getViewMatrix().mat);
    
    glColor3f (0, 1, 1);
    
    glLineWidth (2);
    
    const int numSegments = 200;
    
    glBegin (GL_LINE_LOOP);
    for (int i = 0; i != numSegments; ++i)
    {
        float theta = M_PI * 2 * i / numSegments;
        
        glVertex3f (radius * sin (theta),
                    radius * cos (theta),
                    0);
    }
    glEnd();
    
    glBegin (GL_LINE_LOOP);
    for (int i = 0; i != numSegments; ++i)
    {
        float theta = M_PI * 2 * i / numSegments;
        
        glVertex3f (radius * sin (theta),
                    0,
                    radius * cos (theta));
    }
    glEnd();
    
    glBegin (GL_LINE_LOOP);
    for (int i = 0; i != numSegments; ++i)
    {
        float theta = M_PI * 2 * i / numSegments;
        
        glVertex3f (0,
                    radius * sin (theta),
                    radius * cos (theta));
    }
    glEnd();
    
    glColor3f (1, 0, 0);
    glBegin (GL_LINES);
    glVertex3f (0, 0, 0);
    glVertex3f (radius, 0, 0);
    glEnd();
    
    glColor3f (0, 1, 0);
    glBegin (GL_LINES);
    glVertex3f (0, 0, 0);
    glVertex3f (0, -radius, 0);
    glEnd();
    
    glColor3f (0, 0, 1);
    glBegin (GL_LINES);
    glVertex3f (0, 0, 0);
    glVertex3f (0, 0, radius);
    glEnd();
    
//    glPopMatrix();
    
    for (auto i = speaker.begin(); i != speaker.end(); ++i)
    {
        drawSpeaker (*i);
    }
}

void Display::drawSpeaker (const Speaker & s) const
{
    glLineWidth (3);
    
    glPointSize (3);
    
    Vec dir = s.direction.normalized() * radius;
    
    glBegin (GL_LINES);
    
    glColor3f (0, 1, 1);
    glVertex3f (0, 0, 0);
    glVertex3f (dir.x, dir.y, dir.z);
    
    glEnd();
    
    glBegin (GL_POINTS);
    
    const int inclinationPoints = 40;
    const int azimuthPoints = inclinationPoints * 2;
    
    for (int i = 0; i != inclinationPoints; ++i)
    {
        const float inclination = M_PI * i / (inclinationPoints - 1);
        
        for (int j = 0; j != azimuthPoints; ++j)
        {
            const float azimuth = 2 * M_PI * j / azimuthPoints;
            
            const Vec direction = Speaker::polarToCartesian (inclination, azimuth);
            
            const float attenuation = s.attenuation (direction);
            
            Vec pt;
            
            if (attenuation > 0)
            {
                glColor3f (0, 1, 1);
                pt = direction * attenuation * radius;
            }
            else
            {
                glColor3f (1, 0, 0);
                pt = -direction * attenuation * radius;
            }
            
            glVertex3f (pt.x, pt.y, pt.z);
        }
    }
    
    glEnd();
}

void Display::paint (Graphics & g)
{
    
}

void Display::resized()
{
    draggableOrientation.setViewport (getLocalBounds());
}

Matrix3D<float> Display::getProjectionMatrix() const
{
    float scale = 1.0f;
    
    float w = 0;
    float h = 0;
    float aspect = getLocalBounds().toFloat().getAspectRatio (false);
    
    if (aspect < 1)
    {
        h = 1.0f / (scale + 0.1f);
        w = h / aspect;
    }
    else
    {
        w = 1.0f / (scale + 0.1f);
        h = w * aspect;
    }
    
    Matrix3D<float> ret = Matrix3D<float>::fromFrustum (-w, w, -h, h, 1.0f, -radius * 2);
    return ret;
}

Matrix3D<float> Display::getViewMatrix() const
{
    Matrix3D<float> viewMatrix (Vector3D<float> (0.0f, 0.0f, -radius - 1));
    viewMatrix *= draggableOrientation.getRotationMatrix();
    return viewMatrix;
}

void Display::mouseDown (const MouseEvent& e)
{
    draggableOrientation.mouseDown (e.getPosition());
}

void Display::mouseDrag (const MouseEvent& e)
{
    draggableOrientation.mouseDrag (e.getPosition());
}