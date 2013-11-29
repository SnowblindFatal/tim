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

class MainMenu : public GameState {
public:
private:
            
    
    
public:
    explicit MainMenu(sf::RenderWindow& _App) : GameState(_App), gui(_App)  {}
    MainMenu(const MainMenu& orig) = delete;
    MainMenu operator=(const MainMenu& orig) = delete;
    virtual ~MainMenu(){}
    GameState::StateSelect run();
private:

	tgui::Gui gui;
    void handleKeyPress(sf::Event event);
    void drawStuff();
};

#endif	/* MAINMENU_H */

