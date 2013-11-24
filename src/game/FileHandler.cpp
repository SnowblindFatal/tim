#include "FileHandler.h"
#include "GameObject.h"
#include "WinCondition.h"

#include <iostream>
#include <fstream>

FileHandler::FileHandler(std::string path) : filePath(path)) { }

FileHandler::~FileHandler() { }

bool FileHandler::loadLevel(LevelData& level)
{
	std::ifstream file;
	std::string line;
	bool levelObjects = playerObjects = winConditions = false;
	file.open(filePath);
	
	if (!file.is_open()) 
	{
		// Error: Could not open file <path>
		return false;
	}
	
	while (getline(file, line))
	{
		if (line == "levelobjects")
		{
			while (getline(file, line))
				{
					GameObject* obj;
					if (line == ".")
						break;
					if ((obj = createObject(line)) == NULL)
						return false;
					level.addLevelObject(obj);
				}
		}
		if (line == "playerobjects")
		{
			while (getline(file, line))
				{
					GameObject* obj;
					if (line == ".")
						break;
					if ((obj = createObject(line)) == NULL)
						return false;
					level.addPlayerObject(obj);
				}
		}
	}
	
	if (levelObjects && playerObjects && goals)
		return true;
	errorMsg = "Level file corrupted. LevelObjects: " << levelObjects ", PlayerObjects: " << playerObjects
													<< ", WinConditions:" << winConditions << std::endl; // ### REMOVE ENDL LATER
	return false;
}

bool FileHandler::saveLevel(LevelData& level)
{
	// Implement this
}

GameObject* FileHandler::createObject(std::string& line)
{
	GameObject* obj;
	std::string delimiter = " ";
	size_t position = line.find(delimiter);
	std::string name = line.substr(0, line.find(delimiter));
	if (name == "Platform")
	{
		line.erase(0, )
		obj = new Platform()
	}
	else if (name == "Wall")
	{
		// Create wall
	}
	else if (name == "BigBall")
	{
		// Create big ball
	}
	else {
		errorMsg = "No object named " << name << "." << std::endl; // ### REMOVE ENDL LATER
		return NULL;
	}
}

std::string FileHandler::getError() const
{
	return errorMsg;
}
