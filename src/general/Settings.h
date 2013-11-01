/* 
 * File:   Settings.h
 * Author: js
 *
 * Created on October 29, 2013, 5:37 PM
 */

#ifndef SETTINGS_H
#define	SETTINGS_H

#include <string>

class Settings {
public:
    Settings(std::string filePath);
    Settings(const Settings& orig) = delete;
    Settings operator=(const Settings& orig) = delete;
    ~Settings(){}
    
    unsigned int b2_to_sfml; // a conversion ratio, you have to multiply b2 coordinates
    			     // by this amount to get pixel coordinates
    
    
private:

};

#endif	/* SETTINGS_H */

