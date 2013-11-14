#include "Level.h"

void LevelData::loadlevel() {
  //Load a demo level:
/*
	levelobjects.push_back(new HorizontalBlock(phys_world,12.0f, 40.0f));
	levelobjects.push_back(new DroppingSquare(phys_world,22.5f,20.0f));
	levelobjects.push_back(new DroppingSquare(phys_world,23.0f,18.0f));
	levelobjects.push_back(new DroppingSquare(phys_world,10.0f,20.0f,3.0f));
	levelobjects.push_back(new ExampleSquare(phys_world,15.0f,20.0f));
	levelobjects.push_back(new BouncingBall(phys_world,31.0f,25.0f));
	levelobjects.push_back(new Platform(phys_world, 30.0f, 30.0f, 4.0f, 4.0f));
	levelobjects.push_back(new Platform(phys_world, 34.0f, 34.0f, 8.0f, 4.0f));
	levelobjects.push_back(new Platform(phys_world, 42.0f, 38.0f, 12.0f, -6.0f));
	levelobjects.push_back(new BouncingBall(phys_world, 10.0f, 10.0f));

	levelobjects.push_back(new Platform(phys_world, 20.0, 40.0, 40.0, 0));
	levelobjects.push_back(new Platform(phys_world, 5.0, 20.0, 15.0, 20.0));
	levelobjects.push_back(new BigBall(phys_world, 6.0, 15.0));
	
	levelobjects.push_back(new BouncingBall(phys_world, 50.0f, 10.0f));
	levelobjects.push_back(new Chain(phys_world));

	levelobjects.push_back(new Platform(phys_world, 15.0, 12.0, 1.0, 0));
	levelobjects.push_back(new Platform(phys_world, 35.0, 12.0, 1.0, 0));
	level_loaded = true;
	*/


	//Load a "bare minimum" playable level:

	levelobjects.push_back(new Platform(phys_world, 20.0, 40.0, 40.0, 0));
	for (size_t i = 0; i<10; i++)
	{
		levelobjects.push_back(new Domino(phys_world, 25.0f+i*2.5f, 38.0f));
	}
	/* Chain test
	levelobjects.push_back(new Chain(phys_world));
	levelobjects.push_back(new Platform(phys_world, 15.0, 15.0, 2.0, 0));
	levelobjects.push_back(new Platform(phys_world, 25.0, 15.0, 2.0, 0));
	*/
	levelobjects.push_back(new BigBall(phys_world, 6.0, 15.0));
	winconditions.push_back(new IsNearPoint(levelobjects.back(), 40.0f, 40.0f, 5.0f));
	available["Platform"]=1;
	available["Wall"]=1;

}
