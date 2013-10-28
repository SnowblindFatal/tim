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
    explicit MainMenu(sf::RenderWindow& window) : GameState(window){}
    MainMenu(const MainMenu& orig) = delete;
    MainMenu operator=(const MainMenu& orig) = delete;
    virtual ~MainMenu(){}
    void run();
private:
    void handleKeyPress(sf::Event event);
    void drawStuff();
};

#endif	/* MAINMENU_H */

