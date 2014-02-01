//
//  Flattener.cpp
//  rayverb
//
//  Created by Reuben Thomas on 18/01/2014.
//
//

#include "Flattener.h"
#include "Adapters.h"
#include "MainComponent.h"

Flattener::Flattener (std::vector<Rayverb::RayTrace> & raytrace)
:   raytrace (raytrace)
{
    progress = 1;
    
    addChildComponent (progressBar = new ProgressBar (progress));
    progressBar->setPercentageDisplay (false);
    progressBar->setColour (ProgressBar::foregroundColourId, Colours::darkgrey);
    progressBar->setColour (ProgressBar::backgroundColourId, Colours::transparentBlack);
    
    writeThread = new WriteThread (raytrace);

    loadThread = new JSONLoadThread();
    loadThread->addListener (this);
    
    addAndMakeVisible (flattenButton = new TextButton ("flatten"));
    flattenButton->addListener (this);
    flattenButton->setColour (TextButton::buttonColourId, Colours::lightgrey);
    flattenButton->setEnabled (canFlatten());
    
    addAndMakeVisible (speakerEditorGroup = new GroupComponent ("speaker editor group", "speaker settings"));
    
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef defaultURL = CFBundleCopyResourceURL (mainBundle, CFSTR("blumlein"), CFSTR("json"), NULL);
    
    
    speakerEditorGroup->addAndMakeVisible
    (   speakerEditor = new SpeakerEditor
        (   File
            (   CFStringGetCStringPtr
                (   CFURLCopyFileSystemPath (defaultURL, kCFURLPOSIXPathStyle)
                ,   kCFStringEncodingMacRoman
                )
            )
        )
    );
//    speakerEditor->setEnabled (false);
    
    addAndMakeVisible (filterEditorGroup = new GroupComponent ("filter editor group", "filter settings"));
    
    filterEditorGroup->addAndMakeVisible (loCrossoverSlider = new Slider());
    filterEditorGroup->addAndMakeVisible (hiCrossoverSlider = new Slider());
    
    loCrossoverSlider->setRange (20, 20000);
    hiCrossoverSlider->setRange (20, 20000);
    
    loCrossoverSlider->setSkewFactorFromMidPoint (640);
    hiCrossoverSlider->setSkewFactorFromMidPoint (640);
    
    loCrossoverSlider->setColour (Slider::thumbColourId, Colours::darkgrey);
    hiCrossoverSlider->setColour (Slider::thumbColourId, Colours::darkgrey);
    
    loCrossoverSlider->setEnabled(false);
    hiCrossoverSlider->setEnabled(false);
    
    loCrossoverSlider->setValue(200);
    hiCrossoverSlider->setValue(2000);
    
    loCrossoverSlider->addListener(this);
    hiCrossoverSlider->addListener(this);
    
    filterEditorGroup->addAndMakeVisible (loCrossoverLabel = new Label());
    filterEditorGroup->addAndMakeVisible (hiCrossoverLabel = new Label());
    loCrossoverLabel->setText ("lo x-over", dontSendNotification);
    hiCrossoverLabel->setText ("hi x-over", dontSendNotification);
    
    addAndMakeVisible (writeEditorGroup = new GroupComponent ("write editor group", "write settings"));
    
    writeEditorGroup->addAndMakeVisible (bitDepthBox = new ComboBox());
    writeEditorGroup->addAndMakeVisible (sampleRateBox = new ComboBox());
    
    bitDepthBox->addItem ("8 bit", 8);
    bitDepthBox->addItem ("16 bit", 16);
    bitDepthBox->addItem ("24 bit", 24);
    
    sampleRateBox->addItem ("11025", 11025);
    sampleRateBox->addItem ("12000", 12000);
    sampleRateBox->addItem ("22050", 22050);
    sampleRateBox->addItem ("24000", 24000);
    sampleRateBox->addItem ("32000", 32000);
    sampleRateBox->addItem ("44100", 44100);
    sampleRateBox->addItem ("48000", 48000);
    sampleRateBox->addItem ("64000", 64000);
    sampleRateBox->addItem ("88200", 88200);
    sampleRateBox->addItem ("96000", 69000);
    sampleRateBox->addItem ("176400", 176400);
    sampleRateBox->addItem ("192000", 192000);
    
    bitDepthBox->setSelectedItemIndex (1);
    sampleRateBox->setSelectedItemIndex (5);
    
    bitDepthBox->setEnabled (false);
    sampleRateBox->setEnabled (false);
    
    bitDepthBox->addListener (this);
    sampleRateBox->addListener (this);
    
    writeEditorGroup->addAndMakeVisible (bitDepthLabel = new Label());
    writeEditorGroup->addAndMakeVisible (sampleRateLabel = new Label());
    
    bitDepthLabel->setText ("resolution", dontSendNotification);
    sampleRateLabel->setText ("sample rate", dontSendNotification);
    
    bitDepth = bitDepthBox->getSelectedId();
    sampleRate = sampleRateBox->getSelectedId();
}

Flattener::~Flattener()
{
    
}

void Flattener::loadTrace (const File & f)
{
    loadThread->setFile (f);
    loadThread->startThread();
    
    flattenButton->setEnabled (false);
    speakerEditor->setEnabled (false);
    loCrossoverSlider->setEnabled (false);
    hiCrossoverSlider->setEnabled (false);
    bitDepthBox->setEnabled (false);
    sampleRateBox->setEnabled (false);
    
    progressBar->setVisible (true);
}

void Flattener::writeAudio (const File & f) const
{
    writeThread->setParameters (speakerEditor->getSpeakers(),
                                f,
                                bitDepth,
                                sampleRate,
                                {loCrossoverSlider->getValue(),
                                    hiCrossoverSlider->getValue()});
    writeThread->startThread();
}

bool Flattener::canFlatten() const
{
    return ! raytrace.empty();
}

bool Flattener::hasLoaded() const
{
    return ! loadThread->isThreadRunning();
}

void Flattener::buttonClicked (Button * button)
{
    if (button == flattenButton)
    {
        MainContentComponent::getApplicationCommandManager().invokeDirectly (MainContentComponent::flatten, false);
    }
}

void Flattener::sliderValueChanged (Slider * s)
{
    if (s == loCrossoverSlider)
    {
        
    }
    else if (s == hiCrossoverSlider)
    {
        
    }
}

void Flattener::comboBoxChanged (ComboBox * c)
{
    if (c == bitDepthBox)
    {
        bitDepth = bitDepthBox->getSelectedId();
    }
    else if (c == sampleRateBox)
    {
        sampleRate = sampleRateBox->getSelectedId();
    }
}

void Flattener::threadStopped (ThreadWithListener * const t)
{
    if (t == loadThread)
    {
        raytrace = loadThread->getRaytrace();
        
        flattenButton->setEnabled (true);
        
        loCrossoverSlider->setEnabled (true);
        hiCrossoverSlider->setEnabled (true);
        
        loCrossoverSlider->setTextBoxIsEditable (true);
        hiCrossoverSlider->setTextBoxIsEditable (true);
        
        bitDepthBox->setEnabled (true);
        sampleRateBox->setEnabled (true);
        
        progressBar->setVisible (false);
        
        if (speakerEditor->empty())
        {
            speakerEditor->addSpeaker();
        }
        
        speakerEditor->setEnabled (true);
    }
}

void Flattener::paint (Graphics & g)
{
    g.fillAll (Colours::lightgrey);
}

void Flattener::resized()
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
    
    flattenButton->setBounds (getWidth() - (1 * spacing) - (1 * buttonWidth),
                              getHeight() - controlHeight - spacing,
                              buttonWidth,
                              controlHeight);
    
    speakerEditorGroup->setBounds (0, 0, getWidth(), getHeight() - (controlHeight * 2 + 30 + spacing) - controlHeight - spacing * 2);
    speakerEditor->setBounds (10, 20, speakerEditorGroup->getWidth() - 20, speakerEditorGroup->getHeight() - 30);
    
    filterEditorGroup->setBounds (0, speakerEditorGroup->getBottom(), getWidth() / 2, controlHeight * 2 + 30 + spacing);
    
    loCrossoverLabel->setBounds (10, 20, 80, controlHeight);
    hiCrossoverLabel->setBounds (loCrossoverLabel->getX(),
                                 loCrossoverLabel->getBottom() + spacing,
                                 loCrossoverLabel->getWidth(),
                                 controlHeight);
    
    loCrossoverSlider->setBounds (loCrossoverLabel->getRight(),
                                  20,
                                  filterEditorGroup->getWidth() - loCrossoverLabel->getWidth() - 20,
                                  controlHeight);
    hiCrossoverSlider->setBounds (loCrossoverSlider->getX(),
                                  loCrossoverSlider->getBottom() + spacing,
                                  loCrossoverSlider->getWidth(),
                                  controlHeight);
    
    writeEditorGroup->setBounds (getWidth() / 2, speakerEditorGroup->getBottom(), getWidth() / 2, controlHeight * 2 + 30 + spacing);
    
    bitDepthLabel->setBounds (10, 20, 80, controlHeight);
    sampleRateLabel->setBounds (bitDepthLabel->getX(),
                                bitDepthLabel->getBottom() + spacing,
                                bitDepthLabel->getWidth(),
                                controlHeight);
    
    bitDepthBox->setBounds (bitDepthLabel->getRight(),
                            20,
                            writeEditorGroup->getWidth() - bitDepthLabel->getWidth() - 20,
                            controlHeight);
    sampleRateBox->setBounds (bitDepthBox->getX(),
                              bitDepthBox->getBottom() + spacing,
                              bitDepthBox->getWidth(),
                              controlHeight);
}

void Flattener::openSettings (const juce::File &f)
{
    speakerEditor->openSettings (f);
}

void Flattener::saveSettings (const juce::File &f) const
{
    speakerEditor->saveSettings (f);
}