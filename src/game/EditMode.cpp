/* 
 * File:   EditMode.cpp
 * Author: js
 * 
 * Created on October 28, 2013, 9:29 PM
 */

#include "EditMode.h"
#include "GameState.h"

GameState::StateSelect EditMode::run()
{
    done = false;
    sf::Font coolFont; //!!Temp shit I can't wait to delete.
    if (!coolFont.loadFromFile("res/fonts/BorisBlackBloxx.ttf"))
    {
        return GameState::StateSelect::Exit;
    }
    sf::Text welcomeText("You're an inspiration for birth control", coolFont, 30);
    welcomeText.setPosition((App.getSize().x - welcomeText.getLocalBounds().width) / 2, (App.getSize().y - welcomeText.getLocalBounds().height) / 2);
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
        App.draw(welcomeText); //!!Temp shit I can't wait to delete.
        drawStuff(); //do the actual drawing here!
        App.display();
    }
    return retval;
}

void EditMode::drawStuff()
{
    
}
    
void EditMode::handleKeyPress(sf::Event event)
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
        retval = GameState::StateSelect::Menu;
        done = true;
    }
}
