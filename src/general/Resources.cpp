/* 
 * File:   Resources.cpp
 * Author: js
 * 
 * Created on October 29, 2013, 12:04 AM
 */

#include "Resources.h"
#include <string>

Resources::Resources(std::string path)
{
    path.size(); //suppress warnings. :)
    
    //!!TODO:     
    //Iterate through the resource folder. Load the resources up in various hashmaps.
    //usage would be then:
    //Sound& sound = resources.sounds["gunshot.ogg"]
    //Texture& tex = resources.textures["brickwall.png"]
    //Remember to only use references! Don't duplicate the resource data!
}
