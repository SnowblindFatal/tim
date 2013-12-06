#ifndef FILEHANDLER_H
#define	FILEHANDLER_H

#include "Level.h"
#include "GameObject.h"
#include "WinCondition.h"

#include <string>
#include <vector>

class FileHandler {
	public:
	FileHandler(std::string);
	~FileHandler();
	// Returns true if the loading/saving operation was successful.
	// Returns false and sets errorMsg if the operation failed.
	bool loadLevel(LevelData&);
	bool saveLevel(LevelData&);
	std::string getError() const;
	
	private:
	GameObject* createObject(LevelData&, std::string&);
	GameObject* createWinCondition(LevelData&, GameObject*, std::vector<std::string>&);
	void parseLine(std::vector<std::string>&, std::string&);
	std::string filePath;
	std::string errorMsg;
	int conds;
};

/* 
Usage example:

	LevelData level(App);
	FileHandler fh("levels/level1.lvl");
	if (fh.loadLevel(level)) {
		// Load up playing field
	} else {
		errorMessage(fh.getError());
		// Return to level selection screen
	}
*/

#endif
