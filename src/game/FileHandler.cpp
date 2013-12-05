#include "FileHandler.h"
#include "GameObject.h"
#include "WinCondition.h"

#include <iostream>
#include <fstream>
#include <sstream>

FileHandler::FileHandler(std::string path) : filePath(path)) { }

FileHandler::~FileHandler() { }

bool FileHandler::loadLevel(LevelData& level)
{
	std::ifstream file;
	std::string line;
	bool levelObjects = false;
	bool playerObjects = false;
	file.open(filePath);
	
	if (!file.is_open()) 
	{
		errorMsg = "Could not open file " + filePath + ".";
		return false;
	}
	
	while (getline(file, line))
	{
		if (line == "LevelObjects")
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
		if (line == "PlayerObjects")
		{
			while (getline(file, line))
			{
				GameObject* obj;
				if (line == ".")
					break;
				if ((obj = createObject(level, line)) == NULL)
					return false;
				level.addPlayerObject(obj);
			}
		}
	}
	
	if (levelObjects && playerObjects && goals)
		return true;
	std::stringstream errorStream;
	errorStream << "Level file corrupted. LevelObjects: " << levelObjects << ", PlayerObjects: " << playerObjects
													<< ", WinConditions:" << winConditions << std::endl; // ### REMOVE ENDL LATER
	errorMsg = errorStream.str();
	return false;
}

bool FileHandler::saveLevel(LevelData& level)
{
	// Implement this
}

GameObject* FileHandler::createObject(LevelData& level, std::string& line)
{
	GameObject* obj;
	std::vector<std::string> parameters;
	std::string name;
	std::stringstream ss;
	float x, y, width, height;
	
	parseLine(parameters, line);
	name = parameters[0];
	
	if (name == "Platform")
	{
		for (size_t i = 1;i < 5;i++)
			ss << parameters[i] << " ";
		ss >> x >> y >> width >> height;
		obj = new Platform(level);
		parameters.erase(parameters.begin(),parameters.begin()+5);
	}
	else if (name == "Wall")
	{
		for (size_t i = 1;i < 5;i++)
			ss << parameters[i] << " ";
		ss >> x >> y >> width >> height;
		obj = new Platform(level);
		parameters.erase(parameters.begin(),parameters.begin()+5);
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

void parseLine(std::vector<std::string>&, std::string&)
{
	if (line.length() > 0)
	{
		std::string delimiter = " ";
		size_t position = line.find(delimiter);
		while (true)
		{
			parameters.push_back(line.substr(0, position));
			if (position == std::string::npos)
			{
				line.erase(0, line.length());
				break;
			}
			line.erase(0, position + 1);
			position = line.find(delimiter);
		}
	}
}

std::string FileHandler::getError() const
{
	return errorMsg;
}
