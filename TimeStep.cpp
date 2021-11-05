#include "Application.h"
#include "Globals.h"
#include "ModuleRender.h"
#include "TimeStep.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "ModuleFonts.h"
#include "ModuleInput.h"

#include<string.h>

TimeStep::TimeStep(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

TimeStep::~TimeStep()
{

}

update_status TimeStep::PreUpdate() {
	return UPDATE_CONTINUE;
}

update_status TimeStep::Update() {

	// Fixed delta time + time wait

	double t = 0.0;
	double dt = 1.0 / 60.0; // Fixed 60fps = 16ms


	// start time counter
	frameStart = SDL_GetTicks();

	// Integrate for a dt = constant
	///integrate(posvel, t, dt);
	App->scene_intro->ScenePreUpdate();
	App->scene_intro->SceneUpdate();
	App->scene_intro->ScenePostUpdate();
	App->physics->PhysicsPreUpdate(dt, 6, 2);
	App->physics->PhysicsUpdate();
	App->physics->PhysicsPostUpdate();
	t += dt;
	
	// Render
	///render(posvel);


	//SDL_RenderPresent(App->renderer);
	//App->renderer->RenderPreUpdate();
	//App->renderer->RenderUpdate();
	//App->renderer->RenderPostUpdate();
	renderAll();

	// Measure time elapsed since tic
	telapsed = SDL_GetTicks() - frameStart; // ex. 5ms

	// wait until next frame
	// only if you have spare time!!!
	if (dt - telapsed > 0.0f) SDL_Delay(dt - telapsed);


	return UPDATE_CONTINUE;
}
update_status TimeStep::PostUpdate()
{

	
	return UPDATE_CONTINUE;
}

void TimeStep::renderAll()
{
	//_________________________IMPRIMIR UPDATE SCENE_________________________________
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = App->scene_intro->ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	SDL_Rect r = { 0, 0, 508, 940 };
	SDL_Rect s = { 0, 0, 508, 940 };

	App->renderer->Blit(App->scene_intro->fondo, 0, 0, &r);
	// App->renderer->Blit(spring, &s);

	// ray -----------------
	if (App->scene_intro->ray_on == true)
	{
		fVector destination(mouse.x - App->scene_intro->ray.x, mouse.y - App->scene_intro->ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(App->scene_intro->ray.x, App->scene_intro->ray.y, App->scene_intro->ray.x + destination.x, App->scene_intro->ray.y + destination.y, 255, 255, 255);

		if (normal.x != 0.0f)
			App->renderer->DrawLine(App->scene_intro->ray.x + destination.x, App->scene_intro->ray.y + destination.y, App->scene_intro->ray.x + destination.x + normal.x * 25.0f, App->scene_intro->ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}
	

	//_________________________IMPRIMIR POSTUPDATE SCENE_________________________________

	App->renderer->Blit(App->scene_intro->ball_tex, METERS_TO_PIXELS(App->scene_intro->ball->body->GetPosition().x - 10), METERS_TO_PIXELS(App->scene_intro->ball->body->GetPosition().y - 10));

	App->renderer->Blit(App->scene_intro->spring, METERS_TO_PIXELS(App->physics->spring->body->GetPosition().x - 12), METERS_TO_PIXELS(App->physics->spring->body->GetPosition().y - 6));

	for (size_t i = 0; i < 4; i++)
	{
		if (i % 2 == 0) {
			//if (App->physics->spinners[i]->left == true) {
			App->renderer->Blit(App->scene_intro->spinner_tex_izq, METERS_TO_PIXELS(App->physics->spinners[i]->body->GetPosition().x - 30), METERS_TO_PIXELS(App->physics->spinners[i]->body->GetPosition().y - 7), 0, 1.0f, App->physics->spinners[i]->body->GetAngle() * RADTODEG);
		}
		else {
			App->renderer->Blit(App->scene_intro->spinner_tex_der, METERS_TO_PIXELS(App->physics->spinners[i]->body->GetPosition().x - 30), METERS_TO_PIXELS(App->physics->spinners[i]->body->GetPosition().y - 7), 0, 1.0f, App->physics->spinners[i]->body->GetAngle() * RADTODEG);
		}

	}

	App->fonts->BlitText(0, 20, App->scene_intro->textFont, "Balls:");
	if (App->scene_intro->numBalls == 1) {
		App->fonts->BlitText(72, 20, App->scene_intro->textFont, App->scene_intro->ballsLeft, 1, 200, 20, 20);
	}
	else if (App->scene_intro->numBalls <= 0) {
		App->fonts->BlitText(72, 20, App->scene_intro->textFont, App->scene_intro->ballsLeft);
		if (App->scene_intro->numScoreAct > App->scene_intro->numScoreHigh) {
			App->fonts->BlitText(12, 300, App->scene_intro->textFont, "CONGRATS", 1, 255, 200, 20);
			App->fonts->BlitText(12, 330, App->scene_intro->textFont, "YOU BEAT");
			App->fonts->BlitText(2, 350, App->scene_intro->textFont, "THE RECORD");
			App->fonts->BlitText(12, 380, App->scene_intro->textFont, "SPACE TO");
			App->fonts->BlitText(7, 400, App->scene_intro->textFont, "TRY AGAIN");
		}
		else {
			//App->fonts->BlitText(0, 150, textFont, "GAME OVER|SPACE TO|TRY AGAIN",1, 255, 255, 255,1920,20);
			App->fonts->BlitText(5, 300, App->scene_intro->textFont, "GAME OVER", 1, 200, 20, 20);
			//App->fonts->BlitText(0, 150, textFont, "GAME OVER");
			App->fonts->BlitText(12, 330, App->scene_intro->textFont, "SPACE TO");
			App->fonts->BlitText(7, 350, App->scene_intro->textFont, "TRY AGAIN");
			//App->fonts->BlitText(0, 180, textFont, "PRESS SPACE");
			//App->fonts->BlitText(0, 200, textFont, "TO TRY AGAIN");
		}
	}
	else {
		App->fonts->BlitText(72, 20, App->scene_intro->textFont, App->scene_intro->ballsLeft);
	}

	App->fonts->BlitText(0, 50, App->scene_intro->textFont, "-Score-");
	App->fonts->BlitText(0, 80, App->scene_intro->textFont, "Current:");
	App->fonts->BlitText(0, 100, App->scene_intro->textFont, App->scene_intro->actualScore);
	App->fonts->BlitText(0, 130, App->scene_intro->textFont, "Previous:");
	App->fonts->BlitText(0, 150, App->scene_intro->textFont, App->scene_intro->prevScore);
	App->fonts->BlitText(0, 180, App->scene_intro->textFont, "Highest:");
	App->fonts->BlitText(0, 200, App->scene_intro->textFont, App->scene_intro->highScore);
	App->fonts->BlitText(0, 230, App->scene_intro->textFont, "---------");

	//_________________________IMPRIMIR POSTUPDATE PHYSICS_________________________________
	

	if (!App->physics->debug) {

	}
	else {




		// Bonus code: this will iterate all objects in the world and draw the circles
		// You need to provide your own macro to translate meters to pixels
		for (b2Body* b = App->physics->world->GetBodyList(); b; b = b->GetNext())
		{
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				switch (f->GetType())
				{
					// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 0, 0);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for (int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if (i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for (int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if (i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 128, 0);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 128, 0);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
				}

				
			}
		}

		// If the player keeps pressing the mouse button, update
		// target position and draw a red line between both anchor points
		if (App->physics->mouse_body != nullptr && App->physics->mouse_joint != nullptr)
		{
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				
				
				// Draw a red line between both anchor points
				App->renderer->DrawLine(METERS_TO_PIXELS(App->physics->mouse_body->GetPosition().x), METERS_TO_PIXELS(App->physics->mouse_body->GetPosition().y), App->input->GetMouseX(), App->input->GetMouseY(), 255, 0, 0);
			}
		}

	
	}
	
}

