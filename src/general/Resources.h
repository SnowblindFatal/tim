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
    sf::Texture* getTexture(const std::string &fileName) const;
    const sf::SoundBuffer& getSoundBuffer(const std::string &fileName) const;
    sf::Music& getMusic(const std::string &fileName) const;
    const sf::Font getFont(const std::string &fileName) const;
    void winLevel(const std::string levelname);
    void saveWinStatus();
    void setCurrentLevel(const std::string& fileName);
    const std::string& getCurrentLevelName() const;
    const std::map<std::string, bool>& getLevelInfo() const;
    void addLevel(std::string levelName);
    
private:
    Resources(){}

    void loadTextures();
    void loadSounds();
    void loadMusic();
    void loadFonts();
    void loadLevelInfo();
    std::map<std::string, sf::Texture*> textures;
    std::map<std::string, sf::SoundBuffer*> soundBuffers;
    std::map<std::string, sf::Music*> music;
    std::map<std::string, sf::Font*> fonts;
    std::map<std::string, bool> levelInfo;
    std::string currentLevel;
};

#endif	/* RESOURCES_H */

