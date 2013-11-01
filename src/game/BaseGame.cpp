/* 
 * File:   BaseGame.cpp
 * Author: js
 * 
 * Created on October 28, 2013, 9:32 PM
 */

#include "BaseGame.h"
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <list>
#include "MainMenu.h"
#include "EditMode.h"
#include "PlayMode.h"

void BaseGame::start()
{
    initialise();
    
    Settings settings("cfg/config.txt");
    Resources resources("res/");
    
    MainMenu menu(App, resources, settings);
    EditMode edit(App, resources, settings);
    PlayMode play(App, resources, settings);
    
    
    
    GameState::StateSelect nextState = GameState::StateSelect::Menu;
    
    while (nextState != GameState::StateSelect::Exit)
    {
        if (nextState == GameState::StateSelect::Play)
        {
            nextState = play.run();
        }
        else if (nextState == GameState::StateSelect::Edit)
        {
            nextState = edit.run();
        }
        else if (nextState == GameState::StateSelect::Menu)
        {
            nextState = menu.run();
        }
    }
    
}

void BaseGame::initialise()
{
    App.create(sf::VideoMode(800, 600, 32), "The Incredibly Violent Machine");
    App.setFramerateLimit(60); // Limit to 60 frames per second
}


