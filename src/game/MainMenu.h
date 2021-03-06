/* 
 * File:   MainMenu.h
 * Author: js
 *
 * Created on October 28, 2013, 9:30 PM
 */

#ifndef MAINMENU_H
#define	MAINMENU_H

#include "GameState.h"
#include <TGUI/TGUI.hpp>
#include <string>
#include <vector>

class MainMenu : public GameState {
public:
private:
            
    
    
public:
    explicit MainMenu(sf::RenderWindow& _App) : GameState(_App), gui(_App), gui_loaded(false)  {}
    MainMenu(const MainMenu& orig) = delete;
    MainMenu operator=(const MainMenu& orig) = delete;
    virtual ~MainMenu(){}
    GameState::StateSelect run();
private:
    void initialiseGUI();

	tgui::Gui gui;
    void handleKeyPress(sf::Event event);
    void handleGuiEvents();
    void drawStuff();
    void selectLevel();
    bool gui_loaded;
    void disableOrEnableWidgets(std::string value);
    void populateLevelSelector(std::vector<std::string>& levelNames);
    void handleLevelPicker(std::vector<std::string>& levelNames);
};

#endif	/* MAINMENU_H */

