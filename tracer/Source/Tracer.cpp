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

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

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
    
    display->setEnabled (false);
    
    addAndMakeVisible (micPositionGroup = new GroupComponent ("mic position group", "mic position"));
    addAndMakeVisible (sourcePositionGroup = new GroupComponent ("source position group", "source position"));
    
    micPositionGroup->addAndMakeVisible (micPositionEditor = new VectorEditor<double>());
    sourcePositionGroup->addAndMakeVisible (sourcePositionEditor = new VectorEditor<double>());
    
    micPositionEditor->addListener (this);
    sourcePositionEditor->addListener (this);
    
    micPositionEditor->setEnabled (false);
    sourcePositionEditor->setEnabled (false);
    
    addAndMakeVisible (raytraceSettingsGroup = new GroupComponent ("raytrace settings group", "raytrace settings"));
    
    raytraceSettingsGroup->addAndMakeVisible (rayNumberEditor = new ValueEditor<int>());
    raytraceSettingsGroup->addAndMakeVisible (volumeThresholdEditor = new ValueEditor<double>());
    
    rayNumberEditor->setEnabled (false);
    volumeThresholdEditor->setEnabled (false);
    
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
    
//    addAndMakeVisible (materialSettingsGroup = new GroupComponent ("material settings group", "material settings"));
}

Tracer::~Tracer()
{
    clearPrimitives();
}

void Tracer::loadObjFile (const juce::File &f)
{
    Assimp::Importer importer;
    
    const aiScene* pScene = importer.ReadFile(f.getFullPathName().toStdString().c_str(),
                                              aiProcess_Triangulate |
                                              aiProcess_GenSmoothNormals |
                                              aiProcess_FlipUVs);
    
    if (pScene)
    {
        clearPrimitives();
        
        const Rayverb::Surface PLANE_SURFACE =
        {
            Material (0.9, 0.9),
            Material (0.8, 0.8),
            Material (0.7, 0.7)
        };
        
        vector<Triangle> trivec;
        
        for (unsigned int i = 0; i != pScene->mNumMeshes; ++i)
        {
            const aiMesh * thisMesh = pScene->mMeshes[i];
            
            for (unsigned int j = 0; j != thisMesh->mNumFaces; ++j)
            {
                const aiFace * thisFace = &thisMesh->mFaces[j];
                
                if (thisFace->mNumIndices == 3)
                {
                    const aiVector3D * p0 = &thisMesh->mVertices[thisFace->mIndices[0]];
                    const aiVector3D * p1 = &thisMesh->mVertices[thisFace->mIndices[1]];
                    const aiVector3D * p2 = &thisMesh->mVertices[thisFace->mIndices[2]];
                    
                    trivec.push_back
                    (   Triangle
                        (   PLANE_SURFACE
                        ,   false
                        ,   Vec (p0->x, p0->y, p0->z)
                        ,   Vec (p1->x, p1->y, p1->z)
                        ,   Vec (p2->x, p2->y, p2->z)
                        )
                    );
                }
                else
                {
                    std::cerr << "oh dear - this face isn't a triangle" << std::endl;
                }
            }
        }
        
        traceButton->setEnabled (canTrace());
        
        display->setScene (trivec);
        
        micPositionEditor->setEnabled (true);
        sourcePositionEditor->setEnabled (true);
        rayNumberEditor->setEnabled (true);
        volumeThresholdEditor->setEnabled (true);
        display->setEnabled (true);
    }
    else
    {
        std::cerr << "Error parsing '" << f.getFullPathName().toStdString() <<"': '" << importer.GetErrorString() << "'" << std::endl;
    }
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
    
    const int bottomControlHeight = 115;
    
    display->setBounds (getLocalBounds().removeFromTop(getHeight() - controlHeight - 2 * spacing - bottomControlHeight));
    
    micPositionGroup->setBounds (0,
                                 display->getBottom(),
                                 170,
                                 bottomControlHeight);
    
    sourcePositionGroup->setBounds (micPositionGroup->getRight(),
                                    display->getBottom(),
                                    170,
                                    bottomControlHeight);
    
    micPositionEditor->setBounds (10,
                                  20,
                                  micPositionGroup->getWidth() - 20,
                                  micPositionGroup->getHeight() - 30);
    sourcePositionEditor->setBounds (10,
                                     20,
                                     sourcePositionGroup->getWidth() - 20,
                                     sourcePositionGroup->getHeight() - 30);
    
    raytraceSettingsGroup->setBounds (sourcePositionGroup->getRight(),
                                      display->getBottom(),
                                      220,
                                      bottomControlHeight);
    
    rayNumberLabel->setBounds(10, 20, 60, controlHeight);
    volumeThresholdLabel->setBounds(10, rayNumberLabel->getBottom() + spacing, 60, controlHeight);
    
    rayNumberEditor->setBounds (rayNumberLabel->getRight() + spacing, 20, raytraceSettingsGroup->getWidth() - 10 - spacing - rayNumberLabel->getRight(), controlHeight);
    volumeThresholdEditor->setBounds (volumeThresholdLabel->getRight() + spacing, rayNumberEditor->getBottom() + spacing, rayNumberEditor->getWidth(), controlHeight);
    
//    materialSettingsGroup->setBounds (raytraceSettingsGroup->getRight(),
//                                      display->getBottom(),
//                                      getWidth() - raytraceSettingsGroup->getRight(),
//                                      bottomControlHeight);
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
        delete primitive.back();
        
        primitive.pop_back();
        
        traceButton->setEnabled (canTrace());
        progressBar->setVisible (false);
    }
}

void Tracer::vectorEditorValueChanged (VectorEditor<double> * const ve)
{
    if (ve == micPositionEditor)
    {
        display->setMicPosition (ve->getValue());
    }
    else if (ve == sourcePositionEditor)
    {
        display->setSourcePosition (ve->getValue());
    }
}

bool Tracer::isWireframe() const
{
    return display->isWireframe();
}

void Tracer::setWireframe(bool b)
{
    display->setWireframe (b);
}