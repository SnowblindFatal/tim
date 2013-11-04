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


class EditMode : public PlayingField {
public:
    EditMode(sf::RenderWindow& _App, Resources& res) : PlayingField(_App, res){}
    EditMode(const EditMode& orig) = delete;
    EditMode operator=(const EditMode& orig) = delete;
    virtual ~EditMode(){}
    GameState::StateSelect run();
private:
    void drawStuff();
    void handleKeyPress(sf::Event event);

};

#endif	/* EDITMODE_H */

