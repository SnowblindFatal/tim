/* 
 * File:   EditMode.cpp
 * Author: js
 * 
 * Created on October 28, 2013, 9:29 PM
 */

#include "EditMode.h"
#include "GameState.h"

GameState::StateSelect EditMode::run()
{
    return GameState::StateSelect::Menu;
}