/* 
 * File:   Settings.cpp
 * Author: js
 * 
 * Created on October 29, 2013, 5:37 PM
 */

#include "Settings.h"
#include <string>
void TIMSettings::loadSettings(std::string filePath)
{
    filePath.size(); //suppress warnings. :)
    
    
    
    
    //!!TODO:
    //Load config data from a file.
    //TODO: figure out how to implement getters nicely.
    //Would be nice if the usage was just: 
    //string name = settings.defaultName;
    //int resX = settings.screenWidth;
    //Gotta make some rules about where the settings can be adjusted since I don't think we can use const here.
}

