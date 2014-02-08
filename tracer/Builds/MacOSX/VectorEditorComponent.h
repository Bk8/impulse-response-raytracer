//
//  VectorEditorComponent.h
//  tracer
//
//  Created by Reuben Thomas on 02/02/2014.
//
//

#ifndef __tracer__VectorEditorComponent__
#define __tracer__VectorEditorComponent__

#include "JuceHeader.h"
#include "ValueEditorComponent.h"

template<typename T>
class VectorEditor
:   public Component
,   public ValueEditor<T>::Listener
{
public:
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        virtual void vectorEditorValueChanged (VectorEditor * const ve) = 0;
    };
    
    VectorEditor()
    {
        addAndMakeVisible(xEditor = new ValueEditor<double>());
        addAndMakeVisible(yEditor = new ValueEditor<double>());
        addAndMakeVisible(zEditor = new ValueEditor<double>());
        
        addAndMakeVisible(xLabel = new Label());
        addAndMakeVisible(yLabel = new Label());
        addAndMakeVisible(zLabel = new Label());
        
        xLabel->setText ("x", dontSendNotification);
        yLabel->setText ("y", dontSendNotification);
        zLabel->setText ("z", dontSendNotification);
        
        xEditor->setValue(0, dontSendNotification);
        yEditor->setValue(0, dontSendNotification);
        zEditor->setValue(0, dontSendNotification);
        
        xEditor->setIncrement(1);
        yEditor->setIncrement(1);
        zEditor->setIncrement(1);
        
        xEditor->addListener(this);
        yEditor->addListener(this);
        zEditor->addListener(this);
    }
    
    virtual ~VectorEditor()
    {
        
    }
    
    void resized()
    {
        const int controlHeight = 25;
        const int padding = 5;
        
        const int labelWidth = controlHeight;
        
        xLabel->setBounds (0, 0, labelWidth, controlHeight);
        xEditor->setBounds (xLabel->getRight() + padding,
                            xLabel->getY(),
                            getWidth() - labelWidth - padding,
                            controlHeight);
        
        yLabel->setBounds (0, xLabel->getBottom() + padding, labelWidth, controlHeight);
        yEditor->setBounds (yLabel->getRight() + padding,
                            yLabel->getY(),
                            getWidth() - labelWidth - padding,
                            controlHeight);
        
        zLabel->setBounds (0, yLabel->getBottom() + padding, labelWidth, controlHeight);
        zEditor->setBounds (zLabel->getRight() + padding,
                            zLabel->getY(),
                            getWidth() - labelWidth - padding,
                            controlHeight);
    }
    
    Vector3D<T> getValue() const
    {
        return Vector3D<T> (xEditor->getValue(),
                            yEditor->getValue(),
                            zEditor->getValue());
    }
    
    void valueEditorValueChanged (ValueEditor<double> * const ve)
    {
        BailOutChecker checker (this);
        
        if (! checker.shouldBailOut())
        {
            if (ve == xEditor || ve == yEditor || ve == zEditor)
            {
                listener.call (&Listener::vectorEditorValueChanged, this);
            }
        }
    }
    
    void addListener (Listener * l)
    {
        listener.add(l);
    }
    
    void removeListener (Listener * l)
    {
        listener.remove(l);
    }
    
    void setEnabled (bool b)
    {
        xEditor->setEnabled (b);
        yEditor->setEnabled (b);
        zEditor->setEnabled (b);
    }
    
private:
    ScopedPointer<ValueEditor<T> > xEditor;
    ScopedPointer<ValueEditor<T> > yEditor;
    ScopedPointer<ValueEditor<T> > zEditor;
    
    ScopedPointer<Label> xLabel;
    ScopedPointer<Label> yLabel;
    ScopedPointer<Label> zLabel;
    
    ListenerList<Listener> listener;
};

#endif /* defined(__tracer__VectorEditorComponent__) */
