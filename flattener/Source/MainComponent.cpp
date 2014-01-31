#include "MainComponent.h"

static ScopedPointer<ApplicationCommandManager> applicationCommandManager;

MainContentComponent::MainContentComponent()
{
    addAndMakeVisible (flattener = new Flattener (raytrace));
    
    ApplicationCommandManager& commandManager = getApplicationCommandManager();
    
    addKeyListener (commandManager.getKeyMappings());
    commandManager.registerAllCommandsForTarget (this);
    commandManager.registerAllCommandsForTarget (JUCEApplication::getInstance());
    
    mainMenuBarModel = new MainMenuBarModel();
    addAndMakeVisible (mainMenuBarComponent = new MenuBarComponent (mainMenuBarModel));
    
    setWantsKeyboardFocus (true);
}

MainContentComponent::~MainContentComponent()
{
#if JUCE_MAC
    MenuBarModel::setMacMainMenu (nullptr);
#endif
    PopupMenu::dismissAllActiveMenus();
    
    applicationCommandManager = nullptr;
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colours::black);
}

void MainContentComponent::resized()
{
    mainMenuBarComponent->setBounds (getLocalBounds().removeFromTop (LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));

    flattener->setBounds (getLocalBounds().removeFromBottom (getHeight() - LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));
}

ApplicationCommandManager& MainContentComponent::getApplicationCommandManager()
{
    if (applicationCommandManager == nullptr)
        applicationCommandManager = new ApplicationCommandManager();
    
    return *applicationCommandManager;
}

ApplicationCommandTarget* MainContentComponent::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}

void MainContentComponent::getAllCommands (Array <CommandID>& commands)
{
    const CommandID ids[] =
    {   openTrace
    ,   flatten
    };
    
    commands.addArray (ids, numElementsInArray (ids));
}

void MainContentComponent::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    const String generalCategory ("File");
    
    switch (commandID)
    {
        case openTrace:
            result.setInfo ("Open Trace...", "Open a JSON file containing trace data", generalCategory, 0);
            result.addDefaultKeypress ('o', ModifierKeys::shiftModifier | ModifierKeys::commandModifier);
            break;
            
        case flatten:
            result.setInfo ("Flatten...", "Choose flatten options and create an audio file", generalCategory, 0);
            result.addDefaultKeypress ('f', ModifierKeys::commandModifier);
            result.setActive (flattener->canFlatten());
            break;
            
        default:
            break;
    }
}

bool MainContentComponent::perform (const InvocationInfo& info)
{
    switch (info.commandID)
    {
        case openTrace:
            doOpenTrace();
            break;
            
        case flatten:
            doFlatten();
            break;
            
        default:
            return false;
    }
    
    return true;
}

void MainContentComponent::doOpenTrace()
{
    FileChooser fc ("Choose a file to open...",
                    File::getCurrentWorkingDirectory(),
                    "*.json");
    
    if (fc.browseForFileToOpen())
    {
        flattener->loadTrace (fc.getResult());
    }
}

void MainContentComponent::doFlatten()
{
    FileChooser fc ("Choose a file to save...",
                    File::getCurrentWorkingDirectory(),
                    "*.aif,*.aiff");
    
    if (fc.browseForFileToSave (true))
    {
        flattener->writeAudio (fc.getResult());
    }
}