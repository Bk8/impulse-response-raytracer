#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "Tracer.h"

#include "MainMenuBarModel.h"

class MainContentComponent:
public Component,
public ApplicationCommandTarget
{
public:
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();
    
    static ApplicationCommandManager& getApplicationCommandManager();
    
    ApplicationCommandTarget* getNextCommandTarget();
    void getAllCommands (Array <CommandID>& commands);
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result);
    bool perform (const InvocationInfo& info);
    
    enum CommandIDs
    {   open = 0x2000
    ,   trace
    };
    
private:
    void doOpen();
    void doTrace();
    
    ScopedPointer<Tracer> tracer;
    
    ScopedPointer<MainMenuBarModel> mainMenuBarModel;
    ScopedPointer<MenuBarComponent> mainMenuBarComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif