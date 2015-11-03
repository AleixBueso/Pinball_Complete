#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	Ball.body = App->physics->CreateCircle(610, 747, 35, b2_dynamicBody);
	Ball.texture = App->textures->Load("pinball/pokeball.png");
	r_flipper.texture = App->textures->Load("pinball/r_flipper.png");
	l_flipper.texture = App->textures->Load("pinball/l_flipper.png");
	Ball.body->listener = this;


	int l_f[16] = {
		0, 18,
		4, 5,
		15, 0,
		91, 9,
		96, 18,
		89, 27,
		13, 34,
		5, 30
	};

	l_flipper.body = App->physics->CreatePolygon({ 182, 910, 0, 0 }, l_f, 16, b2_dynamicBody, 10.0f, 0.0f, false);
	l_flipper_anchor = App->physics->CreateCircle(182 + 14, 910 + 17, 10, b2_staticBody);
	App->physics->CreateRevoluteJoint(l_flipper.body, l_flipper_anchor, 14, 17, 0, 0, 30, -30);
	l_flipper.body->listener = this;
	
	int r_f[16] = {
		81, 0,
		6, 8,
		0, 17,
		6, 26,
		83, 35,
		91, 28,
		95, 17,
		91, 5
	};

	r_flipper.body = App->physics->CreatePolygon({ 303, 910, 0, 0 }, r_f, 16, b2_dynamicBody, 10.0f, 0.0f, false);
	r_flipper_anchor = App->physics->CreateCircle(303 + 80, 910 + 17, 10, b2_staticBody);
	App->physics->CreateRevoluteJoint(r_flipper.body, r_flipper_anchor, 80, 17, 0, 0, 30, -30);
	r_flipper.body->listener = this;
	

	pusher.body = App->physics->CreateRectangle(590, 1100, 42, 80, 0.5f, b2_dynamicBody);
	pusher_anchor = App->physics->CreateCircle(570, 1040, 1, b2_staticBody);
	App->physics->CreateLineJoint(pusher.body, pusher_anchor, 1, 1, 0, 0, 15, 0.5);
	pusher.body->listener = this;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(Ball.texture);
	Ball.body->~PhysBody();

	App->textures->Unload(r_flipper.texture);
	r_flipper.body->~PhysBody();

	App->textures->Unload(l_flipper.texture);
	l_flipper.body->~PhysBody();

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		r_flipper.body->Turn(360 * 15);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		l_flipper.body->Turn(-360 * 15);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		pusher.body->Push(0, -2200);
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP)
	{
		Ball.body->Push(0.0f, -400.0f);
	}

	int ball_x, ball_y;
	Ball.body->GetPosition(ball_x, ball_y);
	int r_flipper_x, r_flipper_y;
	r_flipper.body->GetPosition(r_flipper_x, r_flipper_y);
	int l_flipper_x, l_flipper_y;
	l_flipper.body->GetPosition(l_flipper_x, l_flipper_y);

	App->renderer->Blit(Ball.texture, ball_x, ball_y, NULL, 1.0f, Ball.body->GetRotation());
	App->renderer->Blit(r_flipper.texture, r_flipper_x + 10, r_flipper_y - 40, NULL, 1.0f, r_flipper.body->GetRotation() + 30, -5, 20);
	App->renderer->Blit(l_flipper.texture, l_flipper_x - 10, l_flipper_y + 5, NULL, 1.0f, l_flipper.body->GetRotation() - 28, 5, -2);
	if (ball_y > 1000)
	{
		Ball.body->SetLinearSpeed(0, 0);
		Ball.body->SetAngularSpeed(0);
		Ball.body->SetPosition(610, 747);
		lives--;
	}

	if (lives <= 0)
	{
		lives = 3;
		score = 0;
	}
	return UPDATE_CONTINUE;
}



