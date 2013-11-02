/* 
 * File:   MainMenu.h
 * Author: js
 *
 * Created on October 28, 2013, 9:30 PM
 */

#ifndef MAINMENU_H
#define	MAINMENU_H

#include "GameState.h"

class MainMenu : public GameState {
public:
private:
            
    
    
public:
    explicit MainMenu(sf::RenderWindow& _App, Resources& res) : GameState(_App, res){}
    MainMenu(const MainMenu& orig) = delete;
    MainMenu operator=(const MainMenu& orig) = delete;
    virtual ~MainMenu(){}
    GameState::StateSelect run();
private:
    void handleKeyPress(sf::Event event);
    void drawStuff();
};

#endif	/* MAINMENU_H */

