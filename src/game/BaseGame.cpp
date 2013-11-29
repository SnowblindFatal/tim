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
#include "../general/Settings.h"

void BaseGame::start()
{
    initialise();
    
    //No settings to load so far.
    //Settings::loadSettings("cfg/config.txt");
    
    try
    {
        Resources::getInstance().loadResources();
    }
    catch (const std::string& e)
    {
        std::cout << e;
        return;
    }

    //Resources::getInstance().getMusic("MAP01.ogg").play();
    
    MainMenu menu(App);
    EditMode edit(App);
    PlayMode play(App);
    
    
    
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
	sf::ContextSettings sf_settings;
	sf_settings.antialiasingLevel=8;
    App.create(sf::VideoMode(800, 600, 32), "The Incredibly Violent Machine",sf::Style::Default,sf_settings);
    App.setFramerateLimit(60); // Limit to 60 frames per second
}


