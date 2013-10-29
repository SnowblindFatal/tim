/* 
 * File:   GameState.h
 * Author: js
 *
 * Created on October 28, 2013, 9:57 PM
 */

#ifndef GAMESTATE_H
#define	GAMESTATE_H

#include <SFML/Graphics.hpp>

class GameState {
public:
    enum StateSelect
    {
        Play,
        Menu,
        Edit,
        Exit,
    };
    
    explicit GameState(sf::RenderWindow& _window) : App(_window) {}
    virtual ~GameState(){}
    virtual StateSelect run() = 0;
    
protected:
    sf::RenderWindow& App;

};

#endif	/* GAMESTATE_H */

