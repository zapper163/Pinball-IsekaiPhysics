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
	circle = fondo = NULL;
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

	//circle = App->textures->Load("pinball/wheel.png"); 
	fondo = App->textures->Load("pinball/pinball.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	CreateMap();

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
	//spring
	int sPositionX, sPositionY;
	App->physics->spring->GetPosition(sPositionX, sPositionY);
	App->physics->spring->body->ApplyForce({ 0,-10 }, { 0, 0 }, true);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->physics->spring->body->ApplyForce({ 0, 18 }, { 0, 0 }, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		App->physics->spring->body->ApplyForce({ 0, -400 }, { 0, 0 }, true);
	}

	/*if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}*/

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 12));
		circles.getLast()->data->listener = this;
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	SDL_Rect r = { 0, 0, 508, 940 };
	App->renderer->Blit(fondo, 0, 0, &r);

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

update_status ModuleSceneIntro::PostUpdate()
{


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

bool ModuleSceneIntro::CreateMap()
{
	int contorno[66] = {
	237, 937,
	143, 846,
	142, 730,
	175, 701,
	158, 683,
	158, 550,
	143, 510,
	142, 340,
	172, 309,
	172, 244,
	144, 154,
	143, 119,
	158, 81,
	178, 56,
	211, 40,
	392, 40,
	427, 57,
	449, 87,
	462, 124,
	461, 876,
	435, 876,
	435, 620,
	415, 620,
	415, 682,
	398, 700,
	429, 729,
	428, 846,
	335, 938,
	469, 939,
	470, 1,
	134, 0,
	133, 938,
	232, 938
	};

	int g_derecha[50] = {
	416, 571,
	415, 555,
	429, 530,
	428, 340,
	399, 311,
	398, 283,
	406, 281,
	420, 267,
	426, 250,
	424, 157,
	415, 132,
	405, 125,
	392, 119,
	372, 119,
	360, 124,
	351, 133,
	333, 133,
	332, 104,
	364, 71,
	387, 70,
	408, 81,
	423, 99,
	435, 133,
	435, 571,
	421, 571
	};

	int der_abajo[18] = {
	397, 506,
	403, 492,
	404, 365,
	391, 350,
	367, 373,
	367, 390,
	383, 405,
	382, 498,
	388, 507
	};

	int der_arriba[20] = {
	392, 245,
	396, 238,
	397, 166,
	389, 160,
	382, 158,
	375, 159,
	367, 166,
	380, 179,
	381, 236,
	386, 245
	};

	int izq_abajo[20] = {
	185, 506,
	190, 500,
	188, 405,
	205, 389,
	205, 371,
	182, 351,
	168, 363,
	168, 491,
	175, 506,
	180, 508
	};

	int izq_arriba[24] = {
	201, 197,
	206, 189,
	206, 166,
	240, 133,
	240, 104,
	212, 75,
	196, 81,
	189, 87,
	176, 111,
	175, 155,
	191, 195,
	196, 197
	};

	int triangulo_izq[14] = {
	201, 750,
	200, 790,
	217, 803,
	220, 803,
	221, 798,
	205, 750,
	203, 748
	};

	int triangulo_der[14] = {
	356, 803,
	372, 788,
	372, 753,
	370, 747,
	366, 750,
	351, 799,
	352, 802
	};

	int raya_arriba_izq[14] = {
	269, 105,
	271, 101,
	274, 105,
	274, 121,
	271, 123,
	268, 120,
	268, 108
	};

	int raya_arriba_der[12] = {
	303, 103,
	306, 105,
	306, 121,
	303, 123,
	300, 121,
	300, 105
	};

	int raya_abajo_izq[16] = {
	174, 747,
	174, 802,
	180, 813,
	206, 832,
	206, 838,
	179, 818,
	171, 805,
	170, 747
	};

	int raya_abajo_der[18] = {
	367, 831,
	391, 813,
	397, 803,
	397, 748,
	402, 748,
	402, 804,
	397, 814,
	366, 837,
	366, 833
	};

	ricks.add(App->physics->CreateChain(0, 0, contorno, 66));
	ricks.add(App->physics->CreateChain(0, 0, g_derecha, 50));
	ricks.add(App->physics->CreateChain(0, 0, der_abajo, 18));
	ricks.add(App->physics->CreateChain(0, 0, der_arriba, 20));
	ricks.add(App->physics->CreateChain(0, 0, izq_abajo, 20));
	ricks.add(App->physics->CreateChain(0, 0, izq_arriba, 24));
	ricks.add(App->physics->CreateChain(0, 0, triangulo_izq, 14));
	ricks.add(App->physics->CreateChain(0, 0, triangulo_der, 14));
	ricks.add(App->physics->CreateChain(0, 0, raya_arriba_izq, 14));
	ricks.add(App->physics->CreateChain(0, 0, raya_arriba_der, 12));
	ricks.add(App->physics->CreateChain(0, 0, raya_abajo_izq, 16));
	ricks.add(App->physics->CreateChain(0, 0, raya_abajo_der, 18));

	return true;
}
