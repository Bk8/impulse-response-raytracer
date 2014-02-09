//
//  Display.cpp
//  rayverb
//
//  Created by Reuben Thomas on 19/01/2014.
//
//

#include "Display.h"

Display::Display()
:   wireframe (false)
,   zoom (1.0f)
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
    
    glLineWidth (2);
    
    if (wireframe)
    {
        glColor3f (1, 1, 1);
        glDisable (GL_LIGHTING);
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glEnable (GL_LIGHTING);
        glEnable (GL_LIGHT0);
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        float spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    }
    
    glBegin (GL_TRIANGLES);
    for (auto i = scene.begin(); i != scene.end(); ++i)
    {
        if (! wireframe)
        {
            float diffuse[] =
            {
                static_cast<float>(i->material[0].diffuse),
                static_cast<float>(i->material[1].diffuse),
                static_cast<float>(i->material[2].diffuse),
                1.0
            };
            glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse);

            float specular[] =
            {
                static_cast<float>(i->material[0].specular),
                static_cast<float>(i->material[1].specular),
                static_cast<float>(i->material[2].specular),
                1.0
            };
            glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, specular);
            
//            glColorMaterial (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//            glColor3f (static_cast<float>(i->material[0].diffuse),
//                       static_cast<float>(i->material[1].diffuse),
//                       static_cast<float>(i->material[2].diffuse));

//            glColorMaterial (GL_FRONT_AND_BACK, GL_SPECULAR);
//            glColor3f (static_cast<float>(i->material[0].specular),
//                       static_cast<float>(i->material[1].specular),
//                       static_cast<float>(i->material[2].specular));

            glMateriali (GL_FRONT_AND_BACK, GL_SHININESS, 100);
        }
        
        Vec normal = i->getNormal();
        glNormal3d (normal.x, normal.y, normal.z);
        
        glVertex3d (i->v0.x, i->v0.y, i->v0.z);
        glVertex3d (i->v1.x, i->v1.y, i->v1.z);
        glVertex3d (i->v2.x, i->v2.y, i->v2.z);
    }
    glEnd();
    
    if (isEnabled())
    {
        const double s = radius * 0.1;
        
        micLock.enterRead();
        drawOrb (Vec3f (micPosition.x,
                        micPosition.y,
                        micPosition.z), s, 0, 1, 1);
        micLock.exitRead();
        
        sourceLock.enterRead();
        drawOrb (Vec3f (sourcePosition.x,
                        sourcePosition.y,
                        sourcePosition.z), s, 1, 0, 0);
        sourceLock.exitRead();
    }
}

void Display::drawOrb (const Vec3f & position, const double size, const double r, const double g, const double b)
{
    glColor3d (r, g, b);
    
    glDisable (GL_LIGHTING);
    glEnable (GL_DEPTH_TEST);

    drawCircle (position, Vec3f (1, 0, 0), size * 0.9);
    drawCircle (position, Vec3f (0, 1, 0), size * 0.9);
    drawCircle (position, Vec3f (0, 0, 1), size * 0.9);
    
    glDisable(GL_DEPTH_TEST);
    
    Matrix3D<float> modelview = getViewMatrix();
    Vec3f right (modelview.mat[0], modelview.mat[4], modelview.mat[8]);
    Vec3f up (modelview.mat[1], modelview.mat[5], modelview.mat[9]);
    
    Vec3f c = (up.cross (right));
    
    drawCircle (position, c, size);
    
    glPushMatrix();
    
    glTranslated (position.x, position.y, position.z);
    
    glBegin (GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3f(-size, 0, 0);
    glVertex3f(size, 0, 0);
    
    glColor3d(0, 1, 0);
    glVertex3f(0, -size, 0);
    glVertex3f(0, size, 0);
    
    glColor3d(0, 0, 1);
    glVertex3f(0, 0, -size);
    glVertex3f(0, 0, size);
    glEnd();
    
    glPopMatrix();
}

void Display::drawCircle (const Vec3f & p, const Vec3f & n, const double size)
{
    Vec3f t0 = (n.cross (Vec3f (0.2, 0, 0.5))).normalized();
    Vec3f t1 = (t0.cross (n)).normalized();
    
    const int segments = 50;
    const float angleIncrement = (2 * M_PI) / segments;
    
    glBegin (GL_LINE_LOOP);
    for (int i = 0; i != segments; ++i)
    {
        Vec3f p0 = p + ((t0 * sin (angleIncrement * i)) + (t1 * cos (angleIncrement * i))).normalized() * size;
        
        glVertex3f (p0.x, p0.y, p0.z);
    }
    glEnd();
}


Matrix3D<float> Display::getProjectionMatrix() const
{    
    float w = 0;
    float h = 0;
    float aspect = getLocalBounds().toFloat().getAspectRatio (false);
    
    if (aspect < 1)
    {
        h = 1.0f / (zoom + 0.1f);
        w = h / aspect;
    }
    else
    {
        w = 1.0f / (zoom + 0.1f);
        h = w * aspect;
    }
    
    Matrix3D<float> ret = Matrix3D<float>::fromFrustum (-w, w, -h, h, radius * 0.1, -radius * 2);
    return ret;
}

Matrix3D<float> Display::getViewMatrix() const
{
    Matrix3D<float> viewMatrix (Vector3D<float> (0.0f, 0.0f, -radius - 1));
    viewMatrix *= draggableOrientation.getRotationMatrix();
    viewMatrix *= Matrix3D<float>().rotated (Vector3D<float> (M_PI, 0, 0));
    return viewMatrix;
}

void Display::paint (Graphics&) {}

void Display::resized()
{
    draggableOrientation.setViewport (getLocalBounds());
}

void Display::mouseDown (const MouseEvent& e)
{
    if (isEnabled())
        draggableOrientation.mouseDown (e.getPosition());
}

void Display::mouseDrag (const MouseEvent& e)
{
    if (isEnabled())
        draggableOrientation.mouseDrag (e.getPosition());
}

double determinant (const Matrix3D<float> & m) {
    double value =
    m.mat[3] *m.mat[6] *m.mat[9] *m.mat[12] - m.mat[2] *m.mat[7] *m.mat[9] *m.mat[12] - m.mat[3] *m.mat[5] *m.mat[10]*m.mat[12] + m.mat[1] *m.mat[7] *m.mat[10]*m.mat[12]+
    m.mat[2] *m.mat[5] *m.mat[11]*m.mat[12] - m.mat[1] *m.mat[6] *m.mat[11]*m.mat[12] - m.mat[3] *m.mat[6] *m.mat[8] *m.mat[13] + m.mat[2] *m.mat[7] *m.mat[8] *m.mat[13]+
    m.mat[3] *m.mat[4] *m.mat[10]*m.mat[13] - m.mat[0] *m.mat[7] *m.mat[10]*m.mat[13] - m.mat[2] *m.mat[4] *m.mat[11]*m.mat[13] + m.mat[0] *m.mat[6] *m.mat[11]*m.mat[13]+
    m.mat[3] *m.mat[5] *m.mat[8] *m.mat[14] - m.mat[1] *m.mat[7] *m.mat[8] *m.mat[14] - m.mat[3] *m.mat[4] *m.mat[9] *m.mat[14] + m.mat[0] *m.mat[7] *m.mat[9] *m.mat[14]+
    m.mat[1] *m.mat[4] *m.mat[11]*m.mat[14] - m.mat[0] *m.mat[5] *m.mat[11]*m.mat[14] - m.mat[2] *m.mat[5] *m.mat[8] *m.mat[15] + m.mat[1] *m.mat[6] *m.mat[8] *m.mat[15]+
    m.mat[2] *m.mat[4] *m.mat[9] *m.mat[15] - m.mat[0] *m.mat[6] *m.mat[9] *m.mat[15] - m.mat[1] *m.mat[4] *m.mat[10]*m.mat[15] + m.mat[0] *m.mat[5] *m.mat[10]*m.mat[15];
    return value;
}

Matrix3D<float> invert (const Matrix3D<float> & m) {
    Matrix3D<float> ret;
    ret.mat[0]  = m.mat[6] *m.mat[11]*m.mat[13] - m.mat[7] *m.mat[10]*m.mat[13] + m.mat[7] *m.mat[9] *m.mat[14] - m.mat[5] *m.mat[11]*m.mat[14] - m.mat[6] *m.mat[9] *m.mat[15] + m.mat[5] *m.mat[10]*m.mat[15];
    ret.mat[1]  = m.mat[3] *m.mat[10]*m.mat[13] - m.mat[2] *m.mat[11]*m.mat[13] - m.mat[3] *m.mat[9] *m.mat[14] + m.mat[1] *m.mat[11]*m.mat[14] + m.mat[2] *m.mat[9] *m.mat[15] - m.mat[1] *m.mat[10]*m.mat[15];
    ret.mat[2]  = m.mat[2] *m.mat[7] *m.mat[13] - m.mat[3] *m.mat[6] *m.mat[13] + m.mat[3] *m.mat[5] *m.mat[14] - m.mat[1] *m.mat[7] *m.mat[14] - m.mat[2] *m.mat[5] *m.mat[15] + m.mat[1] *m.mat[6] *m.mat[15];
    ret.mat[3]  = m.mat[3] *m.mat[6] *m.mat[9]  - m.mat[2] *m.mat[7] *m.mat[9]  - m.mat[3] *m.mat[5] *m.mat[10] + m.mat[1] *m.mat[7] *m.mat[10] + m.mat[2] *m.mat[5] *m.mat[11] - m.mat[1] *m.mat[6] *m.mat[11];
    ret.mat[4]  = m.mat[7] *m.mat[10]*m.mat[12] - m.mat[6] *m.mat[11]*m.mat[12] - m.mat[7] *m.mat[8] *m.mat[14] + m.mat[4] *m.mat[11]*m.mat[14] + m.mat[6] *m.mat[8] *m.mat[15] - m.mat[4] *m.mat[10]*m.mat[15];
    ret.mat[5]  = m.mat[2] *m.mat[11]*m.mat[12] - m.mat[3] *m.mat[10]*m.mat[12] + m.mat[3] *m.mat[8] *m.mat[14] - m.mat[0] *m.mat[11]*m.mat[14] - m.mat[2] *m.mat[8] *m.mat[15] + m.mat[0] *m.mat[10]*m.mat[15];
    ret.mat[6]  = m.mat[3] *m.mat[6] *m.mat[12] - m.mat[2] *m.mat[7] *m.mat[12] - m.mat[3] *m.mat[4] *m.mat[14] + m.mat[0] *m.mat[7] *m.mat[14] + m.mat[2] *m.mat[4] *m.mat[15] - m.mat[0] *m.mat[6] *m.mat[15];
    ret.mat[7]  = m.mat[2] *m.mat[7] *m.mat[8]  - m.mat[3] *m.mat[6] *m.mat[8]  + m.mat[3] *m.mat[4] *m.mat[10] - m.mat[0] *m.mat[7] *m.mat[10] - m.mat[2] *m.mat[4] *m.mat[11] + m.mat[0] *m.mat[6] *m.mat[11];
    ret.mat[8]  = m.mat[5] *m.mat[11]*m.mat[12] - m.mat[7] *m.mat[9] *m.mat[12] + m.mat[7] *m.mat[8] *m.mat[13] - m.mat[4] *m.mat[11]*m.mat[13] - m.mat[5] *m.mat[8] *m.mat[15] + m.mat[4] *m.mat[9] *m.mat[15];
    ret.mat[9]  = m.mat[3] *m.mat[9] *m.mat[12] - m.mat[1] *m.mat[11]*m.mat[12] - m.mat[3] *m.mat[8] *m.mat[13] + m.mat[0] *m.mat[11]*m.mat[13] + m.mat[1] *m.mat[8] *m.mat[15] - m.mat[0] *m.mat[9] *m.mat[15];
    ret.mat[10] = m.mat[1] *m.mat[7] *m.mat[12] - m.mat[3] *m.mat[5] *m.mat[12] + m.mat[3] *m.mat[4] *m.mat[13] - m.mat[0] *m.mat[7] *m.mat[13] - m.mat[1] *m.mat[4] *m.mat[15] + m.mat[0] *m.mat[5] *m.mat[15];
    ret.mat[11] = m.mat[3] *m.mat[5] *m.mat[8]  - m.mat[1] *m.mat[7] *m.mat[8]  - m.mat[3] *m.mat[4] *m.mat[9]  + m.mat[0] *m.mat[7] *m.mat[9]  + m.mat[1] *m.mat[4] *m.mat[11] - m.mat[0] *m.mat[5] *m.mat[11];
    ret.mat[12] = m.mat[6] *m.mat[9] *m.mat[12] - m.mat[5] *m.mat[10]*m.mat[12] - m.mat[6] *m.mat[8] *m.mat[13] + m.mat[4] *m.mat[10]*m.mat[13] + m.mat[5] *m.mat[8] *m.mat[14] - m.mat[4] *m.mat[9] *m.mat[14];
    ret.mat[13] = m.mat[1] *m.mat[10]*m.mat[12] - m.mat[2] *m.mat[9] *m.mat[12] + m.mat[2] *m.mat[8] *m.mat[13] - m.mat[0] *m.mat[10]*m.mat[13] - m.mat[1] *m.mat[8] *m.mat[14] + m.mat[0] *m.mat[9] *m.mat[14];
    ret.mat[14] = m.mat[2] *m.mat[5] *m.mat[12] - m.mat[1] *m.mat[6] *m.mat[12] - m.mat[2] *m.mat[4] *m.mat[13] + m.mat[0] *m.mat[6] *m.mat[13] + m.mat[1] *m.mat[4] *m.mat[14] - m.mat[0] *m.mat[5] *m.mat[14];
    ret.mat[15] = m.mat[1] *m.mat[6] *m.mat[8]  - m.mat[2] *m.mat[5] *m.mat[8]  + m.mat[2] *m.mat[4] *m.mat[9]  - m.mat[0] *m.mat[6] *m.mat[9]  - m.mat[1] *m.mat[4] *m.mat[10] + m.mat[0] *m.mat[5] *m.mat[10];
    
    double scale = 1 / determinant (m);
    
    for (int i = 0; i != 16; ++i)
    {
        ret.mat[i] *= scale;
    }
    
    return ret;
}

Vector3D<float> mult (const Matrix3D<float> & m, const Vector3D<float> & v, const double w)
{
    return Vector3D<float>
    (   m.mat[0]  * v.x + m.mat[1]  * v.y + m.mat[2]  * v.z + m.mat[3]  * w
    ,   m.mat[4]  * v.x + m.mat[5]  * v.y + m.mat[6]  * v.z + m.mat[7]  * w
    ,   m.mat[8]  * v.x + m.mat[9]  * v.y + m.mat[10] * v.z + m.mat[11] * w
    );
}

//void Display::mouseMove (const MouseEvent& e)
//{
//    generateRay (e.x, e.y);
//}

void Display::mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel)
{
    const float zoomMin = 0.001f;
    const float zoomMax = 100.0f;
    
    zoom += wheel.deltaY * 0.1;
    
    zoom = std::min (zoomMax, std::max (zoomMin, zoom));
}

//Ray Display::generateRay (const double xpos, const double ypos)
//{
//    double x = ((xpos * 2.0) / getWidth()) - 1;
//    double y = 1 - ((ypos * 2.0) / getHeight());
//    
//    Vector3D<float> rayClip (x, y, -1);
//    
//    Vector3D<float> rayEye = mult (invert (getProjectionMatrix()), rayClip, 1);
//    rayEye.z = -1;
//    
//    Vector3D<float> rayWor = mult (invert (getViewMatrix()), rayEye, 0);
//    
//    rayWor = rayWor.normalised();
//}

void Display::setMicPosition (const Vector3D<double> mp)
{
    micLock.enterWrite();
    micPosition = mp;
    micLock.exitWrite();
}

void Display::setSourcePosition (const Vector3D<double> sp)
{
    sourceLock.enterWrite();
    sourcePosition = sp;
    sourceLock.exitWrite();
}

void Display::setScene (const vector<Triangle> & s)
{
    sceneLock.enterWrite();
    scene = s;
    
    double mag = 0;
    
    for (auto i = scene.begin(); i != scene.end(); ++i)
    {
        double dist0 = i->v0.length();
        double dist1 = i->v1.length();
        double dist2 = i->v2.length();
        
        mag = std::max (mag, std::max (dist0, std::max (dist1, dist2)));
    }
    
    radius = mag;
    
    sceneLock.exitWrite();
}

void Display::setWireframe(bool b)
{
    wireframe = b;
}

bool Display::isWireframe() const
{
    return wireframe;
}
