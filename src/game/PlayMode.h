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
    PlayMode(sf::RenderWindow& _App) : PlayingField(_App), simulate(false), level(_App), drawDebug(true), drawLevel(true), active_object(NULL), dragged_object(NULL), highlight_active(false) {}
    PlayMode(const PlayMode& orig) = delete;
    PlayMode operator=(const PlayMode& orig) = delete;
    virtual ~PlayMode(){
		active_object=NULL;
		dragged_object=NULL;
	}
    GameState::StateSelect run();
    void set_simulate(); // Sets the Box2D Simulation on/off.
    void set_drawdebug(); // Sets the Box2D Debug drawing on/off.
    void set_drawlevel(); // Sets level object drawing on/off. 
private:
    bool simulate;
    void drawStuff();
    void handleKeyPress(sf::Event event);
    LevelData level;
    bool drawDebug;
    bool drawLevel;

	GameObject* active_object; //NULL denotes nothing is active.
	GameObject* dragged_object; //NULL denotes nothing is being dragged.
	bool highlight_active; //True when we are resizing, etc.
};

#endif	/* PLAYMODE_H */

