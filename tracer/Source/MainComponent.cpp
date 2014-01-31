#include "MainComponent.h"

#include "Scene.h"

static ScopedPointer<ApplicationCommandManager> applicationCommandManager;

MainContentComponent::MainContentComponent()
{
    addAndMakeVisible (tracer = new Tracer (raytrace));
    
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
    
    tracer->setBounds (getLocalBounds().removeFromBottom (getHeight() - LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));
}

void MainContentComponent::doOpen()
{
    FileChooser fc ("Choose a file to open...",
                    File::getCurrentWorkingDirectory(),
                    "*.obj");
    
    if (fc.browseForFileToOpen())
    {
        tracer->loadObjFile (fc.getResult());
    }
}

void MainContentComponent::doTrace()
{
    tracer->doTrace();
}

void MainContentComponent::doWriteTrace()
{
    FileChooser fc ("Choose a file to save...",
                    File::getCurrentWorkingDirectory(),
                    "*.json");
    
    if (fc.browseForFileToSave (true))
    {
        tracer->writeTrace (fc.getResult());
    }
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
    {   open
    ,   trace
    ,   writeTrace
    };
    
    commands.addArray (ids, numElementsInArray (ids));
}

void MainContentComponent::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    const String generalCategory ("File");
    
    switch (commandID)
    {
        case open:
            result.setInfo ("Open...", "Open an object file to render", generalCategory, 0);
            result.addDefaultKeypress ('o', ModifierKeys::commandModifier);
            break;
            
        case trace:
            result.setInfo ("Trace...", "Choose render options and trace", generalCategory, 0);
            result.addDefaultKeypress ('t', ModifierKeys::commandModifier);
            result.setActive (tracer->canTrace());
            break;
            
        case writeTrace:
            result.setInfo ("Export Trace...", "Write trace data to a JSON file", generalCategory, 0);
            result.addDefaultKeypress ('s', ModifierKeys::commandModifier);
            result.setActive (! raytrace.empty() && ! tracer->isWriting());
            break;
            
        default:
            break;
    }
}

bool MainContentComponent::perform (const InvocationInfo& info)
{
    switch (info.commandID)
    {
        case open:
            doOpen();
            break;
            
        case trace:
            doTrace();
            break;
            
        case writeTrace:
            doWriteTrace();
            break;
            
        default:
            return false;
    }
    
    return true;
}