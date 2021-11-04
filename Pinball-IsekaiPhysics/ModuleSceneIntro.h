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
	PhysBody* sensor500[3];
	PhysBody* sensor1000;


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

	int textFont = -1;
	int numBalls = 3;
	char ballsLeft[2] = { "\0" };

	int numScoreAct = 0;
	int numScoreHigh = 0;
	int numScorePrev = 0;
	char actualScore[7] = { "\0" };
	char highScore[7] = { "\0" };
	char prevScore[7] = { "\0" };
};
