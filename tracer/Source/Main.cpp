/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"


//==============================================================================
class tracerApplication  : public JUCEApplication
{
public:
    //==============================================================================
    tracerApplication() {}

    const String getApplicationName()       { return ProjectInfo::projectName; }
    const String getApplicationVersion()    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed()       { return true; }

    //==============================================================================
    void initialise (const String& commandLine)
    {
        mainWindow = new MainWindow();
    }

    void shutdown()
    {
        mainWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit()
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine)
    {
        
    }

    class MainWindow: public DocumentWindow
    {
    public:
        MainWindow()  : DocumentWindow ("tracer",
                                        Colours::lightgrey,
                                        DocumentWindow::allButtons)
        {
            setVisible (true);
            setUsingNativeTitleBar (true);
            setResizable (true, false);
            LookAndFeel::setDefaultLookAndFeel (&lookAndFeelV3);
            
            lookAndFeelV3.setColour (PopupMenu::highlightedBackgroundColourId, Colours::darkgrey);
            
            centreWithSize (900, 480);
            
            constrainer = new ComponentBoundsConstrainer();
            constrainer->setMinimumSize (getWidth(), getHeight());
            setConstrainer (constrainer);
            
            setContentOwned (new MainContentComponent(), false);
        }

        void closeButtonPressed()
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        LookAndFeel_V3 lookAndFeelV3;
        
        ScopedPointer<ComponentBoundsConstrainer> constrainer;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (tracerApplication)
