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
		GAME_TEST = 2
	};

};

#endif