#pragma once
#include "Module.h"
#include "Globals.h"

struct SDL_Texture;

class TimeStep : public Module
{
public:

	TimeStep(Application* app, bool start_enabled = true);

	~TimeStep();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	void renderAll();

	double frameStart;
	double telapsed;

};


