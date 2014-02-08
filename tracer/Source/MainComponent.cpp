#include "MainComponent.h"

#include "Scene.h"

static ScopedPointer<ApplicationCommandManager> applicationCommandManager;

MainContentComponent::MainContentComponent()
{
    addAndMakeVisible (tracer = new Tracer());
    
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
    FileChooser fc ("Choose a file to save...",
                    File::getCurrentWorkingDirectory(),
                    "*.json");
    
    if (fc.browseForFileToSave (true))
    {
        tracer->doTrace (fc.getResult());
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
    ,   wireframe
    };
    
    commands.addArray (ids, numElementsInArray (ids));
}

void MainContentComponent::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{    
    switch (commandID)
    {
        case open:
            result.setInfo ("Open...", "Open an object file to render", "File", 0);
            result.addDefaultKeypress ('o', ModifierKeys::commandModifier);
            break;
            
        case trace:
            result.setInfo ("Trace...", "Choose render options and trace", "File", 0);
            result.addDefaultKeypress ('t', ModifierKeys::commandModifier);
            result.setActive (tracer->canTrace());
            break;
            
        case wireframe:
            result.setInfo ("Wireframe", "Use wireframe mode for rendering scene", "View", 0);
            result.addDefaultKeypress ('w', ModifierKeys::commandModifier);
            result.setTicked (tracer->isWireframe());
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
            
        case wireframe:
            tracer->setWireframe (! tracer->isWireframe());
            break;
            
        default:
            return false;
    }
    
    return true;
}