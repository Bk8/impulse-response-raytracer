//
//  SpeakerListBoxModel.h
//  flattener
//
//  Created by Reuben Thomas on 25/01/2014.
//
//

#ifndef __flattener__SpeakerListBoxModel__
#define __flattener__SpeakerListBoxModel__

#include "JuceHeader.h"
#include "Speaker.h"

class SpeakerListBoxModel
:   public ListBoxModel
{
public:
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void speakerSelected (const int row) = 0;
        virtual void deleteKeyPressed (const int lastRowSelected) = 0;
    };
    
    SpeakerListBoxModel (std::vector<Speaker> & spaker);
    
    int getNumRows();
    void paintListBoxItem
    (   int rowNumber
    ,   Graphics & g
    ,   int width
    ,   int height
    ,   bool isRowSelected
    );
    
    String getTooltipForRow (int row);
    
    void addListener (Listener * l);
    void removeListener (Listener * l);
    
    void listBoxItemClicked (int row, const MouseEvent & e);
    void deleteKeyPressed (int lastRowSelected);
private:
    ListenerList<Listener> listener;
    std::vector<Speaker> & speaker;
};

#endif /* defined(__flattener__SpeakerListBoxModel__) */
