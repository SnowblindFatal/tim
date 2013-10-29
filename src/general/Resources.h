/* 
 * File:   Resources.h
 * Author: js
 *
 * Created on October 29, 2013, 12:04 AM
 */



#ifndef RESOURCES_H
#define	RESOURCES_H

#include <string>

//All resources, such as graphics and sounds will be loaded into this class at startup.
class Resources {
public:
    Resources(std::string path);
    Resources(const Resources& orig) = delete;
    Resources& operator=(const Resources& orig) = delete;
    ~Resources(){}
private:

};

#endif	/* RESOURCES_H */

