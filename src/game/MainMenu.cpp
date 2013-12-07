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
#include <vector>

GameState::StateSelect MainMenu::run()
{
    done = false;
    if (!gui_loaded)
    {
        initialiseGUI();
        gui_loaded = true;
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
        if (callback.id == 2) 
        {
            selectLevel();
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

    tgui::Button::Ptr button2(gui);
    button2->load("TGUI/Black.conf");
    button2->setPosition(250, 200);
    button2->setText("Choose level");
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

void MainMenu::selectLevel()
{
    disableOrEnableWidgets("false");
    
    std::vector<std::string> levelNames;

    populateLevelSelector(levelNames);
    
    handleLevelPicker(levelNames);
    
    tgui::Panel::Ptr panel = gui.get("panel1");
    panel->removeAllWidgets();
    gui.remove(panel);
    disableOrEnableWidgets("true");
}

void MainMenu::populateLevelSelector(std::vector<std::string>& levelNames) 
{
    //the button callback id i will match the index in levelNames.
    //For instance, if the third button is clicked, it triggers
    //callback id 2, and the level name in index 2 of levelNames vector
    //will be used as the next level.
    
    int x = 20, y = 60, i = 0;
    tgui::Panel::Ptr panel(gui, "panel1");
    panel->setPosition(100, 50);
    panel->setSize(600, 400);
    panel->setBackgroundColor(sf::Color(60, 100, 140));
    for (const auto& pair : Resources::getInstance().getLevelInfo()) {
        levelNames.push_back(pair.first);
        tgui::Button::Ptr button(*panel);
        button->load("TGUI/Black.conf");
        button->setPosition(x, y);
        if (pair.second) {
            button->setText(pair.first + "\n (beaten)");
        } else {
            button->setText(pair.first);
        }
        button->setCallbackId(i);
        button->bindCallback(tgui::Button::LeftMouseClicked);
        button->setSize(90, 50);
        y += 60;
        if (y > 550) {
            x += 100;
            y = 60;
        }
        i++;
        panel->add(button);
    }
    
    tgui::Button::Ptr button(*panel);
    button->load("TGUI/Black.conf");
    button->setPosition(500, 10);
    button->setText("back");
    button->setCallbackId(99999);
    button->bindCallback(tgui::Button::LeftMouseClicked);
    button->setSize(50, 40);
}

void MainMenu::handleLevelPicker(std::vector<std::string>& levelNames)
{
    bool levelPickFinished = false;
    while (levelPickFinished == false) {
        sf::Event event;
        while (App.pollEvent(event)) {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                retval = GameState::StateSelect::Exit;
                done = true;
                return;
            }
            gui.handleEvent(event);
        }

        tgui::Callback callback;
        while (gui.pollCallback(callback)) {
            if (callback.id == 99999) {
                levelPickFinished = true;
            } else {
                Resources::getInstance().setCurrentLevel(levelNames.at(callback.id));
                retval = GameState::StateSelect::Play;
                done = true;
                levelPickFinished = true;
            }
        }

        App.clear();
        gui.draw();
        App.display();
    }
}


void MainMenu::disableOrEnableWidgets(std::string value) 
{
    auto widgets = gui.getWidgets();
    for (auto& widget : widgets)
    {
        widget->setProperty("Enabled", value);
    }
}