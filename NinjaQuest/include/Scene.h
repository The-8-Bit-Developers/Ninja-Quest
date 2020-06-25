#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:

	virtual ~Scene() = 0;

	virtual void OnStart() = 0;
	virtual int OnUpdate() = 0;
	virtual void OnEnd() = 0;

	enum Scenes
	{
		SPLASH = 0,
		MENU = 1,
		LEVEL0 = 2,
		LEVEL1 = 5,
		LEVEL2 = 6,
		LEVEL3 = 7,
		LEVEL4 = 8,
		GAME_OVER = 3,
		INTRO = 4,
		WINNING = 9
	};

};

#endif