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
        "lowbrick.png",
		"tnt.jpg",
		"small_tnt.jpg",
		"gravitybutton.png",
		"gravitybox.png",
		"bowlingball.png",
		"small_ball.png",
		"plank.jpg",
		"block.jpg",
		"domino.jpg",
        "magic.jpg"
    };

    const std::vector<std::string> soundFileNames = {
        "brick.wav",
        "soccer.wav",
        "default.wav"
    };

    const std::vector<std::string> fontFileNames = {
        "BLASTER.TTF",
        "BorisBlackBloxx.ttf"
    };

    const std::vector<std::string> musicFileNames = {
        "MAP01.ogg",
        "default.ogg"
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
        texturePtr->setRepeated(true);
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


const sf::Texture* Resources::getTexture(const std::string &fileName) const
{
    sf::Texture* tex = NULL;
    try {
        tex = textures.at(fileName);
    }    catch (const std::out_of_range& oor) {
        std::cout << "did not find a texture resource. Here's hopefully some info:\n" << oor.what() << "\n";
        return textures.at("magic.jpg");
    }
    return tex;
}

const sf::SoundBuffer& Resources::getSoundBuffer(const std::string &fileName) const {
    sf::SoundBuffer* soundBuffer = NULL;
    try {
        soundBuffer = soundBuffers.at(fileName);
    } catch (const std::out_of_range& oor) {
        std::cout << "did not find a sound resource. Here's hopefully some info:\n" << oor.what() << "\n";
        return soundBuffers.at("default.wav");
    }
    return *soundBuffer;
}

sf::Music& Resources::getMusic(const std::string &fileName) const {
    sf::Music* mus = NULL;
    try {
        mus = music.at(fileName);
    } catch (const std::out_of_range& oor) {
        std::cout << "did not find a music resource. Here's hopefully some info:\n" << oor.what() << "\n";
        return music.at("default.ogg");
    }
    return *mus;
}

const sf::Font& Resources::getFont(const std::string &fileName) const {
    sf::Font* font = NULL;
    try {
        font = fonts.at(fileName);
    } catch (const std::out_of_range& oor) {
        std::cout << "did not find a font resource. Here's hopefully some info:\n" << oor.what() << "\n";
        return fonts.at("BLASTER.TTF");
    }
    return font;
}
