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
#include <TGUI/TGUI.hpp>
#include <vector>

class PlayMode : public PlayingField {
public:
    PlayMode(sf::RenderWindow& _App) : PlayingField(_App), simulate(false), locked_complete(false),level(_App), drawDebug(false), drawLevel(true), active_object(NULL), dragged_object(NULL), highlight_active(false), gui(_App), gui_loaded(false), currentLevelName("") {}
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
    void toggleSimulation();
    void handleInput();
    void handleGui();
    void highlightObject();
    void handleSimulation();
	void levelCompleteDialog();
	void levelCompleteClose();
	bool locked_complete;

    LevelData level;
    bool drawDebug;
    bool drawLevel;

	GameObject* active_object; //NULL denotes nothing is active.
	GameObject* dragged_object; //NULL denotes nothing is being dragged.
	bool highlight_active; //True when we are resizing, etc.

	tgui::Gui gui;
	bool gui_loaded;
	void load_gui();
	void update_available();
	std::vector<std::string> object_names; //Used by TGUI.
    std::string currentLevelName;
};

#endif	/* PLAYMODE_H */

