//
//  ValueEditorComponent.h
//  tracer
//
//  Created by Reuben Thomas on 02/02/2014.
//
//

#ifndef __tracer__ValueEditorComponent__
#define __tracer__ValueEditorComponent__

#include "JuceHeader.h"

template<typename T>
class ValueEditor
:   public Component
,   public Button::Listener
,   public TextEditor::Listener
{
public:
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        virtual void valueEditorValueChanged (ValueEditor<T> * const ve) = 0;
    };
    
    ValueEditor()
    :   minBounded (false)
    ,   maxBounded (false)
    {
        addAndMakeVisible(textEditor = new TextEditor());
        addAndMakeVisible(addButton = new TextButton());
        addAndMakeVisible(subButton = new TextButton());
        
        addButton->setButtonText("+");
        subButton->setButtonText("-");
        
        addButton->setColour(TextButton::buttonColourId, Colours::lightgrey);
        subButton->setColour(TextButton::buttonColourId, Colours::lightgrey);
        
        textEditor->addListener (this);
        addButton->addListener (this);
        subButton->addListener (this);
    }
    
    virtual ~ValueEditor()
    {
        
    }
    
    void resized()
    {
        const int buttonWidth = 25;
        const int padding = 5;
        
        subButton->setBounds (0, 0, buttonWidth, getHeight());
        addButton->setBounds (getWidth() - buttonWidth, 0, buttonWidth, getHeight());
        
        textEditor->setBounds(subButton->getRight() + padding,
                              0,
                              getWidth() - (buttonWidth + padding) * 2,
                              getHeight());
    }
    
    void buttonClicked (Button * b)
    {
        if (b == addButton)
        {
            setValue (getValue() + increment, sendNotification);
        }
        else if (b == subButton)
        {
            setValue (getValue() - increment, sendNotification);
        }
    }
    
    void textEditorReturnKeyPressed (TextEditor & te)
    {
        if (&te == textEditor)
        {
            moveKeyboardFocusToSibling(true);
            
            setValue (textEditor->getText(), sendNotification);
        }
    }
    
    void textEditorEscapeKeyPressed (TextEditor & te)
    {
        if (&te == textEditor)
        {
            moveKeyboardFocusToSibling(true);
            
            setValue (value, dontSendNotification);
        }
    }
    
    void textEditorFocusLost (TextEditor & te)
    {
        if (&te == textEditor)
        {
            setValue (textEditor->getText(), dontSendNotification);
        }
    }
    
    T getValue() {return value;}
    void setValue (const T & in, NotificationType nt)
    {
        value = in;

        if (minBounded && value < minimum) value = minimum;
        if (maxBounded && maximum < value) value = maximum;
        
        textEditor->setText (String (value));
        
        if (nt)
        {
            BailOutChecker checker (this);
        
            if (! checker.shouldBailOut())
            {
                listener.callChecked (checker, &Listener::valueEditorValueChanged, this);
            }
        }
    }
    
    void setValue (const String & in, NotificationType nt);
    
    T getIncrement() {return increment;}
    void setIncrement (const T & in)
    {
        increment = in;
    }
    
    void addListener (Listener * l)
    {
        listener.add (l);
    }
    void removeListener (Listener * l)
    {
        listener.remove (l);
    }
    
    void setMinimum (const T & t)
    {
        minBounded = true;
        minimum = t;
    }
    
    void setMaximum (const T & t)
    {
        maxBounded = true;
        maximum = t;
    }
    
    void noMinimum()
    {
        minBounded = false;
    }
    
    void noMaximum()
    {
        maxBounded = false;
    }
    
    void enableBounds (const T & mi, const T & ma)
    {
        setMinimum (mi);
        setMaximum (ma);
    }
    
    void disableBounds()
    {
        noMinimum();
        noMaximum();
    }
    
    void setEnabled (bool b)
    {
        textEditor->setEnabled (b);
        addButton->setEnabled (b);
        subButton->setEnabled (b);
    }
    
private:
    ScopedPointer<TextEditor> textEditor;
    ScopedPointer<TextButton> addButton;
    ScopedPointer<TextButton> subButton;
    
    ListenerList<Listener> listener;
    
    T value;
    T increment;
    
    bool minBounded;
    bool maxBounded;
    
    T minimum;
    T maximum;
};



#endif /* defined(__tracer__ValueEditorComponent__) */
