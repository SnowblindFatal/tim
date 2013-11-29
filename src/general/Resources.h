/* 
 * File:   Resources.h
 * Author: js
 *
 * Created on October 29, 2013, 12:04 AM
 */



#ifndef RESOURCES_H
#define	RESOURCES_H

#include <string>
#include <map>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//All resources, such as graphics and sounds will be loaded into this class at startup.
class Resources {
public:
    static Resources& getInstance(){
        static Resources resources;
        return resources; 
    }
    Resources(const Resources& orig) = delete;
    Resources& operator=(const Resources& orig) = delete;
    ~Resources();
    
    void loadResources();
    const sf::Texture* getTexture(const std::string &fileName) const;
    const sf::SoundBuffer& getSoundBuffer(const std::string &fileName) const;
    sf::Music& getMusic(const std::string &fileName) const;
    const sf::Font& getFont(const std::string &fileName) const;
private:
    Resources(){}

    std::map<std::string, sf::Texture*> textures;
    std::map<std::string, sf::SoundBuffer*> soundBuffers;
    std::map<std::string, sf::Music*> music;
    std::map<std::string, sf::Font*> fonts;

};

#endif	/* RESOURCES_H */

