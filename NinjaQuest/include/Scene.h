#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:

	~Scene() {}

	virtual void OnStart() = 0;
	virtual int OnUpdate() = 0;
	virtual void OnEnd() = 0;

	enum Scenes
	{
		SPLASH = 0,
		MENU = 1
	};

};

#endif