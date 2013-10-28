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
    
    GameState *currentState, *nextState;
    
    MainMenu menu(App);
    EditMode edit(App);
    PlayMode play(App);
    nextState = &menu;
    
    while (nextState != NULL)
    {
        currentState = nextState;
        nextState = NULL;
        currentState->run();
        //TODO: the run method returns an enum value which determines which mode will be picked next.
    }
    
}

void BaseGame::initialise()
{
    App.Create(sf::VideoMode(800, 600, 32), "SFML Window");
    App.SetFramerateLimit(60); // Limit to 60 frames per second
    App.UseVerticalSync(true);
    
    //TODO: load settings from config file
    //TODO: load resources into a resource object
}


