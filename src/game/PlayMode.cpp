/* 
 * File:   PlayMode.cpp
 * Author: js
 * 
 * Created on October 28, 2013, 9:29 PM
 */

#include "PlayMode.h"
#include "GameState.h"
#include "FileHandler.h"
#include <iostream>
#include <sstream>

GameState::StateSelect PlayMode::run()
{
    done = false;
	locked_complete=false;	
	
    level.clear();
    currentLevelName = Resources::getInstance().getCurrentLevelName();
    FileHandler handler("res/leveldata/"+currentLevelName);
    handler.loadLevel(level);
 
	if (!gui_loaded)
    {
        load_gui();
    }
    else
    {
    	tgui::Label::Ptr bottombar = gui.get("bottombar");
		bottombar->setText(level.getDescription());
    }
	
	
    while (!done)
    {
        handleInput();
        handleGui();
        highlightObject();

        App.clear();
        
        handleSimulation();
        
        level.draw(active_object, drawDebug, drawLevel);

		update_available();
		gui.draw();
        App.display();
    }
    return retval;
}

void PlayMode::handleInput() {

    sf::Event event;
    while (App.pollEvent(event)) {


        // Close window : exit
        if (event.type == sf::Event::Closed) {
            retval = GameState::StateSelect::Exit;
            done = true;
        }
        //TGUI is given the event first. If TGUI didn't use the event, we will.
        if (gui.handleEvent(event)) {
            highlight_active = false;
            continue;
        }

        // If we are simulating, don't respond to anything else than resetting:
        if (simulate) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                simulate = 0;
                level.reset();
            }
            continue;
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::S && dragged_object == NULL && !highlight_active) {
                simulate = 1;
                active_object = NULL;

            }
            if (event.key.code == sf::Keyboard::D) {
                set_drawdebug();
            }
            if (event.key.code == sf::Keyboard::L) {
                set_drawlevel();
            }
            if (event.key.code == sf::Keyboard::R) {
                simulate = 0;
                level.reset();
            }
			else handleKeyPress(event);
		}
        

        else if (event.type == sf::Event::MouseMoved) {

            //Move the dragged_object if there is such a thing:
            if (dragged_object != NULL) {
                dragged_object->move((float) event.mouseMove.x / 10.0f, (float) event.mouseMove.y / 10.0f);
            } else if (highlight_active) {
                active_object->highlightDelta(sf::Mouse::getPosition(App));
            }

        } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (active_object && active_object->highlightPoint(sf::Mouse::getPosition(App))) {
                highlight_active = true;
                active_object->setManipulationStartLocation(sf::Mouse::getPosition(App));
            }

                //Set active and dragging when pressed over some playerobject and not dragging:
            else if (dragged_object == NULL) {
                active_object = level.isInsidePlayerObject(0.1f * (float) sf::Mouse::getPosition(App).x, 0.1f * (float) sf::Mouse::getPosition(App).y);
                dragged_object = active_object;
            }


        }
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {

            //If we were dragging something, stop if we can place it here.
            if (dragged_object != NULL && dragged_object->can_place) {
                dragged_object = NULL;
            } else if (highlight_active && active_object) {
                std::string action = active_object->highlightClicked(sf::Mouse::getPosition(App));
                if (action == "delete") {
                    level.deletePlayerObject(active_object);
                    active_object = NULL;
                }
                highlight_active = false;
            }
        }
    }
}

void PlayMode::handleGui() {

    //TGUI Callbacks:
    tgui::Callback callback;
    while (gui.pollCallback(callback)) {
		if (locked_complete) {
			if (callback.id == 666) {
				done = true;
				retval = GameState::StateSelect::Menu;
				levelCompleteClose();
				toggleSimulation();
			}
			else if (callback.id == 667) {
				levelCompleteClose();
				toggleSimulation();
			}
			continue;
		}
				

        if (callback.id == 100) {
            toggleSimulation();
        }
        else if (callback.id == 101) {
            done = true;
            retval = GameState::StateSelect::Menu;
        }
            //One of the GameObject buttons was pressed:
        else if (dragged_object == NULL && !simulate) {
            active_object = level.createObject(object_names[callback.id], 0.1f * (float) sf::Mouse::getPosition(App).x, 0.1f * (float) sf::Mouse::getPosition(App).y);
            dragged_object = active_object;
        }
    }
}

void PlayMode::highlightObject() {

    //If not simulating, take care of highlighting
    if (!simulate) {
        //Notify whomever we are hovering over, if not dragging or resizing, etc.
        if (dragged_object == NULL && !highlight_active) {
            GameObject* hover = level.isInsidePlayerObject(0.1f * (float) sf::Mouse::getPosition(App).x, 0.1f * (float) sf::Mouse::getPosition(App).y);
            if (hover != NULL) {
                hover->setHighlight("hover");
            }
        }
        //Notify the active/dragged object about highlighting
        if (dragged_object != NULL) {
            dragged_object->setHighlight("dragged");
        } else if (active_object != NULL) {
            active_object->setHighlight("active");
        }
    }
}

void PlayMode::toggleSimulation() {
	if (!simulate) {
		level.reset();
		simulate=1;
		active_object=NULL;
		tgui::Button::Ptr button=gui.get("simulate");
		button->setText("Reset");
        button = gui.get("main menu");
        button->setProperty("Enabled", "false");
        button->setTransparency(127);
        
        tgui::Label::Ptr bottombar = gui.get("bottombar");
		bottombar->setText(level.getDescription());
	}
	else {
		simulate=0;
		level.reset();
		tgui::Button::Ptr button=gui.get("simulate");
		button->setText("Simulate");
        button = gui.get("main menu");
        button->setProperty("Enabled", "true");
        button->setTransparency(255);
	}
}

void PlayMode::handleSimulation() {

    if (simulate) {

        level.simulate();

        if (!locked_complete && level.checkWin()) {
            //toggleSimulation();
            Resources::getInstance().winLevel(currentLevelName);
            tgui::Label::Ptr bottombar = gui.get("bottombar");
            bottombar->setText("Level completed!");
			levelCompleteDialog();
        }
    }
}

void PlayMode::set_simulate() {
    simulate=simulate?0:1;
}
void PlayMode::set_drawdebug() {
    drawDebug=drawDebug?0:1;
}

void PlayMode::set_drawlevel() {
	drawLevel=drawLevel?0:1;
}
void PlayMode::levelCompleteDialog() {
	locked_complete=true; 

	//The Background: 
	tgui::Panel::Ptr box(gui, "completebg");
	box->setBackgroundTexture(Resources::getInstance().getTexture("savebox.png"));
	box->setSize(500,300);
	box->setPosition(50, 50);
	box->setTransparency(180);
	box->setBackgroundColor(sf::Color::Transparent);

	//The congratulation:
	tgui::Panel::Ptr yeah(gui, "congratulations");
	yeah->setBackgroundTexture(Resources::getInstance().getTexture("grats2.png"));
	yeah->setSize(288,33);
	yeah->setPosition(156, 100);
	yeah->setBackgroundColor(sf::Color::Transparent);


	//The Menu Button:
	tgui::Button::Ptr done(gui, "completeExit");
	done->load("TGUI/Black.conf");
	done->setSize(150, 50);
	done->setPosition(100,200);
	done->setText("Menu");
	done->setCallbackId(666);
	done->bindCallback(tgui::Button::LeftMouseClicked);

	//Reset Button:
	tgui::Button::Ptr reset(gui, "completeReset");
	reset->load("TGUI/Black.conf");
	reset->setSize(150, 50);
	reset->setPosition(350,200);
	reset->setText("Reset");
	reset->setCallbackId(667);
	reset->bindCallback(tgui::Button::LeftMouseClicked);
}

void PlayMode::levelCompleteClose() {
	gui.remove(gui.get("completebg"));
	gui.remove(gui.get("congratulations"));
	gui.remove(gui.get("completeExit"));
	gui.remove(gui.get("completeReset"));
	locked_complete=false;
}



	

void PlayMode::load_gui() {
	//Globals:
	gui.setGlobalFont(Resources::getInstance().getFont("BorisBlackBloxx.ttf"));

	//The Background:
	tgui::Panel::Ptr bar(gui, "bg"); 
	bar->setBackgroundTexture(Resources::getInstance().getTexture("Sidebar.png"));
	bar->setSize(200, 600);
	bar->setPosition(600,0);

	//The Play/Reset button:
	tgui::Button::Ptr button_game(gui, "simulate");
	button_game->load("TGUI/Black.conf");
	button_game->setSize(150, 50);
	button_game->setText("Simulate");
	button_game->setPosition(625,10);
	button_game->setCallbackId(100); //This assumes we won't surpass 100 GameObjects. We won't.
	button_game->bindCallback(tgui::Button::LeftMouseClicked);

    //The Return to menu button:
    tgui::Button::Ptr button_menu(gui, "main menu");
    button_menu->load("TGUI/Black.conf");
    button_menu->setSize(150, 50);
    button_menu->setText("main menu");
    button_menu->setPosition(625, 80);
    button_menu->setCallbackId(101);
    button_menu->bindCallback(tgui::Button::LeftMouseClicked);

	//The Bottombar:
	tgui::Label::Ptr bottom(gui, "bottombar");
	bottom->setSize(600,100);
	bottom->setPosition(0,500);
	bottom->setBackgroundColor(sf::Color::White);
	bottom->setTextColor(sf::Color::Black);
	bottom->setTextSize(22);
	bottom->setText(level.getDescription());

	//The different gameobjects:
	int index=0;
	for (auto pair : level.get_available()) {
		tgui::Button::Ptr obj_button(gui, pair.first);
		obj_button->load("TGUI/Black.conf");
		obj_button->setSize(150, 25);
		obj_button->setText(pair.first);
		obj_button->setPosition(625,180+index*30);
		obj_button->setCallbackId(index);
		obj_button->bindCallback(tgui::Button::LeftMouseClicked);
		//So that object_names[index] corresponds to correct object:
		object_names.push_back(pair.first);
		index++;
	}

	gui_loaded=true;

}

void PlayMode::update_available() {
	auto available = level.get_available();
	for (auto& pair : available) {
		tgui::Button::Ptr button = gui.get(pair.first);
		if (pair.second==0) {
			button->hide();
		}
		else {
			button->show();
			std::stringstream ss;
			ss << pair.first << ": " << pair.second;
			button->setText(ss.str());
		}
	}
}
		
	
	

void PlayMode::handleKeyPress(sf::Event event)
{
    //More complex stuff will go to their own methods.
    if (event.key.code == sf::Keyboard::Escape)
    {
        retval = GameState::StateSelect::Exit;
        done = true;
    }
    else if (event.key.code == sf::Keyboard::Return)
    {
        retval = GameState::StateSelect::Edit;
        done = true;
    }
    else if (event.key.code == sf::Keyboard::Space)
    {
        retval = GameState::StateSelect::Menu;
        done = true;
    }
}
