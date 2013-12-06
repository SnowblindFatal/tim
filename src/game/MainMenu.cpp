/* 
 * File:   MainMenu.cpp
 * Author: js
 * 
 * Created on October 28, 2013, 9:30 PM
 */

#include "MainMenu.h"
#include "GameState.h"
#include <TGUI/TGUI.hpp>
#include <stdexcept>

GameState::StateSelect MainMenu::run()
{
    done = false;
    
    initialiseGUI();
	
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
            gui.handleEvent(event);
        }
        handleGuiEvents();
        App.clear();
        drawStuff(); //do the actual drawing here!
        App.display();
    }
    return retval;
}

void MainMenu::drawStuff()
{
    gui.draw();
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

void MainMenu::handleGuiEvents()
{
    tgui::Callback callback;
    while (gui.pollCallback(callback))
    {
        if (callback.id == 1) 
        {
            Resources::getInstance().setCurrentLevel("level1.txt");
            retval = GameState::StateSelect::Play;
            done = true;
        }
        else if (callback.id == 2) 
        {
            //TODO: Level select
        }
        else if (callback.id == 3) 
        {
            retval = GameState::StateSelect::Edit;
            done = true;
        }
        else if (callback.id == 4) 
        {
            retval = GameState::StateSelect::Exit;
            done = true;
        }
    }
    
}

void MainMenu::initialiseGUI() 
{
    gui.setGlobalFont(Resources::getInstance().getFont("BorisBlackBloxx.ttf"));
    
    tgui::Picture::Ptr picture(gui);
    picture->load("res/textures/magic.jpg");
    picture->setSize(800, 600);
    
    tgui::Button::Ptr button(gui);
    button->load("TGUI/Black.conf");
    button->setPosition(250, 100);
    button->setText("(test mode)");
    button->setCallbackId(1);
    button->bindCallback(tgui::Button::LeftMouseClicked);
    button->setSize(300, 50);
    button->setTransparency(220);

    tgui::Button::Ptr button2(gui);
    button2->load("TGUI/Black.conf");
    button2->setPosition(250, 200);
    button2->setText("Choose level (TODO)");
    button2->setCallbackId(2);
    button2->bindCallback(tgui::Button::LeftMouseClicked);
    button2->setSize(500, 50);
    button2->setTransparency(220);

    tgui::Button::Ptr button3(gui);
    button3->load("TGUI/Black.conf");
    button3->setPosition(250, 300);
    button3->setText("Editor");
    button3->setCallbackId(3);
    button3->bindCallback(tgui::Button::LeftMouseClicked);
    button3->setSize(300, 50);
    button3->setTransparency(220);
    
    tgui::Button::Ptr button4(gui);
    button4->load("TGUI/Black.conf");
    button4->setPosition(250, 400);
    button4->setText("Quit");
    button4->setCallbackId(4);
    button4->bindCallback(tgui::Button::LeftMouseClicked);
    button4->setSize(300, 50);
    button4->setTransparency(220);
}