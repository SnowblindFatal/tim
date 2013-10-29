/* 
 * File:   BaseGame.cpp
 * Author: js
 * 
 * Created on October 28, 2013, 9:32 PM
 */

#include "BaseGame.h"
#include <SFML/Graphics.hpp>
#include <list>
#include "GameState.h"
#include "MainMenu.h"
#include "EditMode.h"
#include "PlayMode.h"

void BaseGame::start()
{
    initialise();
    
    
    MainMenu menu(App);
    EditMode edit(App);
    PlayMode play(App);
    GameState::StateSelect stateSelector = GameState::StateSelect::Menu;
    
    while (stateSelector != GameState::StateSelect::Exit)
    {
        if (stateSelector == GameState::StateSelect::Play)
        {
            stateSelector = play.run();
        }
        else if (stateSelector == GameState::StateSelect::Edit)
        {
            stateSelector = edit.run();
        }
        else if (stateSelector == GameState::StateSelect::Menu)
        {
            stateSelector = menu.run();
        }
    }
    
}

void BaseGame::initialise()
{
    App.Create(sf::VideoMode(800, 600, 32), "The Incredibly Violent Machine");
    App.SetFramerateLimit(60); // Limit to 60 frames per second
    App.UseVerticalSync(true);
    
    //TODO: load settings from config file
    //TODO: load resources into a resource object
}


