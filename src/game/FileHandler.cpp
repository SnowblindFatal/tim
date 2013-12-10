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
		if (line == "Description")
		{
			while (getline(file, line))
			{
				if (line.length() == 0)
					continue; // Skip empty lines
				if (line == ".")
					break;
				level.setDescription(level.getDescription() + "\n" + line);
			}
		}
	}
	file.close();
	
	if (levelObjects && playerObjects && level.hasGoals())
	{
		level.setLoaded(true);
		return true;
	}
	std::stringstream errorStream;
	errorStream << "Level file corrupted. LevelObjects: " << levelObjects << ", PlayerObjects: " << playerObjects
													<< ", WinConditions: " << level.hasGoals();
	errorMsg = errorStream.str();
	return false;
}

bool FileHandler::saveLevel(LevelData& level)
{
	auto levelObjects = level.getLevelObjects();
	auto availableObjects = level.get_available();
	auto winConditions = level.getWinConditions();
	bool hasPlayerObjects = false;
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
	for (auto it = availableObjects.begin();it != availableObjects.end();it++)
	{
		if (it->second > 0)
		{
			hasPlayerObjects = true;
			break;
		}
	}
	if (!hasPlayerObjects)
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

	file << "LevelObjects\n";
	for (auto it = winConditions.begin();it != winConditions.end();it++)
	{
		WinCondition* cond = *it;
		for (auto obj = levelObjects.begin();obj != levelObjects.end();obj++)
		{
			if ((cond->getObject())->getID() == (*obj)->getID())
				file << objectToText(cond->getObject(), true) << " " << conditionToText(cond);
		}
	}
	
	for (auto it = levelObjects.begin();it != levelObjects.end();it++)
	{
		if ((*it)->getID() == 0)
			file << objectToText(*it, false);
	}
	
	file << ".\n",
	
	// Write player objects
	file << "PlayerObjects\n";
	for (auto it = availableObjects.begin();it != availableObjects.end();it++)
	{
		for (size_t i = it->second;i > 0;i--)
			file << it->first << "\n";
	}
	file << ".\n";
	
	// Write level description
	file << "Description\n" << level.getDescription() << "\n.";
	
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
		bool flipped;
		for (size_t i = 1;i < 4;i++)
			ss << parameters[i] << " ";
		ss >> x >> y >> flipped;
		obj = new Catapult(level.getWorld(), x, y, flipped);
		parameters.erase(parameters.begin(), parameters.begin()+4);
	}
	else if (name == "Seesaw")
	{
		bool flipped;
		for (size_t i = 1;i < 4;i++)
			ss << parameters[i] << " ";
		ss >> x >> y >> flipped;
		obj = new Seesaw(level.getWorld(), x, y, flipped);
		parameters.erase(parameters.begin(), parameters.begin()+4);
	}
	else if (name == "GravityChanger")
	{
		bool flipped;
		for (size_t i = 1;i < 4;i++)
			ss << parameters[i] << " ";
		ss >> x >> y >> flipped;
		obj = new GravityChanger(level.getWorld(), x, y, flipped);
		parameters.erase(parameters.begin(), parameters.begin()+4);
	}
	else if (name == "Bomb")
	{
		for (size_t i = 1;i < 3;i++)
			ss << parameters[i] << " ";
		ss >> x >> y;
		obj = new Bomb(level.getWorld(), x, y);
		parameters.erase(parameters.begin(), parameters.begin()+3);
	}
	else if (name == "Domino")
	{
		for (size_t i = 1;i < 3;i++)
			ss << parameters[i] << " ";
		ss >> x >> y;
		obj = new Domino(level.getWorld(), x, y);
		parameters.erase(parameters.begin(), parameters.begin()+3);
	}
	else if (name== "Teleport")
	{
		int flipped;
		for (size_t i = 1;i < 4;i++)
			ss<<parameters[i] << " ";
		ss >> x >> y >> flipped;
		obj = new Teleport(level.getWorld(), x, y, flipped);
		parameters.erase(parameters.begin(), parameters.begin()+4);
	}
	else if (name == "Crate")
	{
		for (size_t i = 1;i < 3;i++)
			ss << parameters[i] << " ";
		ss >> x >> y;
		obj = new Crate(level.getWorld(), x, y);
		parameters.erase(parameters.begin(), parameters.begin()+3);
	}
	else 
	{
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
	if (condType == "IsNearPoint" || condType == "IsNotNearPoint")
	{
		for (size_t i = 1;i < 4;i++)
			ss << parameters[i] << " ";
		ss >> x >> y >> tolerance;
		conds++;
		obj->setID(conds); // Set unique ID for GameObject. Used to combine WinCondition with GameObject during saveLevel.
		if (condType == "IsNearPoint") 
		{
			cond = new IsNearPoint(obj, x, y, tolerance);
		}
		else
		{
			cond = new IsNotNearPoint(obj, x, y, tolerance);
		}
		level.addWinCondition(cond);
	}
	else if (condType == "IsDestroyed")
	{
		conds++;
		obj->setID(conds);
		cond = new IsDestroyed(obj);
		level.addWinCondition(cond);
	}
	else 
	{
		ss.flush();
		ss << "No win condition named " << condType << ".";
		delete obj;
		return NULL;
	}
	
	return obj;
}

std::string FileHandler::objectToText(GameObject* obj, bool hasCondition)
{
	std::stringstream ss;
	std::string name = obj->getName();
	
	ss << name << " " << (obj->getPos()).x << " " << (obj->getPos()).y;
	
	if ((name == "Platform") || (name == "Wall"))
	{
		// x, y, width, height
		float width, height;
		if (name == "Platform") 
		{
			Platform* plat = dynamic_cast<Platform*>(obj);
			width = (plat->getDimensions()).x;
			height = (plat->getDimensions()).y;
		}
		else
		{
			Wall* wall = dynamic_cast<Wall*>(obj);
			width = (wall->getDimensions()).x;
			height = (wall->getDimensions()).y;
		}
		ss << " " << width << " " << height;
	} 
	else if ((name == "GravityChanger") || (name == "Catapult") || (name == "Seesaw"))
	{
		bool flipped;
		if (name == "GravityChanger")
		{
			GravityChanger* gc = dynamic_cast<GravityChanger*>(obj);
			flipped = gc->flipped;
		}
		else if (name == "Catapult")
		{
			Catapult* ca = dynamic_cast<Catapult*>(obj);
			flipped = ca->flipped;
		}
		else
		{
			Seesaw* se = dynamic_cast<Seesaw*>(obj);
			flipped = se->flipped;
		}
		// x, y, flipped
		ss << " " << flipped;
	}
	else if (name=="Teleport") {
		int flipped;
		Teleport* tp_ptr = dynamic_cast<Teleport*>(obj);
		flipped= tp_ptr->flipped;
		ss << " " << flipped;
	}
	
	if (!hasCondition)
		ss << "\n";
	return ss.str();
}

std::string FileHandler::conditionToText(WinCondition* cond)
{
	std::stringstream ss;
	std::string name = cond->getName();
	
	ss << name;
	
	if (name == "IsNearPoint")
	{
		IsNearPoint* inp = dynamic_cast<IsNearPoint*>(cond);
		// x, y, tolerance
		ss << " " << (inp->getPos()).x << " " << (inp->getPos()).y << " " << inp->getTolerance();
	}
	else if (name == "IsNotNearPoint")
	{
		IsNotNearPoint* inp = dynamic_cast<IsNotNearPoint*>(cond);
		ss << " " << (inp->getPos()).x << " " << (inp->getPos()).y << " " << inp->getTolerance();
	}
	/*
	#####################################
	### ADD MORE WIN CONDITIONS LATER ###
	#####################################
	*/
	
	ss << "\n";
	return ss.str();
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
