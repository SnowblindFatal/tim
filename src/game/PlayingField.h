/* 
 * File:   PlayingField.h
 * Author: js
 *
 * Created on October 28, 2013, 9:30 PM
 */

#ifndef PLAYINGFIELD_H
#define	PLAYINGFIELD_H

#include "GameState.h"
#include <SFML/Graphics.hpp>


class PlayingField : public GameState {
public:
    PlayingField(sf::RenderWindow& _App, Resources& res, Settings& set) : GameState(_App, res, set){}
    virtual ~PlayingField(){}
private:

};

#endif	/* PLAYINGFIELD_H */

