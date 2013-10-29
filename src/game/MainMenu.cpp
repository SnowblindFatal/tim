/* 
 * File:   MainMenu.cpp
 * Author: js
 * 
 * Created on October 28, 2013, 9:30 PM
 */

#include "MainMenu.h"
#include "GameState.h"
GameState::StateSelect MainMenu::run()
{
    sf::Font coolFont;
    if (!coolFont.LoadFromFile("res/fonts/BLASTER.TTF"))
    {
        return GameState::StateSelect::Exit;
    }
    sf::String welcomeText("Eat shit and die", coolFont, 100);
    welcomeText.SetPosition((App.GetWidth() - welcomeText.GetRect().GetWidth()) / 2, (App.GetHeight() - welcomeText.GetRect().GetHeight()) / 2);
    //welcomeText.SetPosition(welcomeText.GetCenter().y);
    while (App.IsOpened())
    {
        // Process events
        sf::Event event;
        while (App.GetEvent(event))
        {
            // Close window : exit
            if (event.Type == sf::Event::Closed)
            {
                App.Close();
            }
            else if (event.Type == sf::Event::KeyPressed)
            {
                handleKeyPress(event);
            }
        }
        // Clear screen
        App.Clear();
        // Draw the string
        App.Draw(welcomeText);
        drawStuff();
        // Update the window
        App.Display();
    }
    return GameState::StateSelect::Exit;
}

void MainMenu::drawStuff()
{
    
}
    
void MainMenu::handleKeyPress(sf::Event event)
{
    if (event.Key.Code == sf::Key::Escape)
    {
        App.Close();
    }
}