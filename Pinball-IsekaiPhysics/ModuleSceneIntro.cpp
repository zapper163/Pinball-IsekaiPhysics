#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = fondo = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	//fondo = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	App->physics->spinners[0] = App->physics->CreateSpinner(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 50, 20, true);
	App->physics->spinners[1] = App->physics->CreateSpinner((SCREEN_WIDTH / 4) * 3, SCREEN_HEIGHT / 2, 50, 20, false);
	//App->physics->

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int contorno[80] = {
		67, 469,
		67, 0,
		234, 0,
		234, 469,
		167, 469,
		214, 422,
		214, 365,
		199, 350,
		207, 342,
		207, 310,
		217, 310,
		217, 438,
		231, 438,
		231, 61,
		227, 49,
		223, 42,
		219, 36,
		213, 29,
		207, 25,
		200, 22,
		195, 20,
		106, 20,
		99, 23,
		93, 26,
		87, 31,
		82, 37,
		78, 43,
		75, 50,
		72, 58,
		71, 77,
		86, 120,
		86, 156,
		71, 170,
		71, 254,
		78, 275,
		78, 342,
		86, 350,
		72, 365,
		72, 424,
		118, 469
		};
		
		int piezag[66] = {
		207, 286,
		217, 286,
		218, 66,
		212, 51,
		204, 40,
		195, 36,
		183, 35,
		166, 52,
		166, 66,
		175, 66,
		184, 60,
		195, 59,
		207, 66,
		212, 79,
		212, 126,
		207, 137,
		200, 142,
		200, 155,
		214, 170,
		214, 178,
		210, 183,
		203, 183,
		195, 175,
		184, 187,
		183, 195,
		191, 202,
		191, 239,
		199, 253,
		212, 254,
		214, 257,
		214, 265,
		207, 277,
		207, 284
		};
		
		int piezap[18] = {
		88, 253,
		84, 245,
		84, 181,
		91, 175,
		102, 186,
		102, 196,
		95, 203,
		95, 250,
		92, 253
		};

		int pieza[22] = {
		100, 99,
		103, 95,
		103, 83,
		120, 66,
		120, 52,
		105, 37,
		103, 37,
		93, 45,
		87, 57,
		87, 78,
		96, 99
		};

		ricks.add(App->physics->CreateChain(0,0, contorno, 80));
		ricks.add(App->physics->CreateChain(0,0, piezag, 66));
		ricks.add(App->physics->CreateChain(0, 0, piezap, 18));
		ricks.add(App->physics->CreateChain(0, 0, pieza, 22));
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(fondo, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
