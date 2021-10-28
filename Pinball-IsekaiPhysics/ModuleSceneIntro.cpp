#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

#include <math.h>

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ball_tex = fondo = spinner_tex = NULL;
	ray_on = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	ball_tex = App->textures->Load("pinball/ball.png"); 
	spinner_tex = App->textures->Load("pinball/spinner.png");
	fondo = App->textures->Load("pinball/pinball.png");
	//spring = App->textures->Load("pinball/pinball.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	CreateMap();

	ball = App->physics->CreateCircle(448, 750, 10);
	ball->body->SetBullet(true);
	ball->listener = this;

	App->physics->spinners[0] = App->physics->CreateSpinner(238, 399, 50, 14, true);
	App->physics->spinners[1] = App->physics->CreateSpinner(332, 399, 50, 14, false);
	App->physics->spinners[2] = App->physics->CreateSpinner(238, 854, 50, 14, true);
	App->physics->spinners[3] = App->physics->CreateSpinner(332, 854, 50, 14, false);
	bouncer[0] = App->physics->CreateBouncer(287, 190, 24);
	bouncer[1] = App->physics->CreateBouncer(240, 628, 24);
	bouncer[2] = App->physics->CreateBouncer(287, 692, 24);
	bouncer[3] = App->physics->CreateBouncer(334, 628, 24);

	py = App->physics->spring->body->GetPosition().y;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

update_status ModuleSceneIntro::PreUpdate()
{
	if (ball->body->GetPosition().y >= PIXEL_TO_METERS(940))
	{
		ball = App->physics->CreateCircle(448, 775, 10);
		ball->body->SetBullet(true);
		ball->listener = this;
	}

	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		ball = App->physics->CreateCircle(448, 775, 10);
		//ball = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10);
		ball->body->SetBullet(true);
		ball->listener = this;
	}

	//spring
	int sPositionX, sPositionY;
	
	App->physics->spring->GetPosition(sPositionX, sPositionY);
	App->physics->spring->body->ApplyForce({ 0,-10 }, { 0, 0 }, true);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		App->physics->spring->body->ApplyForce({ 0, 10 }, { 0, 0 }, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{ 
		float dif = App->physics->spring->body->GetPosition().y - py;

		if (dif < 1.1f)
		{
			App->physics->spring->body->ApplyForce({ 0, -100 }, { 0, 0 }, true);
		}
		else if (dif < 1.5f) 
		{
			App->physics->spring->body->ApplyForce({ 0, -175 }, { 0, 0 }, true);
		}
		else if (dif < 1.9f)
		{
			App->physics->spring->body->ApplyForce({ 0, -250 }, { 0, 0 }, true);
		}
		else if (dif < 2.3f)
		{
			App->physics->spring->body->ApplyForce({ 0, -325 }, { 0, 0 }, true);
		}
		else 
		{
			App->physics->spring->body->ApplyForce({ 0, -400 }, { 0, 0 }, true);
		}
	}

	/*if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}*/

	// Prepare for raycast ------------------------------------------------------
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	SDL_Rect r = { 0, 0, 508, 940 };
	SDL_Rect s = { 0, 0, 508, 940 };

	App->renderer->Blit(fondo, 0, 0, &r);
	// App->renderer->Blit(spring, &s);

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

	/*module = sqrt(pow(ball->body->GetLinearVelocity().x, 2) + pow(ball->body->GetLinearVelocity().y, 2));
	b2Vec2 v_u = { ball->body->GetLinearVelocity().x / module, ball->body->GetLinearVelocity().y / module };
	
	if (module > PIXEL_TO_METERS(1500))
	{
		ball->body->SetLinearVelocity({ v_u.x * PIXEL_TO_METERS(1500), v_u.y * PIXEL_TO_METERS(1500) });
	}*/

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate()
{
	App->renderer->Blit(ball_tex, METERS_TO_PIXELS(ball->body->GetPosition().x - 10), METERS_TO_PIXELS(ball->body->GetPosition().y - 10));

	for (size_t i = 0; i < 4; i++)
	{
		App->renderer->Blit(spinner_tex, METERS_TO_PIXELS(App->physics->spinners[i]->body->GetPosition().x - 25), METERS_TO_PIXELS(App->physics->spinners[i]->body->GetPosition().y - 7));
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA == ball)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (bodyB == bouncer[i])
			{
				//b2Vec2 v = 100 * (bodyB->body->GetPosition() - bodyA->body->GetPosition());
				//bodyA->body->ApplyForce({ 100, 100 }, bodyA->body->GetPosition(), true);
			}
		}
	}
	else if (bodyB == ball)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (bodyA == bouncer[i])
			{
				//b2Vec2 v = 100 * (bodyA->body->GetPosition() - bodyB->body->GetPosition());
				//->body->ApplyForce({ 100, 100 }, bodyB->body->GetPosition(), true);
			}
		}
	}
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

	int naranjas[8] = {
	176, 246,
	180, 246,
	180, 307,
	176, 307
	};
	
	int raya1[14] = {
	198, 551,
	201, 548,
	204, 551,
	204, 568,
	201, 570,
	198, 568,
	198, 553
	};

	int raya2[14] = {
	231, 550,
	233, 548,
	237, 550,
	237, 567,
	234, 570,
	231, 567,
	231, 553
	};


	int raya3[14] = {
	266, 551,
	269, 548,
	272, 551,
	272, 567,
	269, 570,
	266, 567,
	266, 553
	};

	int raya4[14] = {
	300, 551,
	303, 548,
	306, 551,
	306, 568,
	303, 570,
	300, 567,
	300, 553
	};

	int raya5[14] = {
	334, 550,
	337, 547,
	340, 550,
	340, 567,
	337, 570,
	334, 567,
	334, 552
	};

	int raya6[14] = {
	368, 551,
	371, 547,
	374, 552,
	374, 567,
	371, 570,
	368, 567,
	368, 554
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
	ricks.add(App->physics->CreateChain(0, 0, naranjas, 8));
	ricks.add(App->physics->CreateChain(0, 0, raya1, 14));
	ricks.add(App->physics->CreateChain(0, 0, raya2, 14));
	ricks.add(App->physics->CreateChain(0, 0, raya3, 14));
	ricks.add(App->physics->CreateChain(0, 0, raya4, 14));
	ricks.add(App->physics->CreateChain(0, 0, raya5, 14));
	ricks.add(App->physics->CreateChain(0, 0, raya6, 14));

	return true;
}
