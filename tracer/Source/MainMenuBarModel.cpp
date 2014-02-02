//
//  MenuBar.cpp
//  rayverb
//
//  Created by Reuben Thomas on 28/12/2013.
//
//

#include "MainMenuBarModel.h"

#include "MainComponent.h"

PopupMenu MainMenuBarModel::getMenuForIndex (int menuIndex, const String& menuName)
{
    ApplicationCommandManager* commandManager = &MainContentComponent::getApplicationCommandManager();
    
    PopupMenu menu;
    
    if (menuIndex == 0)
    {
        menu.addCommandItem (commandManager, MainContentComponent::open);
        menu.addCommandItem (commandManager, MainContentComponent::trace);
    }
    
    return menu;
}

void MainMenuBarModel::menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/)
{
    
}

StringArray MainMenuBarModel::getMenuBarNames()
{
    const char* const names[] = { "File", nullptr };
    
    return StringArray (names);
}