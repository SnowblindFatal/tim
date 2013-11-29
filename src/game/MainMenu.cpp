/* 
 * File:   MainMenu.cpp
 * Author: js
 * 
 * Created on October 28, 2013, 9:30 PM
 */

#include "MainMenu.h"
#include "GameState.h"
#include <TGUI/TGUI.hpp>

GameState::StateSelect MainMenu::run()
{
    done = false;
    sf::Font coolFont; //!!Temp shit I can't wait to delete.
    if (!coolFont.loadFromFile("res/fonts/BorisBlackBloxx.ttf"))
    {
        return GameState::StateSelect::Exit;
    }
	
    while (!done)
    {
        sf::Event event;
        while (App.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
            {
                retval = GameState::StateSelect::Exit;
                done = true;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                handleKeyPress(event);
            }
        }
        App.clear();
        drawStuff(); //do the actual drawing here!
        App.display();
    }
    return retval;
}

void MainMenu::drawStuff()
{
    
}
    
void MainMenu::handleKeyPress(sf::Event event)
{
    //More complex stuff will go to their own methods.
    if (event.key.code == sf::Keyboard::Escape)
    {
        retval = GameState::StateSelect::Exit;
        done = true;
    }
    else if (event.key.code == sf::Keyboard::Return)
    {
        retval = GameState::StateSelect::Play;
        done = true;
    }
    else if (event.key.code == sf::Keyboard::Space)
    {
        retval = GameState::StateSelect::Edit;
        done = true;
    }
}
