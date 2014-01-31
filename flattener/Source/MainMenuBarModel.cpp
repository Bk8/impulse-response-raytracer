//
//  MainMenuBarModel.cpp
//  flattener
//
//  Created by Reuben Thomas on 25/01/2014.
//
//

#include "MainMenuBarModel.h"

#include "MainComponent.h"

PopupMenu MainMenuBarModel::getMenuForIndex (int menuIndex, const String& menuName)
{
    ApplicationCommandManager* commandManager = &MainContentComponent::getApplicationCommandManager();
    
    PopupMenu menu;
    
    if (menuName == "File")
    {
        menu.addCommandItem (commandManager, MainContentComponent::openTrace);
        menu.addCommandItem (commandManager, MainContentComponent::flatten);
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