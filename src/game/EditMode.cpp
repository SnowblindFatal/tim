
#include "EditMode.h"
#include "GameState.h"
#include <iostream>
#include <sstream>


GameState::StateSelect EditMode::run()
{
    done = false;
 
	if (!gui_loaded)
		load_gui();
	
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
			//TGUI is given the event first. If TGUI didn't use the event, we will.
			if (gui.handleEvent(event)) {
				continue;
			}
			if (locked) continue;
			
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::D) {
                    set_drawdebug();
                }
                if (event.key.code == sf::Keyboard::L) {
                   set_drawlevel();
                }

			}	
           	 
        	else if (event.type == sf::Event::MouseMoved) {
				
				//Move the dragged_object if there is such a thing:
				if (dragged_object!=NULL) {
					dragged_object->move((float)event.mouseMove.x/10.0f, (float)event.mouseMove.y/10.0f);
				}
				else if (highlight_active) {
					active_object->highlightDelta(sf::Mouse::getPosition(App));
				}
					
			}
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				if (active_object && active_object->highlightPoint(sf::Mouse::getPosition(App))) {
                    highlight_active=true;
                    active_object->setManipulationStartLocation(sf::Mouse::getPosition(App));
				}


				//Set active and dragging when pressed over some Levelobject and not dragging:
				else if (dragged_object==NULL) {
					active_object = level.isInsideLevelObject(0.1f*(float)sf::Mouse::getPosition(App).x, 0.1f*(float)sf::Mouse::getPosition(App).y);
					dragged_object = active_object;
				}
				

			}

			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				
				//If we were dragging something, stop if we can place it here.
				if (dragged_object!=NULL && dragged_object->can_place) {
					dragged_object=NULL;
				}
				else if (highlight_active) {
					std::string action = active_object->highlightClicked(sf::Mouse::getPosition(App));
					if (action=="delete") {
						level.deleteLevelObject(active_object);
						active_object=NULL;
					}
					highlight_active=false;
				}
			}
		}
		//TGUI Callbacks:
		tgui::Callback callback;
		while (gui.pollCallback(callback)) {
			
			if (callback.id == 100 && !locked) {
				save_procedure();
			}

			//One of the GameObject buttons was pressed:
			else if (dragged_object==NULL && !locked) {
					active_object = level.createObject(object_names[callback.id], 0.1f*(float)sf::Mouse::getPosition(App).x, 0.1f*(float)sf::Mouse::getPosition(App).y, true);
					dragged_object = active_object;
			}	

			if (locked) {
				if (callback.id > 199 && callback.id <299) {
					if (level.get_available()[object_names[callback.id-200]] > 0) { 
						level.get_available()[object_names[callback.id-200]]--;
					}
				}
				else if (callback.id >299 && callback.id <399) {
					level.get_available()[object_names[callback.id-300]]++;
				}
			}	
		}
				
		//If not locked, take care of highlighting
		if (!locked) {
			//Notify whomever we are hovering over, if not dragging or resizing, etc.
            if (dragged_object==NULL && !highlight_active) {
                GameObject* hover = level.isInsideLevelObject(0.1f*(float)sf::Mouse::getPosition(App).x, 0.1f*(float)sf::Mouse::getPosition(App).y);
                if (hover!=NULL) {
                    hover->setHighlight("hover");
                }
            }
			//Notify the active/dragged object about highlighting
			if (dragged_object!=NULL) {
				dragged_object->setHighlight("dragged");
			}
			else if (active_object!=NULL) {
				active_object->setHighlight("active");
			}
		}



        App.clear();
        level.draw(active_object, drawDebug, drawLevel);
		if (locked) {
			update_save_procedure();
		}

		gui.draw();
        App.display();
    }
    return retval;
}

void EditMode::set_drawdebug() {
    drawDebug=drawDebug?0:1;
}

void EditMode::set_drawlevel() {
	drawLevel=drawLevel?0:1;
}

void EditMode::save_procedure() {
	locked=true;
	std::cout << "Save!\n";
	
	//The Background:
	tgui::Panel::Ptr box(gui, "savebox");
	box->setBackgroundTexture(Resources::getInstance().getTexture("savebox.png"));
	box->setSize(500,500);
	box->setPosition(50, 50);

	//The gameobjects:
	int index=0;
	for (auto name : object_names) {
		tgui::Label::Ptr lbl(gui, (name+"_lbl"));
		lbl->setSize(170,25);
		lbl->setPosition(180,100+index*30);
		lbl->setText(name);
		lbl->setTextSize(20);
		lbl->setTextColor(sf::Color(255,165,0));
		
		tgui::Button::Ptr plus(gui, name+"_plus");
		plus->load("TGUI/Black.conf");
		plus->setSize(10,20);
		plus->setPosition(140,100+index*30);
		plus->setText("+");
		plus->setCallbackId(300 +index);
		plus->bindCallback(tgui::Button::LeftMouseClicked);

		
		tgui::Button::Ptr minus(gui, name+"_minus");
		minus->load("TGUI/Black.conf");
		minus->setSize(10,20);
		minus->setPosition(100,100+index*30);
		minus->setText("-");
		minus->setCallbackId(200 +index);
		minus->bindCallback(tgui::Button::LeftMouseClicked);

		tgui::Label::Ptr number(gui, (name+"_available"));
		number->setSize(30,20);
		number->setPosition(65, 100+index*30);
		std::stringstream ss;
		ss << level.get_available()[name];
		number->setText(ss.str());
		number->setTextColor(sf::Color::Black);
		number->setTextSize(20);
		number->setBackgroundColor(sf::Color::White);
		
		index++;
	}
}
void EditMode::update_save_procedure() {
	for (auto name : object_names) {
		tgui::Label::Ptr number = gui.get(name+"_available");
		std::stringstream ss;
		ss << level.get_available()[name];
		number->setText(ss.str());
	}

}

void EditMode::load_gui() {
	//Globals:
	gui.setGlobalFont(Resources::getInstance().getFont("BorisBlackBloxx.ttf"));

	//The Background:
	tgui::Panel::Ptr bar(gui, "bg"); 
	bar->setBackgroundTexture(Resources::getInstance().getTexture("Sidebar.png"));
	bar->setSize(200, 600);
	bar->setPosition(600,0);
	
	
	//The Save button:
	tgui::Button::Ptr button(gui, "save");
	button->load("TGUI/Black.conf");
	button->setSize(150, 50);
	button->setText("Save");
	button->setPosition(625,10);
	button->setCallbackId(100); //This assumes we won't surpass 100 GameObjects. We won't.
	button->bindCallback(tgui::Button::LeftMouseClicked);
	

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

		
	
	

void EditMode::handleKeyPress(sf::Event event)
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
