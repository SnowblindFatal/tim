/* 
 * File:   EditMode.h
 * Author: js
 *
 * Created on October 28, 2013, 9:29 PM
 */

#ifndef EDITMODE_H
#define	EDITMODE_H
#include "../general/Settings.h"

#include "PlayingField.h"
#include <SFML/Graphics.hpp>
#include "Level.h"
#include <TGUI/TGUI.hpp>
#include <vector>


class EditMode : public PlayingField {
public:
    EditMode(sf::RenderWindow& _App) : PlayingField(_App), drawLevel(true), drawDebug(false), gui(_App), gui_loaded(false), level(_App),locked_available(false),locked_save(false), locked_goals(false), active_object(NULL), dragged_object(NULL), highlight_active(false), running_id(1), wincondition_active(false) {}
    EditMode(const EditMode& orig) = delete;
    EditMode operator=(const EditMode& orig) = delete;
    virtual ~EditMode(){}
    GameState::StateSelect run();
private:
	
	bool drawLevel;
	bool drawDebug;
    void drawStuff();
    void handleKeyPress(sf::Event event);
    tgui::Gui gui;
    void load_gui();
    bool gui_loaded;
    LevelData level;
	std::vector<std::string> object_names;
	//Available:
	bool locked_available;
	void available_procedure();
	void update_available_procedure();
	void close_available();

	//Save:
	bool locked_save;
	void save_procedure();
	void close_save();
	void commit_save();

	//Goals:
	bool locked_goals;
	void goals_procedure();
	void close_goals();
	
	bool not_locked();
	void set_drawdebug();
	void set_drawlevel();
	GameObject* active_object;
	GameObject* dragged_object;
	bool highlight_active;
	size_t running_id;
	bool wincondition_active;
	void update_gui();
};


#endif	/* EDITMODE_H */

