/* 
 * File:   PlayMode.h
 * Author: js
 *
 * Created on October 28, 2013, 9:29 PM
 */

#ifndef PLAYMODE_H
#define	PLAYMODE_H

#include "PlayingField.h"
#include <SFML/Graphics.hpp>

class PlayMode : public PlayingField {
public:
    PlayMode(sf::RenderWindow& _App, Resources& res, Settings& set) : PlayingField(_App, res, set){}
    PlayMode(const PlayMode& orig) = delete;
    PlayMode operator=(const PlayMode& orig) = delete;
    virtual ~PlayMode(){}
    GameState::StateSelect run();
private:
    void drawStuff();
    void handleKeyPress(sf::Event event);

};

#endif	/* PLAYMODE_H */

