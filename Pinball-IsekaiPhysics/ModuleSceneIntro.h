#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	bool CreateMap();

	PhysBody* bouncer[4];

	PhysBody* ball;

public:
	p2List<PhysBody*> ricks;

	SDL_Texture* ball_tex;
	SDL_Texture* spinner_tex;
	SDL_Texture* fondo;
	SDL_Texture* spring;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	float py;

	float module;
	float speedTX;
	float speedTY;
};
