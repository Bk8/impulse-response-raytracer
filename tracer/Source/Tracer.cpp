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
    
    traceThread = new TraceThread (progress);
    traceThread->addListener (this);
        
    addAndMakeVisible (traceButton = new TextButton ("trace"));
    traceButton->addListener (this);
    traceButton->setColour (TextButton::buttonColourId, Colours::lightgrey);
    traceButton->setEnabled (canTrace());
    
    addAndMakeVisible (display = new Display());
    
    addAndMakeVisible (micPositionGroup = new GroupComponent ("mic position group", "mic position"));
    addAndMakeVisible (sourcePositionGroup = new GroupComponent ("source position group", "source position"));
    
    micPositionGroup->addAndMakeVisible (micPositionEditor = new VectorEditor<double>());
    sourcePositionGroup->addAndMakeVisible (sourcePositionEditor = new VectorEditor<double>());
    
    addAndMakeVisible (raytraceSettingsGroup = new GroupComponent ("raytrace settings group", "raytrace settings"));
    
    raytraceSettingsGroup->addAndMakeVisible (rayNumberEditor = new ValueEditor<int>());
    raytraceSettingsGroup->addAndMakeVisible (volumeThresholdEditor = new ValueEditor<double>());
    
    rayNumberEditor->setValue (1000, dontSendNotification);
    volumeThresholdEditor->setValue(0.001, dontSendNotification);
    
    rayNumberEditor->setIncrement (10);
    volumeThresholdEditor->setIncrement(0.1);
    
    rayNumberEditor->setMinimum (1);
    volumeThresholdEditor->enableBounds (0, 1);
    
    raytraceSettingsGroup->addAndMakeVisible (rayNumberLabel = new Label());
    raytraceSettingsGroup->addAndMakeVisible (volumeThresholdLabel = new Label());
    
    rayNumberLabel->setText("rays", dontSendNotification);
    volumeThresholdLabel->setText("min vol", dontSendNotification);
    
    addAndMakeVisible (materialSettingsGroup = new GroupComponent ("material settings group", "material settings"));
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

void Tracer::doTrace (const File & f)
{
    Vector3D<double> mp = micPositionEditor->getValue();
    Mic m (Vec (mp.x, mp.y, mp.z));
    
    const Rayverb::Surface SOURCE_SURFACE =
    {
        Material (1, 1),
        Material (1, 1),
        Material (1, 1)
    };
    
    Vector3D<double> sp = sourcePositionEditor->getValue();
    
    primitive.push_back(new Rayverb::Sphere(SOURCE_SURFACE,
                                            true,
                                            Vec(sp.x, sp.y, sp.z),
                                            0.001));
    
    traceThread->setParameters (f, primitive, m, rayNumberEditor->getValue(), volumeThresholdEditor->getValue());
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
    
    const int bottomControlHeight = 125;
    
    display->setBounds (getLocalBounds().removeFromTop(getHeight() - controlHeight - 2 * spacing - bottomControlHeight));
    
    micPositionGroup->setBounds (spacing,
                                 display->getBottom() + spacing,
                                 170,
                                 bottomControlHeight - 2 * spacing);
    
    sourcePositionGroup->setBounds (micPositionGroup->getRight() + spacing,
                                    display->getBottom() + spacing,
                                    170,
                                    bottomControlHeight - 2 * spacing);
    
    micPositionEditor->setBounds (10,
                                  20,
                                  micPositionGroup->getWidth() - 20,
                                  micPositionGroup->getHeight() - 30);
    sourcePositionEditor->setBounds (10,
                                     20,
                                     sourcePositionGroup->getWidth() - 20,
                                     sourcePositionGroup->getHeight() - 30);
    
    raytraceSettingsGroup->setBounds (sourcePositionGroup->getRight() + spacing,
                                      display->getBottom() + spacing,
                                      220,
                                      bottomControlHeight - 2 * spacing);
    
    rayNumberLabel->setBounds(10, 20, 60, controlHeight);
    volumeThresholdLabel->setBounds(10, rayNumberLabel->getBottom() + spacing, 60, controlHeight);
    
    rayNumberEditor->setBounds (rayNumberLabel->getRight() + spacing, 20, raytraceSettingsGroup->getWidth() - 10 - spacing - rayNumberLabel->getRight(), controlHeight);
    volumeThresholdEditor->setBounds (volumeThresholdLabel->getRight() + spacing, rayNumberEditor->getBottom() + spacing, rayNumberEditor->getWidth(), controlHeight);
    
    materialSettingsGroup->setBounds (raytraceSettingsGroup->getRight() + spacing,
                                      display->getBottom() + spacing,
                                      getWidth() - 2 * spacing - raytraceSettingsGroup->getRight(),
                                      bottomControlHeight - 2 * spacing);
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

bool Tracer::isTracing() const
{
    return traceThread->isThreadRunning();
}

void Tracer::threadStopped (ThreadWithListener * const twl)
{
    if (twl == traceThread)
    {
        primitive.pop_back();
        
        traceButton->setEnabled (canTrace());
        progressBar->setVisible (false);
    }
}

void Tracer::vectorEditorValueChanged (VectorEditor<double> * const ve)
{
    if (ve == micPositionEditor)
    {
        
    }
    else if (ve == sourcePositionEditor)
    {
        
    }
}