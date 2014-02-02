//
//  Renderer.cpp
//  rayverb
//
//  Created by Reuben Thomas on 05/01/2014.
//
//

#include "Tracer.h"
#include "Adapters.h"
#include "MainComponent.h"

Tracer::Tracer()
{
    addChildComponent (progressBar = new ProgressBar (progress));
    progressBar->setPercentageDisplay (false);
    progressBar->setColour (ProgressBar::foregroundColourId, Colours::darkgrey);
    progressBar->setColour (ProgressBar::backgroundColourId, Colours::transparentBlack);
    
    traceThread = new TraceThread (progress, raytrace);
    traceThread->addListener (this);
    
    writeThread = new JSONWriteThread();
    
    addAndMakeVisible (traceButton = new TextButton ("trace"));
    traceButton->addListener (this);
    traceButton->setColour (TextButton::buttonColourId, Colours::lightgrey);
    traceButton->setEnabled (canTrace());
    
    addAndMakeVisible (display = new Display());
    
    const Rayverb::Surface PLANE_SURFACE =
    {
        Material (1, 1),
        Material (1, 1),
        Material (1, 1)
    };
    
    primitive.push_back(new Rayverb::Sphere(PLANE_SURFACE,
                                            true,
                                            Vec(10, 2, -10),
                                            0.001));
}

Tracer::~Tracer()
{
    clearPrimitives();
}

void Tracer::loadObjFile (const juce::File &f)
{
    if (objFile.load (f).wasOk())
    {
        const Rayverb::Surface PLANE_SURFACE =
        {
            Material (0.9, 0.9),
            Material (0.8, 0.8),
            Material (0.7, 0.7)
        };
        
        for (int i = 0; i < objFile.shapes.size(); ++i)
        {
            WavefrontObjFile::Shape * thisShape = objFile.shapes.getUnchecked (i);
            WavefrontObjFile::Mesh thisMesh = thisShape->mesh;
            
            for (int j = 0; j < objFile.shapes.getUnchecked (i)->mesh.indices.size(); j += 3)
            {
                WavefrontObjFile::Index i0 = thisMesh.indices.getUnchecked (j + 0);
                WavefrontObjFile::Index i1 = thisMesh.indices.getUnchecked (j + 1);
                WavefrontObjFile::Index i2 = thisMesh.indices.getUnchecked (j + 2);
                
                WavefrontObjFile::Vertex v0 = thisMesh.vertices.getUnchecked (i0);
                WavefrontObjFile::Vertex v1 = thisMesh.vertices.getUnchecked (i1);
                WavefrontObjFile::Vertex v2 = thisMesh.vertices.getUnchecked (i2);

                primitive.push_back (new Triangle (PLANE_SURFACE, false,
                                                   Vec (v0.x, v0.y, v0.z),
                                                   Vec (v1.x, v1.y, v1.z),
                                                   Vec (v2.x, v2.y, v2.z)));
            }
        }
    }
    
    traceButton->setEnabled (canTrace());
    
    display->loadObjFile (f);
}

void Tracer::doTrace()
{
    Mic m (Vec (10.6, 2.1, -10.6));
    
    traceThread->setParameters (primitive, m, 1000);
    traceThread->startThread();
    
    traceButton->setEnabled (false);
    progressBar->setVisible (true);
}

void Tracer::buttonClicked (juce::Button * button)
{
    BailOutChecker checker (this);
    
    if (! checker.shouldBailOut())
    {
        if (button == traceButton)
        {
            MainContentComponent::getApplicationCommandManager().invokeDirectly (MainContentComponent::trace, false);
        }
    }
}

void Tracer::resized()
{
    const int buttonWidth   = 100;
    const int spacing       = 5;
    const int controlHeight = 25;
    
    const int progressWidth = getWidth() - ((1 * buttonWidth) +
                                            (3 * spacing));
    
    progressBar->setBounds (spacing,
                            getHeight() - controlHeight - spacing,
                            progressWidth,
                            controlHeight);
    
    traceButton->setBounds (getWidth() - (1 * spacing) - (1 * buttonWidth),
                            getHeight() - controlHeight - spacing,
                            buttonWidth,
                            controlHeight);
    
    display->setBounds (getLocalBounds().removeFromTop(getHeight() - controlHeight - 2 * spacing));
}

void Tracer::paint (juce::Graphics &g)
{
    g.fillAll (Colours::lightgrey);
}

void Tracer::clearPrimitives()
{
    for (auto i = primitive.begin(); i != primitive.end(); ++i)
    {
        delete *i;
    }
    primitive.clear();
}

bool Tracer::canTrace() const
{
    return (! isTracing()) && (primitive.size() > 1);
}

bool Tracer::canWrite() const
{
    return !raytrace.empty() && !isTracing() && !isWriting();
}

bool Tracer::isTracing() const
{
    return traceThread->isThreadRunning();
}

bool Tracer::isWriting() const
{
    return writeThread->isThreadRunning();
}

void Tracer::writeTrace (const File & f)
{
    if (! writeThread->isThreadRunning())
    {
        writeThread->setParameters (f, getVar (raytrace));
        writeThread->startThread();
    }
}

void Tracer::threadStopped (ThreadWithListener * const twl)
{
    if (twl == traceThread)
    {
        traceButton->setEnabled (canTrace());
        progressBar->setVisible (false);
    }
}