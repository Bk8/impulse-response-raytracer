//
//  MenuBar.h
//  rayverb
//
//  Created by Reuben Thomas on 28/12/2013.
//
//

#ifndef __rayverb__MenuBar__
#define __rayverb__MenuBar__

#include "JuceHeader.h"

class MainMenuBarModel:
public Component,
public MenuBarModel {
public:
    MainMenuBarModel() {}
    virtual ~MainMenuBarModel() {}
    
    PopupMenu getMenuForIndex (int menuIndex, const String& menuName);
    void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/);
    StringArray getMenuBarNames();
};

#endif /* defined(__rayverb__MenuBar__) */