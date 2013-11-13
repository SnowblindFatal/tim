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
			
			// If we are simulating, don't respond to anything else than resetting:
			if (simulate) {
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
					simulate=0;
					level.reset();
				}
				continue;
			}

            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::S) {
                	simulate=1;
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

				//GameObject creation:
				if (event.key.code == sf::Keyboard::Num1) {
					active_object = level.createObject("Platform", 0.1f*(float)sf::Mouse::getPosition(App).x, 0.1f*(float)sf::Mouse::getPosition(App).y);
					dragged_object = active_object;
					if (active_object==NULL)
						std::cout << "No Platforms available\n";
				}
				if (event.key.code == sf::Keyboard::Num2) {
					active_object = level.createObject("Wall", 0.1f*(float)sf::Mouse::getPosition(App).x, 0.1f*(float)sf::Mouse::getPosition(App).y);
					dragged_object = active_object;
					if (active_object==NULL)
						std::cout << "No Walls available\n";
				}
				
				

                else handleKeyPress(event);
            }
           	 
        	else if (event.type == sf::Event::MouseMoved) {
				
				//Move the dragged_object if there is such a thing:
				if (dragged_object!=NULL) {
					dragged_object->move((float)event.mouseMove.x/10.0f, (float)event.mouseMove.y/10.0f);
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				
				//Set active and dragging when pressed over some playerobject and not dragging:
				if (dragged_object==NULL) {
					active_object = level.isInsidePlayerObject(0.1f*(float)sf::Mouse::getPosition(App).x, 0.1f*(float)sf::Mouse::getPosition(App).y);
					dragged_object = active_object;
				}

			}

			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				
				//If we were dragging something, stop.
				if (dragged_object!=NULL) {
					dragged_object=NULL;
				}
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
