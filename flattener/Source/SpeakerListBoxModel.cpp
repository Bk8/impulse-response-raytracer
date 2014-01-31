//
//  SpeakerListBoxModel.cpp
//  flattener
//
//  Created by Reuben Thomas on 25/01/2014.
//
//

#include "SpeakerListBoxModel.h"

SpeakerListBoxModel::SpeakerListBoxModel (std::vector<Speaker> & speaker)
:   speaker (speaker)
{
    
}

int SpeakerListBoxModel::getNumRows()
{
    return speaker.size();
}

void SpeakerListBoxModel::paintListBoxItem
(   int rowNumber
,   Graphics & g
,   int width
,   int height
,   bool isRowSelected
)
{
    g.setColour (Colours::black);

    if (isRowSelected)
    {
        g.fillAll (Colours::darkgrey);
        g.setColour (Colours::white);
    }
    
    g.setFont (height * 0.7f);
    
    g.drawText ("speaker " + String (rowNumber + 1),
                5, 0, width, height,
                Justification::centredLeft, true);
}

String SpeakerListBoxModel::getTooltipForRow (int row)
{
    String ret;
    
    ret += "x: ";
    ret += String (speaker[row].direction.x);
    
    ret += ", y: ";
    ret += String (speaker[row].direction.y);
    
    ret += ", z: ";
    ret += String (speaker[row].direction.z);
    
    ret += ", shape: ";
    ret += String (speaker[row].directionality);
    
    return ret;
}

void SpeakerListBoxModel::addListener (Listener * l)
{
    listener.add (l);
}

void SpeakerListBoxModel::removeListener (Listener * l)
{
    listener.remove (l);
}

void SpeakerListBoxModel::listBoxItemClicked (int row, const MouseEvent & e)
{
    listener.call (&Listener::speakerSelected, row);
}

void SpeakerListBoxModel::deleteKeyPressed (const int lastRowSelected)
{
    listener.call (&Listener::deleteKeyPressed, lastRowSelected);
}