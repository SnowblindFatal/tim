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
#include "Level.h"

class PlayMode : public PlayingField {
public:
    PlayMode(sf::RenderWindow& _App, Resources& res) : PlayingField(_App, res), simulate(false), level(_App), drawDebug(false) {}
    PlayMode(const PlayMode& orig) = delete;
    PlayMode operator=(const PlayMode& orig) = delete;
    virtual ~PlayMode(){}
    GameState::StateSelect run();
    void set_simulate(); // Runs the Box2D Simulator for a few steps.
    void set_drawdebug(); // Draws the debug info.
private:
    bool simulate;
    void drawStuff();
    void handleKeyPress(sf::Event event);
    LevelData level;
    bool drawDebug;

};

#endif	/* PLAYMODE_H */

