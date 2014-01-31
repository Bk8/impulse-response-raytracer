//
//  MainMenuBarModel.h
//  flattener
//
//  Created by Reuben Thomas on 25/01/2014.
//
//

#ifndef __flattener__MainMenuBarModel__
#define __flattener__MainMenuBarModel__

#include "JuceHeader.h"

class MainMenuBarModel
:   public Component
,   public MenuBarModel
{
public:
    MainMenuBarModel() {}
    virtual ~MainMenuBarModel() {}
    
    PopupMenu getMenuForIndex (int menuIndex, const String& menuName);
    void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/);
    StringArray getMenuBarNames();
};

#endif /* defined(__flattener__MainMenuBarModel__) */