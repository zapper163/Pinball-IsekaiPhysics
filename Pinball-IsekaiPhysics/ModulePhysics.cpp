#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	//spring
	b2Filter b;
	b.categoryBits = 1;
	b.maskBits = 1 | 0;
	spring = CreateRectangle(448, 750, 24, 12);
	spring->body->SetType(b2_dynamicBody);
	spring->body->GetFixtureList()->SetFilterData(b);
	pivotSpring = CreateRectangle(448, (750) + 120, 24, 12);
	pivotSpring->body->SetType(b2_staticBody);
	pivotSpring->body->GetFixtureList()->SetFilterData(b);
	App->physics->CreatePrismaticJoint(spring, pivotSpring);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	/*
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}
	*/
	return UPDATE_CONTINUE;
}


void ModulePhysics::PhysicsPreUpdate(double posVel, double t, double dt) {
	//world->Step(dt, posVel, 2);
	//world->Step(dt, 6, posVel);
	world->Step(dt, 6, 2);

	for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if (c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if (pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}
}

update_status ModulePhysics::Update()
{
	/*
	for (size_t i = 0; i < 4; i++)
	{
		if (i % 2 == 0)
		{
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
			{
				spinners[i]->body->SetAngularVelocity(-15);
				spinners[i]->isActive = true;
			}
			if (spinners[i]->isActive == true)
			{
				if (spinners[i]->cd <= 5)
				{
					spinners[i]->body->SetAngularVelocity(15);
				}
				if (spinners[i]->cd <= 0)
				{
					spinners[i]->body->SetAngularVelocity(0);
					spinners[i]->cd = 10;
					spinners[i]->isActive = false;
				}
				else
				{
					spinners[i]->cd -= DEGTORAD;
				}
			}
		}
		else
		{
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
			{
				spinners[i]->body->SetAngularVelocity(15);
				spinners[i]->isActive = true;
			}
			if (spinners[i]->isActive == true)
			{
				if (spinners[i]->cd <= 5)
				{
					spinners[i]->body->SetAngularVelocity(-15);
				}
				if (spinners[i]->cd <= 0)
				{
					spinners[i]->body->SetAngularVelocity(0);
					spinners[i]->cd = 10;
					spinners[i]->isActive = false;
				}
				else
				{
					spinners[i]->cd -= DEGTORAD;
				}
			}
		}
	}
	*/

	return UPDATE_CONTINUE;
}

void ModulePhysics::PhysicsUpdate(){
	for (size_t i = 0; i < 4; i++)
	{
		if (i % 2 == 0)
		{
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
			{
				spinners[i]->body->SetAngularVelocity(-15);
				spinners[i]->isActive = true;
			}
			if (spinners[i]->isActive == true)
			{
				if (spinners[i]->cd <= 5)
				{
					spinners[i]->body->SetAngularVelocity(15);
				}
				if (spinners[i]->cd <= 0)
				{
					spinners[i]->body->SetAngularVelocity(0);
					spinners[i]->cd = 10;
					spinners[i]->isActive = false;
				}
				else
				{
					spinners[i]->cd -= DEGTORAD;
				}
			}
		}
		else
		{
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
			{
				spinners[i]->body->SetAngularVelocity(15);
				spinners[i]->isActive = true;
			}
			if (spinners[i]->isActive == true)
			{
				if (spinners[i]->cd <= 5)
				{
					spinners[i]->body->SetAngularVelocity(-15);
				}
				if (spinners[i]->cd <= 0)
				{
					spinners[i]->body->SetAngularVelocity(0);
					spinners[i]->cd = 10;
					spinners[i]->isActive = false;
				}
				else
				{
					spinners[i]->cd -= DEGTORAD;
				}
			}
		}
	}
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	
	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

void ModulePhysics::CreatePrismaticJoint(PhysBody* dynami, PhysBody* stati)
{

	b2PrismaticJointDef prismaticJoint;
	prismaticJoint.collideConnected = true;
	prismaticJoint.bodyA = spring->body;
	prismaticJoint.bodyB = pivotSpring->body;

	prismaticJoint.localAnchorA.Set(0, 0);
	prismaticJoint.localAnchorB.Set(0, -1.5f);
	prismaticJoint.localAxisA.Set(0, -1);
	prismaticJoint.enableLimit = true;
	prismaticJoint.lowerTranslation = -0.02;
	prismaticJoint.upperTranslation = 1.25f;
	(b2PrismaticJoint*)world->CreateJoint(&prismaticJoint);

}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(METERS_TO_PIXELS(x),METERS_TO_PIXELS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

Spinner* ModulePhysics::CreateSpinner(int x, int y, int w, int h, bool left)
{
	Spinner* s = new Spinner();

	b2BodyDef spinner;
	spinner.type = b2_dynamicBody;
	spinner.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	s->body = world->CreateBody(&spinner);

	b2PolygonShape spinner_shape;
	spinner_shape.SetAsBox(PIXEL_TO_METERS(w) * 0.5f, PIXEL_TO_METERS(h) * 0.5f);
	//b2Vec2 vertices[4];


	/*
	if (left)
	{
		vertices[0].Set(-PIXEL_TO_METERS(w / 2), 0.0f);
		vertices[1].Set(-PIXEL_TO_METERS(h / 2), -PIXEL_TO_METERS(h / 2));
		vertices[2].Set(PIXEL_TO_METERS(w / 2), 0.0f);
		vertices[3].Set(-PIXEL_TO_METERS(h / 2), PIXEL_TO_METERS(h / 2));
	}
	else
	{
		vertices[0].Set(-PIXEL_TO_METERS(w / 2), 0.0f);
		vertices[1].Set(PIXEL_TO_METERS(h / 2), -PIXEL_TO_METERS(h / 2));
		vertices[2].Set(PIXEL_TO_METERS(w / 2), 0.0f);
		vertices[3].Set(PIXEL_TO_METERS(h / 2), PIXEL_TO_METERS(h / 2));
	}
	
	spinner_shape.Set(vertices, 4);

	*/
	b2FixtureDef spinner_fixture;
	spinner_fixture.shape = &spinner_shape;
	s->body->CreateFixture(&spinner_fixture);

	b2BodyDef anchor;
	anchor.type = b2_staticBody;

	if (left)
	{
		anchor.position.Set(s->body->GetPosition().x - PIXEL_TO_METERS(w / 2), s->body->GetPosition().y);
	}
	else
	{
		anchor.position.Set(s->body->GetPosition().x + PIXEL_TO_METERS(w / 2), s->body->GetPosition().y);
	}
	
	s->anchor = world->CreateBody(&anchor);

	b2PolygonShape anchor_box;
	anchor_box.SetAsBox(PIXEL_TO_METERS(1) * 0.5f, PIXEL_TO_METERS(1) * 0.5f);

	b2FixtureDef anchor_fixture;
	anchor_fixture.shape = &anchor_box;
	s->anchor->CreateFixture(&anchor_fixture);

	b2RevoluteJointDef jointDef;
	jointDef.Initialize(s->anchor, s->body, s->anchor->GetWorldCenter());
	jointDef.lowerAngle = -0.5f * b2_pi; // -90 degrees
	jointDef.upperAngle = 0.25f * b2_pi; // 45 degrees
	jointDef.enableLimit = true;
	jointDef.maxMotorTorque = 10.0f;
	jointDef.motorSpeed = 0.0f;
	jointDef.enableMotor = true;

	b2RevoluteJoint* joint;
	joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);

	return s;
}

PhysBody* ModulePhysics::CreateBouncer(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.restitution = 1.7f;
	b->CreateFixture(&fixture);

	PhysBody* bouncer = new PhysBody();
	bouncer->body = b;
	b->SetUserData(bouncer);
	bouncer->width = bouncer->height = radius;

	return bouncer;
}

update_status ModulePhysics::PostUpdate()
{
	return UPDATE_CONTINUE;
}

//update_status ModulePhysics::PostUpdate()
void ModulePhysics::PhysicsPostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					//App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 0, 0);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							//App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					//App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
						//	App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 128, 0);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					//App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 128, 0);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					//App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				// test if the current body contains mouse position
				b2Vec2 p = { PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) };
				if (f->GetShape()->TestPoint(b->GetTransform(), p) == true)
				{

					// If a body was selected, create a mouse joint
					// using mouse_joint class property

					// NOTE: you do TODO2 here or also in the original handout's location. 
					// It doesn't matter unless you are triggering several objects at once;
					// I leave it to you to add safety checks to avoid re-defining several mouse joints.

					// The variable "b2Body* mouse_body;" is defined in the header ModulePhysics.h 
					// We need to keep this body throughout several game frames; you cannot define it as a local variable here. 
					mouse_body = b;

					// Get current mouse position
					b2Vec2 mousePosition;
					mousePosition.x = p.x;
					mousePosition.y = p.y;

					// Define new mouse joint
					b2MouseJointDef def;
					def.bodyA = ground; // First body must be a static ground
					def.bodyB = mouse_body; // Second body will be the body to attach to the mouse
					def.target = mousePosition; // The second body will be pulled towards this location
					def.dampingRatio = 0.5f; // Play with this value
					def.frequencyHz = 2.0f; // Play with this value
					def.maxForce = 200.0f * mouse_body->GetMass(); // Play with this value

					// Add the new mouse joint into the World
					mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
				}
			}
		}
	}

	// If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points
	if (mouse_body != nullptr && mouse_joint != nullptr)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			// Get new mouse position and re-target mouse_joint there
			b2Vec2 mousePosition;
			mousePosition.x = PIXEL_TO_METERS(App->input->GetMouseX());
			mousePosition.y = PIXEL_TO_METERS(App->input->GetMouseY());
			mouse_joint->SetTarget(mousePosition);

			// Draw a red line between both anchor points
			//App->renderer->DrawLine(METERS_TO_PIXELS(mouse_body->GetPosition().x), METERS_TO_PIXELS(mouse_body->GetPosition().y), App->input->GetMouseX(), App->input->GetMouseY(), 255, 0, 0);
		}
	}

	//If the player releases the mouse button, destroy the joint
	if (mouse_body != nullptr && mouse_joint != nullptr)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
		{
			// Tell Box2D to destroy the mouse_joint
			world->DestroyJoint(mouse_joint);

			// DO NOT FORGET THIS! We need it for the "if (mouse_body != nullptr && mouse_joint != nullptr)"
			mouse_joint = nullptr;
			mouse_body = nullptr;
		}
	}

	return;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::EndContact(b2Contact* contact)
{
	LOG("collision");
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}