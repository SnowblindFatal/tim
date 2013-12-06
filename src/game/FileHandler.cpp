#include "FileHandler.h"
#include "GameObject.h"
#include "WinCondition.h"

#include <iostream>
#include <fstream>
#include <sstream>

FileHandler::FileHandler(std::string path) : filePath(path), conds(0) { }

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
				if ((obj = createObject(level, line)) == NULL)
					return false;
				level.addLevelObject(obj);
				levelObjects = true;
			}
		}
		if (line == "PlayerObjects")
		{
			while (getline(file, line))
			{
				if (line == ".")
					break;
				if (!level.addPlayerObject(line))
				{
					errorMsg = "Invalid player object.";
					return false;
				}
				playerObjects = true;
			}
		}
	}
	file.close();
	
	if (levelObjects && playerObjects && level.hasGoals())
		return true;
	std::stringstream errorStream;
	errorStream << "Level file corrupted. LevelObjects: " << levelObjects << ", PlayerObjects: " << playerObjects
													<< ", WinConditions:" << level.hasGoals();
	errorMsg = errorStream.str();
	return false;
}

bool FileHandler::saveLevel(LevelData& level)
{
	auto levelObjects = level.getLevelObjects();
	auto availableObjects = level.get_available();
	auto winConditions = level.getWinConditions();
	// Check that level contains level- and player objects and win conditions
	if (winConditions.empty())
	{
		errorMsg = "No goals defined.";
		return false;
	}
	else if (levelObjects.empty())
	{
		errorMsg = "No level objects.";
		return false;
	}
	else if (availableObjects.empty())
	{
		errorMsg = "No player objects.";
		return false;
	}
	
	std::ofstream file;
	std::string line; // Needed?
	std::stringstream ss;
	
	file.open(filePath);
	if (!file.is_open()) 
	{
		errorMsg = "Could not open file " + filePath + ".";
		return false;
	}
	
	// Write level objects
	// TODO: Implement function to handle convertion from GameObject to text
	file << "LevelObjects\n";
	for (auto it = winConditions.begin();it != winConditions.end();it++)
	{
		WinCondition* cond = *it;
		for (auto obj = levelObjects.begin();obj != levelObjects.end();obj++)
		{
			if ((cond->getObject())->getID() == cond->getID())
			{
				// Convert GameObject to text followed by WinCondition in text form and write to file
			}
		}
	}
	
	for (auto it = levelObjects.begin();it != levelObjects.end();it++)
	{
		if ((*it)->getID() == 0)
		{
			// Convert GameObject to text and write to file
		}
	}
	
	file << ".\n",
	
	// Write player objects
	file << "PlayerObjects\n";
	for (auto it = availableObjects.begin();it != availableObjects.end();it++)
	{
		for (size_t i = it->second;i > 0;i--)
			file << it->first << "\n";
	}
	file << ".";
	
	file.close();
	return true;
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
		obj = new Platform(level.getWorld(), x, y, width, height);
		parameters.erase(parameters.begin(),parameters.begin()+5);
	}
	else if (name == "Wall")
	{
		for (size_t i = 1;i < 5;i++)
			ss << parameters[i] << " ";
		ss >> x >> y >> width >> height;
		obj = new Wall(level.getWorld(), x, y, width, height);
		parameters.erase(parameters.begin(),parameters.begin()+5);
	}
	else if (name == "BouncingBall")
	{
		for (size_t i = 1;i < 3;i++)
			ss << parameters[i] << " ";
		ss >> x >> y;
		obj = new BouncingBall(level.getWorld(), x, y);
		parameters.erase(parameters.begin(), parameters.begin()+3);
	}
	else if (name == "BigBall")
	{
		for (size_t i = 1;i < 3;i++)
			ss << parameters[i] << " ";
		ss >> x >> y;
		obj = new BigBall(level.getWorld(), x, y);
		parameters.erase(parameters.begin(), parameters.begin()+3);
	}
	else if (name == "BowlingBall")
	{
		for (size_t i = 1;i < 3;i++)
			ss << parameters[i] << " ";
		ss >> x >> y;
		obj = new BowlingBall(level.getWorld(), x, y);
		parameters.erase(parameters.begin(), parameters.begin()+3);
	}
	else if (name == "Catapult")
	{
		for (size_t i = 1;i < 3;i++)
			ss << parameters[i] << " ";
		ss >> x >> y;
		obj = new Catapult(level.getWorld(), x, y);
		parameters.erase(parameters.begin(), parameters.begin()+3);
	}
	else if (name == "Seesaw")
	{
		for (size_t i = 1;i < 3;i++)
			ss << parameters[i] << " ";
		ss >> x >> y;
		obj = new Seesaw(level.getWorld(), x, y);
		parameters.erase(parameters.begin(), parameters.begin()+3);
	}
	else if (name == "GravityChanger")
	{
		for (size_t i = 1;i < 3;i++)
			ss << parameters[i] << " ";
		ss >> x >> y;
		obj = new GravityChanger(level.getWorld(), x, y);
		parameters.erase(parameters.begin(), parameters.begin()+3);
	}
	else if (name == "Bomb")
	{
		for (size_t i = 1;i < 3;i++)
			ss << parameters[i] << " ";
		ss >> x >> y;
		obj = new Bomb(level.getWorld(), x, y);
		parameters.erase(parameters.begin(), parameters.begin()+3);
	}
	else {
		ss.flush();
		ss << "No object named " << name << ".";
		errorMsg = ss.str();
		return NULL;
	}
	if (!parameters.empty())
		return createWinCondition(level, obj, parameters);
	return obj;
}

GameObject* FileHandler::createWinCondition(LevelData& level, GameObject* obj, std::vector<std::string>& parameters)
{
	WinCondition* cond;
	std::string condType = parameters[0];
	std::stringstream ss;
	float x, y, tolerance;
	if (condType == "IsNearPoint")
	{
		for (size_t i = 1;i < 4;i++)
			ss << parameters[i] << " ";
		ss >> x >> y >> tolerance;
		conds++;
		obj->setID(conds); // Set unique ID for GameObject. Used to combine WinCondition with GameObject during saveLevel.
		cond = new IsNearPoint(obj, x, y, tolerance);
		level.addWinCondition(cond);
	}
	else 
	{
		ss.flush();
		ss << "No win condition named " << condType << ".";
		return NULL;
	}
	
	
	return obj;
}

void FileHandler::parseLine(std::vector<std::string>& parameters, std::string& line)
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

std::string FileHandler::getError() const
{
	return errorMsg;
}
