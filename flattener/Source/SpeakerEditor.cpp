//
//  SpeakerEditor.cpp
//  flattener
//
//  Created by Reuben Thomas on 26/01/2014.
//
//

#include "SpeakerEditor.h"
#include "Adapters.h"

SpeakerEditor::SpeakerEditor()
:   speakerListBoxModel (speaker)
{
    addAndMakeVisible (listBox = new ListBox ("speaker selection", &speakerListBoxModel));
    
    addAndMakeVisible (addSpeakerButton = new TextButton ("+"));
    addAndMakeVisible (removeSpeakerButton = new TextButton ("-"));
    
    addSpeakerButton->addListener (this);
    removeSpeakerButton->addListener (this);
    
    addSpeakerButton->setColour (TextButton::buttonColourId, Colours::lightgrey);
    removeSpeakerButton->setColour (TextButton::buttonColourId, Colours::lightgrey);
    
    speakerListBoxModel.addListener (this);
    
    addAndMakeVisible (inclinationSlider = new Slider());
    addAndMakeVisible (azimuthSlider = new Slider());
    addAndMakeVisible (shapeSlider = new Slider());
    
    inclinationSlider->addListener (this);
    azimuthSlider->addListener (this);
    shapeSlider->addListener (this);
    
    inclinationSlider->setRange (0, 180);
    azimuthSlider->setRange (0, 360);
    shapeSlider->setRange (0, 1);
    
    inclinationSlider->setColour (Slider::thumbColourId, Colours::darkgrey);
    azimuthSlider->setColour (Slider::thumbColourId, Colours::darkgrey);
    shapeSlider->setColour (Slider::thumbColourId, Colours::darkgrey);
    
    addAndMakeVisible (inclinationLabel = new Label());
    addAndMakeVisible (azimuthLabel = new Label());
    addAndMakeVisible (shapeLabel = new Label());
    
    inclinationLabel->setText ("inclination", dontSendNotification);
    azimuthLabel->setText ("azimuth", dontSendNotification);
    shapeLabel->setText ("shape", dontSendNotification);
    
    addAndMakeVisible (display = new SpeakerDisplay());
}

SpeakerEditor::SpeakerEditor (const File & f)
:   SpeakerEditor()
{
    openSettings (f);
}

SpeakerEditor::~SpeakerEditor()
{
    
}

void SpeakerEditor::buttonClicked (Button * button)
{
    if (button == addSpeakerButton)
    {
        addSpeaker();
        removeSpeakerButton->setEnabled (true);
    }
    else if (button == removeSpeakerButton)
    {
        removeSpeaker (listBox->getSelectedRow());
    }
}

void SpeakerEditor::sliderValueChanged (juce::Slider * slider)
{
    if (slider == inclinationSlider || slider == azimuthSlider)
    {
        speaker[listBox->getSelectedRow()].setDirection (degToRad (inclinationSlider->getValue()),
                                                         degToRad (azimuthSlider->getValue()));
    }
    else if (slider == shapeSlider)
    {
        speaker[listBox->getSelectedRow()].directionality = shapeSlider->getValue();
    }
    
    display->setSpeaker (speaker);
}

void SpeakerEditor::addSpeaker()
{
    speaker.push_back (Speaker (Vec (1, 0, 0), 1));
    listBox->updateContent();
    listBox->selectRow (speaker.size() - 1, sendNotification);
    speakerSelected (listBox->getSelectedRow());
    
    display->setSpeaker (speaker);
}

void SpeakerEditor::removeSpeaker (const int index)
{
    speaker.erase (speaker.begin() + index);
    listBox->updateContent();
    listBox->selectRow (index ? index - 1 : index, sendNotification);
    speakerSelected (listBox->getSelectedRow());
    
    if (speaker.size() == 1)
    {
        removeSpeakerButton->setEnabled (false);
    }
    
    display->setSpeaker (speaker);
}

void SpeakerEditor::speakerSelected (const int row)
{
    inclinationSlider->setValue (wrap (inclinationSlider->getMinimum(),
                                       inclinationSlider->getMaximum(),
                                       radToDeg (speaker[listBox->getSelectedRow()].getInclination())),
                                 dontSendNotification);
    azimuthSlider->setValue (wrap (azimuthSlider->getMinimum(),
                                   azimuthSlider->getMaximum(),
                                   radToDeg (speaker[listBox->getSelectedRow()].getAzimuth())),
                             dontSendNotification);
    shapeSlider->setValue (wrap (shapeSlider->getMinimum(),
                                 shapeSlider->getMaximum(),
                                 speaker[listBox->getSelectedRow()].directionality),
                           dontSendNotification);
    
    display->setSelectedIndex (row);
}

void SpeakerEditor::deleteKeyPressed (const int lastRowSelected)
{
    removeSpeaker (lastRowSelected);
}

Real SpeakerEditor::radToDeg (const Real rad)
{
    return (rad * 180) / M_PI;
}

Real SpeakerEditor::degToRad (const Real deg)
{
    return (deg * M_PI) / 180;
}

void SpeakerEditor::resized()
{
    const int spacing = 5;
    const int controlHeight = 25;
    
    Rectangle<int> listBoxBounds = getLocalBounds();
    listBoxBounds = listBoxBounds.removeFromLeft (200);
    listBoxBounds = listBoxBounds.removeFromTop (listBoxBounds.getHeight() - controlHeight - spacing);
    
    listBox->setBounds (listBoxBounds);
    
    addSpeakerButton->setBounds (listBox->getX(),
                                 listBox->getBottom() + spacing,
                                 controlHeight,
                                 controlHeight);
    removeSpeakerButton->setBounds (addSpeakerButton->getRight() + spacing,
                                    addSpeakerButton->getY(),
                                    controlHeight,
                                    controlHeight);
    
    const int labelWidth = 80;
    const int sliderWidth = getWidth() - listBox->getWidth() - labelWidth - (2 * spacing);
    
    inclinationLabel->setBounds (listBox->getRight() + spacing,
                                 listBox->getY(),
                                 labelWidth,
                                 controlHeight);
    
    inclinationSlider->setBounds (inclinationLabel->getRight() + spacing,
                                  inclinationLabel->getY(),
                                  sliderWidth,
                                  controlHeight);
    
    azimuthLabel->setBounds (listBox->getRight() + spacing,
                             inclinationLabel->getBottom() + spacing,
                             labelWidth,
                             controlHeight);
    
    azimuthSlider->setBounds (azimuthLabel->getRight() + spacing,
                              azimuthLabel->getY(),
                              sliderWidth,
                              controlHeight);
    
    shapeLabel->setBounds (listBox->getRight() + spacing,
                           azimuthLabel->getBottom() + spacing,
                           labelWidth,
                           controlHeight);
    
    shapeSlider->setBounds (shapeLabel->getRight() + spacing,
                            shapeLabel->getY(),
                            sliderWidth,
                            controlHeight);
    
    const int displayTop = shapeLabel->getBottom() + spacing;
    
    display->setBounds (inclinationLabel->getX(),
                        displayTop,
                        labelWidth + sliderWidth + spacing,
                        listBox->getBottom() - displayTop);

}

void SpeakerEditor::setEnabled (bool shouldBeEnabled)
{
    addSpeakerButton->setEnabled (shouldBeEnabled);
    removeSpeakerButton->setEnabled (speaker.size() > 1 && shouldBeEnabled);
    
    inclinationSlider->setEnabled (shouldBeEnabled);
    azimuthSlider->setEnabled (shouldBeEnabled);
    shapeSlider->setEnabled (shouldBeEnabled);
    
    listBox->setEnabled (shouldBeEnabled);

    inclinationSlider->setTextBoxIsEditable (shouldBeEnabled);
    azimuthSlider->setTextBoxIsEditable (shouldBeEnabled);
    shapeSlider->setTextBoxIsEditable (shouldBeEnabled);
}

bool SpeakerEditor::empty() const
{
    return speaker.empty();
}

std::vector<Speaker> SpeakerEditor::getSpeakers() const
{
    return speaker;
}

void SpeakerEditor::openSettings (const File & f)
{
    speaker = fromVar<vector<Speaker> > () (JSON::parse (f));
    
    listBox->updateContent();
    listBox->selectRow (speaker.size() - 1, sendNotification);
    speakerSelected (listBox->getSelectedRow());

    display->setSpeaker (speaker);
}

void SpeakerEditor::saveSettings (const File & f) const
{
    FileOutputStream fos (f);
    JSON::writeToStream (fos, getVar (speaker));
}