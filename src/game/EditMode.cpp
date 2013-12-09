
#include "EditMode.h"
#include "GameState.h"
#include <iostream>
#include <sstream>
#include <TGUI/TGUI.hpp>
#include "Level.h"
#include "WinCondition.h"
#include "FileHandler.h"

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
				//Reset the bottom bar:
				if (event.type==sf::Event::MouseButtonPressed && event.mouseButton.button== sf::Mouse::Left) {
					tgui::Label::Ptr bottom=gui.get("bottombar");
					bottom->setText(" ");
				}
				continue;
			}
			if (!not_locked()) continue; 
			
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
				else if (wincondition_active) {
					level.winconditionDelta(sf::Mouse::getPosition(App), active_object);
				}
					
			}
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				//Reset the bottom bar:
				tgui::Label::Ptr bottom=gui.get("bottombar");
				bottom->setText(" ");

				if (active_object && active_object->highlightPoint(sf::Mouse::getPosition(App))) {
                    highlight_active=true;
                    active_object->setManipulationStartLocation(sf::Mouse::getPosition(App));
				}
				else if (active_object && level.winconditionPoint(sf::Mouse::getPosition(App), active_object)) {
					wincondition_active=true;
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
						level.deleteWincondition(active_object);
						level.deleteLevelObject(active_object);
						active_object=NULL;
					}
					highlight_active=false;
				}
				else if (wincondition_active) {
					std::string action = level.winconditionClicked(sf::Mouse::getPosition(App), active_object);
					if (action=="delete") {
						level.deleteWincondition(active_object);
						active_object->setID(0);
					}
					wincondition_active=false;
				}
			}
		}
		//TGUI Callbacks:
		tgui::Callback callback;
		while (gui.pollCallback(callback)) {

			if (callback.id==100 && not_locked()) {
				save_procedure();
			}
			
			else if (callback.id == 101 && not_locked()) {
				available_procedure();
			}
			else if (callback.id == 102 && not_locked()) {
				goals_procedure();
			}
			else if (callback.id == 103 && not_locked()) {
				done=true;
				retval = GameState::StateSelect::Menu;
			}


			else if (locked_available) {
				if (callback.id > 199 && callback.id <299) {
					if (level.get_available()[object_names[callback.id-200]] > 0) { 
						level.get_available()[object_names[callback.id-200]]--;
					}
				}
				else if (callback.id >299 && callback.id <399) {
					level.get_available()[object_names[callback.id-300]]++;
				}
				else if (callback.id==120) {
					close_available();
				}
			}	
			else if (locked_goals) {
				if (callback.id==140) {
					close_goals();
				}
					
				else if (callback.id==141) {
					active_object->setID(running_id++);
					IsNearPoint* cond_ptr= new IsNearPoint(active_object, active_object->getCurrentPos().x+5, active_object->getCurrentPos().y);
					level.addWinCondition(cond_ptr);
					close_goals();
				}
				else if (callback.id==142) {
					active_object->setID(running_id++);
					IsNotNearPoint* cond_ptr= new IsNotNearPoint(active_object, active_object->getCurrentPos().x+5, active_object->getCurrentPos().y);
					level.addWinCondition(cond_ptr);
					close_goals();
				}
				else if (callback.id==143) {
					active_object->setID(running_id++);
					IsDestroyed* cond_ptr=new IsDestroyed(active_object);
					level.addWinCondition(cond_ptr);
					close_goals();
				}
				else if (callback.id==144) {
					active_object->setID(running_id++);
					IsUntouched* cond_ptr=new IsUntouched(active_object);
					level.addWinCondition(cond_ptr);
					close_goals();
				}

			}
			else if (locked_save) {
				if (callback.id==160) {
					commit_save();
				}
				else if (callback.id==161) {
					close_save();
				}
			}
					
			//One of the GameObject buttons was pressed:
			else if (dragged_object==NULL && not_locked()) {
					active_object = level.createObject(object_names[callback.id], 0.1f*(float)sf::Mouse::getPosition(App).x, 0.1f*(float)sf::Mouse::getPosition(App).y, true);
					dragged_object = active_object;
			}	
		}
				
		//If not locked, take care of highlighting
		if (not_locked()) {
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
		if (locked_available) {
			update_available_procedure();
		}
		level.drawWincondition(active_object);
		update_gui();

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

bool EditMode::not_locked() {
	if (!locked_available && !locked_goals && !locked_save) return true;
	return false;
}

void EditMode::available_procedure() {
	locked_available=true;
	
	//The Background:
	tgui::Panel::Ptr box(gui, "availablebox");
	box->setBackgroundTexture(Resources::getInstance().getTexture("savebox.png"));
	box->setSize(500,500);
	box->setPosition(50, 50);

	//The Done Button:
	tgui::Button::Ptr done(gui, "availabledone");
	done->load("TGUI/Black.conf");
	done->setSize(50, 25);
	done->setPosition(400,400);
	done->setText("Done");
	done->setCallbackId(120);
	done->bindCallback(tgui::Button::LeftMouseClicked);

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
void EditMode::update_available_procedure() {
	for (auto name : object_names) {
		tgui::Label::Ptr number = gui.get(name+"_available");
		std::stringstream ss;
		ss << level.get_available()[name];
		number->setText(ss.str());
	}

}

void EditMode::close_available() {
	gui.remove(gui.get("availablebox"));
	gui.remove(gui.get("availabledone"));
	for (auto name : object_names) {
		gui.remove(gui.get(name+"_lbl"));
		gui.remove(gui.get(name+"_plus"));
		gui.remove(gui.get(name+"_minus"));
		gui.remove(gui.get(name+"_available"));
	}
	locked_available=false;
}

void EditMode::goals_procedure() {
	locked_goals=true;
	//The background:
	tgui::Panel::Ptr box(gui, "goalsbox");
	box->setBackgroundTexture(Resources::getInstance().getTexture("savebox.png"));
	box->setSize(250, 250);
	box->setPosition(100,100);

	//The Cancel:
	tgui::Button::Ptr cancel(gui, "cancelgoals");
	cancel->load("TGUI/Black.conf");
	cancel->setSize(150,30);
	cancel->setPosition(150, 300);
	cancel->setText("Cancel");
	cancel->setCallbackId(140);
	cancel->bindCallback(tgui::Button::LeftMouseClicked);
	
	//The buttons::
	if (active_object!=NULL && (active_object->getName()=="Domino" || active_object->getName()=="BigBall" || active_object->getName()=="BouncingBall" || active_object->getName()=="BowlingBall")) {
		tgui::Button::Ptr isnear(gui, "isnearbutton");
		isnear->load("TGUI/Black.conf");
		isnear->setSize(150, 30);
		isnear->setPosition(120,120);
		isnear->setText("Is Near Point");
		isnear->setCallbackId(141);
		isnear->bindCallback(tgui::Button::LeftMouseClicked);

		tgui::Button::Ptr isfar(gui, "isfarbutton");
		isfar->load("TGUI/Black.conf");
		isfar->setSize(150, 30);
		isfar->setPosition(120,155);
		isfar->setText("Is Not Near Point");
		isfar->setCallbackId(142);
		isfar->bindCallback(tgui::Button::LeftMouseClicked);

	}
	if (active_object!=NULL && (active_object->getName()=="Bomb") ) {
		tgui::Button::Ptr destroyed(gui, "destroyed");
		destroyed->load("TGUI/Black.conf");
		destroyed->setSize(150, 30);
		destroyed->setPosition(120,190);
		destroyed->setText("Destroyed");
		destroyed->setCallbackId(143);
		destroyed->bindCallback(tgui::Button::LeftMouseClicked);
		
		tgui::Button::Ptr untouched(gui, "untouched");
		untouched->load("TGUI/Black.conf");
		untouched->setSize(150, 30);
		untouched->setPosition(120,225);
		untouched->setText("Untouched");
		untouched->setCallbackId(144);
		untouched->bindCallback(tgui::Button::LeftMouseClicked);
	}
}

void EditMode::close_goals() {
	gui.remove(gui.get("goalsbox"));
	gui.remove(gui.get("cancelgoals"));
	if (gui.get("isnearbutton") != NULL) {
		gui.remove(gui.get("isnearbutton"));
	}
	if (gui.get("isfarbutton") !=NULL) {
		gui.remove(gui.get("isfarbutton"));
	}
	if (gui.get("destroyed") != NULL) {
		gui.remove(gui.get("destroyed"));
	}
	if (gui.get("untouched") != NULL) {
		gui.remove(gui.get("untouched"));
	}
	locked_goals=false;
}

void EditMode::save_procedure() {
	locked_save=true;

	//The Background:
	tgui::Panel::Ptr box(gui, "savebox");
	box->setBackgroundTexture(Resources::getInstance().getTexture("savebox.png"));
	box->setSize(500,500);
	box->setPosition(50,50);
	
	//The name:
	tgui::EditBox::Ptr name(gui, "savename");
	name->load("TGUI/Black.conf");
	name->setSize(400, 30);
	name->setPosition(100,100);
	name->setText("Name");

	//The Description:
	tgui::TextBox::Ptr description(gui, "description");
	description->load("TGUI/Black.conf");
	description->setSize(400,200);
	description->setPosition(100,200);
	description->setTextSize(22);
	description->setText("Write a\ndescription of\nthe level\nhere");	

	//The save and cancel buttons:
	tgui::Button::Ptr savebutton(gui, "commitsave");
	savebutton->load("TGUI/Black.conf");
	savebutton->setSize(150, 30);
	savebutton->setText("Save");
	savebutton->setPosition(100,450);
	savebutton->setCallbackId(160); 
	savebutton->bindCallback(tgui::Button::LeftMouseClicked);

	tgui::Button::Ptr cancel(gui, "cancelsave");
	cancel->load("TGUI/Black.conf");
	cancel->setSize(150, 30);
	cancel->setText("Cancel");
	cancel->setPosition(350,450);
	cancel->setCallbackId(161); 
	cancel->bindCallback(tgui::Button::LeftMouseClicked);
	
}

void EditMode::close_save() {
	gui.remove(gui.get("savebox"));
	gui.remove(gui.get("savename"));
	gui.remove(gui.get("description"));
	gui.remove(gui.get("commitsave"));
	gui.remove(gui.get("cancelsave"));
	locked_save=false;
}

void EditMode::commit_save() {
	tgui::EditBox::Ptr name = gui.get("savename");
	tgui::TextBox::Ptr description = gui.get("description");
	level.setDescription(description->getText());
	FileHandler fh("res/leveldata/"+name->getText()+".txt");
	tgui::Label::Ptr bottom=gui.get("bottombar");
	if (!fh.saveLevel(level)) {
		bottom->setText("Save failed: \n"+fh.getError());
	}
	else {
		Resources::getInstance().addLevel(name->getText()+".txt");
		bottom->setText("Save succesful!");
	}
	close_save();
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
	tgui::Button::Ptr save(gui, "save");
	save->load("TGUI/Black.conf");
	save->setSize(150, 30);
	save->setText("Save");
	save->setPosition(625,7);
	save->setCallbackId(100); //This assumes we won't surpass 100 GameObjects. We won't.
	save->bindCallback(tgui::Button::LeftMouseClicked);
	
	
	//The Available button:
	tgui::Button::Ptr button(gui, "available");
	button->load("TGUI/Black.conf");
	button->setSize(150, 30);
	button->setText("Available");
	button->setPosition(625,42);
	button->setCallbackId(101); 
	button->bindCallback(tgui::Button::LeftMouseClicked);

	//The Add goal button:
	tgui::Button::Ptr addgoal(gui, "addgoal");
	addgoal->load("TGUI/Black.conf");
	addgoal->setSize(150, 30);
	addgoal->setText("Add goal");
	addgoal->setPosition(625,77);
	addgoal->setCallbackId(102); 
	addgoal->bindCallback(tgui::Button::LeftMouseClicked);
	
	//The Main menu button:
	tgui::Button::Ptr mainmenu(gui, "mainmenu");
	mainmenu->load("TGUI/Black.conf");
	mainmenu->setSize(150, 30);
	mainmenu->setText("Main menu");
	mainmenu->setPosition(625,112);
	mainmenu->setCallbackId(103);
	mainmenu->bindCallback(tgui::Button::LeftMouseClicked);

	//The Bottombar:
	tgui::Label::Ptr bottom(gui, "bottombar");
	bottom->setSize(600,100);
	bottom->setPosition(0,500);
	bottom->setBackgroundColor(sf::Color::White);
	bottom->setTextColor(sf::Color::Black);
	bottom->setTextSize(22);
	bottom->setText(" ");
	

	
	
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

void EditMode::update_gui() {
	tgui::Button::Ptr addgoal = gui.get("addgoal");
	if (level.canCreateWincondition(active_object)) {
		addgoal->show();
	}
	else {
		addgoal->hide();
	}
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
