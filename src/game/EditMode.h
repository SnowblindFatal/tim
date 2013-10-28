/* 
 * File:   EditMode.h
 * Author: js
 *
 * Created on October 28, 2013, 9:29 PM
 */

#ifndef EDITMODE_H
#define	EDITMODE_H

#include "PlayingField.h"


class EditMode : public PlayingField {
public:
    EditMode(sf::RenderWindow& window) : PlayingField(window){}
    EditMode(const EditMode& orig) = delete;
    EditMode operator=(const EditMode& orig) = delete;
    virtual ~EditMode(){}
    void run();
private:

};

#endif	/* EDITMODE_H */

