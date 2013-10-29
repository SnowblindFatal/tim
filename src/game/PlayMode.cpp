/* 
 * File:   PlayMode.cpp
 * Author: js
 * 
 * Created on October 28, 2013, 9:29 PM
 */

#include "PlayMode.h"
#include "GameState.h"
GameState::StateSelect PlayMode::run()
{
    return GameState::StateSelect::Menu;
}

