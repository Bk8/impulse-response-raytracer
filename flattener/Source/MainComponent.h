#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "Scene.h"

#include "Flattener.h"
#include "WriteThread.h"
#include "MainMenuBarModel.h"

class MainContentComponent
:   public Component
,   public ApplicationCommandTarget
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
    {
        openTrace = 0x2000,
        flatten,
        openSettings,
        saveSettings,
    };
private:
    void doOpenTrace();
    void doFlatten();
    void doOpenSettings();
    void doSaveSettings();
    
    std::vector<RayTrace> raytrace;
        
    ScopedPointer<Flattener> flattener;
    
    ScopedPointer<ListBox> speakerList;
    
    ScopedPointer<MainMenuBarModel> mainMenuBarModel;
    ScopedPointer<MenuBarComponent> mainMenuBarComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
