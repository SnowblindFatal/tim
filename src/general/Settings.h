/* 
 * File:   Settings.h
 * Author: js
 *
 * Created on October 29, 2013, 5:37 PM
 */

#ifndef SETTINGS_H
#define	SETTINGS_H

#include <string>
#include <cstddef>
/*
This implementation seems to me the most logical.
For each parameter you need in code, just use: TIMSettings::parameter
Each file should probably include this.

Currently, however, for some reason this does not work!
*/


namespace Settings {
    /*
     Pixels per meter.
     Multiply b2 units by this to get SFML pixels.
     */
    //size_t PPM=10;

    
    


    //loadSettings from a file:
    void loadSettings(std::string filename);

    
}

#endif	/* SETTINGS_H */

