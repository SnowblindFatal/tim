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
    sf::Font coolFont; //!!Temp shit I can't wait to delete.
    if (!coolFont.LoadFromFile("res/fonts/BorisBlackBloxx.ttf"))
    {
        return GameState::StateSelect::Exit;
    }
    sf::String welcomeText("It's time to kick ass and chew bubblegum\nand I'm all outta gum", coolFont, 30);
    welcomeText.SetPosition((App.GetWidth() - welcomeText.GetRect().GetWidth()) / 2, (App.GetHeight() - welcomeText.GetRect().GetHeight()) / 2);
    while (!done)
    {
        sf::Event event;
        while (App.GetEvent(event))
        {
            // Close window : exit
            if (event.Type == sf::Event::Closed)
            {
                retval = GameState::StateSelect::Exit;
                done = true;
            }
            else if (event.Type == sf::Event::KeyPressed)
            {
                handleKeyPress(event);
            }
        }
        App.Clear();
        App.Draw(welcomeText); //!!Temp shit I can't wait to delete.
        drawStuff(); //do the actual drawing here!
        App.Display();
    }
    return retval;
}

void PlayMode::drawStuff()
{
    
}
    
void PlayMode::handleKeyPress(sf::Event event)
{
    //More complex stuff will go to their own methods.
    if (event.Key.Code == sf::Key::Escape || event.Key.Code == sf::Key::Q)
    {
        retval = GameState::StateSelect::Exit;
        done = true;
    }
    else if (event.Key.Code == sf::Key::Return || event.Key.Code == sf::Key::E)
    {
        retval = GameState::StateSelect::Edit;
        done = true;
    }
    else if (event.Key.Code == sf::Key::Space || event.Key.Code == sf::Key::M)
    {
        retval = GameState::StateSelect::Menu;
        done = true;
    }
}

