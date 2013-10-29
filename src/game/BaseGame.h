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
    explicit BaseGame(){}
    BaseGame(const BaseGame& orig) = delete;
    BaseGame operator=(const BaseGame& orig) = delete;
    void start();
private:
    void initialise();

    sf::RenderWindow App;
};

#endif	/* BASEGAME_H */

