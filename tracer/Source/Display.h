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
    void disable (OpenGLContext& openGLContext);
    
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

struct Shape
{
    Shape()
    {
        
    }
    
    void loadObjFile (WavefrontObjFile & objfile, OpenGLContext & openGLContext)
    {
        for (int i = 0; i != objfile.shapes.size(); ++i)
        {
            
        }
        
        for (int i = 0; i != objfile.shapes.size(); ++i)
            vertexBuffers.add (new VertexBuffer (openGLContext, *objfile.shapes.getUnchecked (i)));
    }
    
    void draw (OpenGLContext& openGLContext, Attributes& attributes)
    {
        for (int i = 0; i < vertexBuffers.size(); ++i)
        {
            VertexBuffer& vertexBuffer = *vertexBuffers.getUnchecked (i);
            vertexBuffer.bind();
            
            attributes.enable (openGLContext);
            glDrawElements (GL_TRIANGLES, vertexBuffer.numIndices, GL_UNSIGNED_INT, 0);
            attributes.disable (openGLContext);
        }
    }
    
private:
    struct VertexBuffer
    {
        VertexBuffer (OpenGLContext& context, WavefrontObjFile::Shape& shape) : openGLContext (context)
        {
            numIndices = shape.mesh.indices.size();
            
            openGLContext.extensions.glGenBuffers (1, &vertexBuffer);
            openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
            
            Array<Vertex> vertices;
            createVertexListFromMesh (shape.mesh, vertices, Colours::green);
            
            openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, vertices.size() * sizeof (Vertex),
                                                   vertices.getRawDataPointer(), GL_STATIC_DRAW);
            
            openGLContext.extensions.glGenBuffers (1, &indexBuffer);
            openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
            openGLContext.extensions.glBufferData (GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof (juce::uint32),
                                                   shape.mesh.indices.getRawDataPointer(), GL_STATIC_DRAW);
        }
        
        ~VertexBuffer()
        {
            openGLContext.extensions.glDeleteBuffers (1, &vertexBuffer);
            openGLContext.extensions.glDeleteBuffers (1, &indexBuffer);
        }
        
        void bind()
        {
            openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
            openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        }
        
        GLuint vertexBuffer, indexBuffer;
        int numIndices;
        OpenGLContext& openGLContext;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VertexBuffer)
    };
    
    OwnedArray<VertexBuffer> vertexBuffers;
    
    static void createVertexListFromMesh (const WavefrontObjFile::Mesh& mesh, Array<Vertex>& list, Colour colour)
    {
        const float scale = 0.2f;
        WavefrontObjFile::Vertex defaultNormal = { 0.5f, 0.5f, 0.5f };
        
        for (int i = 0; i < mesh.vertices.size(); ++i)
        {
            const WavefrontObjFile::Vertex& v = mesh.vertices.getReference (i);
            
            const WavefrontObjFile::Vertex& n
            = i < mesh.normals.size() ? mesh.normals.getReference (i) : defaultNormal;
            
            Vertex vert =
            {
                { scale * v.x, scale * v.y, scale * v.z, },
                { scale * n.x, scale * n.y, scale * n.z, },
                { colour.getFloatRed(), colour.getFloatGreen(), colour.getFloatBlue(), colour.getFloatAlpha() }
            };
            
            list.add (vert);
        }
    }
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
    
    void loadObjFile (const File & file);
    
    void mouseDown (const MouseEvent& e);
    void mouseDrag (const MouseEvent& e);
    void mouseWheelMove (const MouseEvent& e, const MouseWheelDetails& wheel);

private:
    float zoom;
    
    Draggable3DOrientation draggableOrientation;
    OpenGLContext openGLContext;
    
    ScopedPointer<OpenGLShaderProgram> shader;
    ScopedPointer<Shape> shape;
    ScopedPointer<Attributes> attributes;
    ScopedPointer<Uniforms> uniforms;
    
    Matrix3D<float> getProjectionMatrix() const;
    Matrix3D<float> getViewMatrix() const;
    
    WavefrontObjFile objFile;
    
    ReadWriteLock rwl;
};

#endif /* defined(__rayverb__Display__) */