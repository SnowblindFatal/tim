/* 
 * File:   BaseGame.h
 * Author: js
 *
 * Created on October 28, 2013, 9:31 PM
 */

#ifndef BASEGAME_H
#define	BASEGAME_H

#include <SFML/Graphics.hpp>

class BaseGame {
public:
private:
    sf::RenderWindow App;
    
    
public:
    explicit BaseGame(){}
    BaseGame(const BaseGame& orig) = delete;
    BaseGame operator=(const BaseGame& orig) = delete;
    void start();
private:
    void initialise();

};

#endif	/* BASEGAME_H */

