/* 
 * File:   PlayMode.cpp
 * Author: js
 * 
 * Created on October 28, 2013, 9:29 PM
 */

#include "PlayMode.h"
#include "GameState.h"
GameState::StateSelect PlayMode::run()
{
    done = false;
 
 
 
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

void PlayMode::drawStuff()
{
    
}
    
void PlayMode::handleKeyPress(sf::Event event)
{
    //More complex stuff will go to their own methods.
    if (event.key.code == sf::Keyboard::Escape)
    {
        retval = GameState::StateSelect::Exit;
        done = true;
    }
    else if (event.key.code == sf::Keyboard::Return)
    {
        retval = GameState::StateSelect::Edit;
        done = true;
    }
    else if (event.key.code == sf::Keyboard::Space)
    {
        retval = GameState::StateSelect::Menu;
        done = true;
    }
}
