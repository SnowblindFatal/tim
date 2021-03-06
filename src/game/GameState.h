/* 
 * File:   GameState.h
 * Author: js
 *
 * Created on October 28, 2013, 9:57 PM
 */

#ifndef GAMESTATE_H
#define	GAMESTATE_H

#include <SFML/Graphics.hpp>
#include "../general/Resources.h"
#include "../general/Settings.h"
#include "Level.h"

class GameState {
public:
    enum StateSelect
    {
        Play,
        Menu,
        Edit,
        Exit
    };
    
    explicit GameState(sf::RenderWindow& _App) : App(_App), retval(GameState::StateSelect::Exit) {}
    virtual ~GameState(){}
    virtual StateSelect run() = 0;
    
    friend class LevelData;
protected:
    sf::RenderWindow& App;
    bool done;
    GameState::StateSelect retval;
    

};

#endif	/* GAMESTATE_H */

