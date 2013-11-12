/* 
 * File:   PlayMode.cpp
 * Author: js
 * 
 * Created on October 28, 2013, 9:29 PM
 */

#include "PlayMode.h"
#include "GameState.h"
#include <iostream>
GameState::StateSelect PlayMode::run()
{
    done = false;
    if (!level.loaded())
        level.loadlevel();
 
    while (!done)
    {
        sf::Event event;
        while (App.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
            {
                retval = GameState::StateSelect::Exit;
                done = true;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::S) {
                    set_simulate();
                }
                if (event.key.code == sf::Keyboard::D) {
                    set_drawdebug();
                }
                if (event.key.code == sf::Keyboard::L) {
                   set_drawlevel();
                }
				if (event.key.code == sf::Keyboard::R) {
					simulate=0;
					level.reset();
				}
                else handleKeyPress(event);
            }
            
        }
        App.clear();
        if (simulate) {
            level.simulate();
			if (level.checkWin()) {
				set_simulate();
				std::cout << "You win!\n";
			}
		}
        level.draw(drawDebug, drawLevel);
        App.display();
    }
    return retval;
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
