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
    PlayMode(sf::RenderWindow& window) : PlayingField(window){}
    PlayMode(const PlayMode& orig) = delete;
    PlayMode operator=(const PlayMode& orig) = delete;
    virtual ~PlayMode(){}
    void run();
private:

};

#endif	/* PLAYMODE_H */
