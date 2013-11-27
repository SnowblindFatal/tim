/* 
 * File:   Resources.cpp
 * Author: js
 * 
 * Created on October 29, 2013, 12:04 AM
 */

#include "Resources.h"
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>
namespace {
    //The file name lists are static like this since otherwise we would either 
    //require the usage of the boost library or write platform-dependant code.
    const std::vector<std::string> textureFileNames = {
        "brick1.jpg",
        "whitebrick.jpg",
        "lowbrick.png"
    };

    const std::vector<std::string> soundFileNames = {
        "brick.wav",
        "soccer.wav"
    };

    const std::vector<std::string> fontFileNames = {
        "BLASTER.TTF",
        "BorisBlackBloxx.ttf"
    };

    const std::vector<std::string> musicFileNames = {
        "MAP01.ogg"
    };
}

Resources::~Resources()
{
    for (auto pair : textures)
    {
        delete pair.second;
    }
    for (auto pair : soundBuffers) 
    {
        delete pair.second;
    }
    for (auto pair : music) 
    {
        delete pair.second;
    }
    for (auto pair : fonts) 
    {
        delete pair.second;
    }
}

void Resources::loadResources()
{
    for (const std::string& fileName : textureFileNames)
    {
        sf::Texture* texturePtr = new sf::Texture;
        textures[fileName] = texturePtr;
        if (!texturePtr->loadFromFile("res/textures/" + fileName)) 
        {
            throw "Did not find texture file: " + fileName;
        }
    }
    
    for (const std::string& fileName : soundFileNames) 
    {
        sf::SoundBuffer* soundBufferPtr = new sf::SoundBuffer;
        soundBuffers[fileName] = soundBufferPtr;
        if (!soundBufferPtr->loadFromFile("res/sounds/" + fileName)) 
        {
            throw "Did not find sound file: " + fileName;
        }
    }
    
    for (const std::string& fileName : fontFileNames) 
    {
        sf::Font* fontPtr = new sf::Font;
        fonts[fileName] = fontPtr;
        if (!fontPtr->loadFromFile("res/fonts/" + fileName)) 
        {
            throw "Did not find font file: " + fileName;
        }
    }
    
    for (const std::string& fileName : musicFileNames) 
    {
        sf::Music* musicPtr = new sf::Music;
        music[fileName] = musicPtr;
        if (!musicPtr->openFromFile("res/music/" + fileName)) 
        {
            throw "Did not find music file: " + fileName;
        }
    }
}


const sf::Texture& Resources::getTexture(const std::string &fileName) const
{
    return *textures.at(fileName);
}

const sf::SoundBuffer& Resources::getSoundBuffer(const std::string &fileName) const
{
    return *soundBuffers.at(fileName);
}

sf::Music& Resources::getMusic(const std::string &fileName) const 
{
    return *music.at(fileName);
}

const sf::Font& Resources::getFont(const std::string &fileName) const
{
    return *fonts.at(fileName);
}