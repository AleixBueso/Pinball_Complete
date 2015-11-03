#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h" 
#include "ModulePlayer.h"
#include "SDL\include\SDL_timer.h"

#define BOUNCER_TIME 200;

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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

	App->audio->PlayMusic("pinball/Tablero_Azul_Pinball.ogg", 0.0f);
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	PinballMap = App->textures->Load("pinball/bluetable_base.png");

	CreatePinball();
	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	App->textures->Unload(PinballMap);
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(PinballMap, 0, 0);
	
	p2List_item<PhysBody*>* tmp_1 = shellders.getFirst();
	while (tmp_1 != NULL)
	{
		if (tmp_1->data->hit_timer > 0)
		{
			if (SDL_TICKS_PASSED(SDL_GetTicks(), tmp_1->data->hit_timer) == false)
			{
				int x, y;
				tmp_1->data->GetPosition( x, y);
				App->renderer->Blit(tmp_1->data->texture, x - 8, y - 10);
			}

			else
			{
				tmp_1->data->hit_timer = 0;
				App->player->score += 20;
			}
			
		}
		tmp_1 = tmp_1->next;
	}

	
	if (triangles.getFirst()->data->hit_timer > 0)
		{
			if (SDL_TICKS_PASSED(SDL_GetTicks(), triangles.getFirst()->data->hit_timer) == false)
			{
				int x, y;
				triangles.getFirst()->data->GetPosition(x, y);
				App->renderer->Blit(triangles.getFirst()->data->texture, x + 128, y + 750);
			}

			else
			{
				triangles.getFirst()->data->hit_timer = 0;
				App->player->score += 10;
			}
		
		}

	if (triangles.getLast()->data->hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), triangles.getLast()->data->hit_timer) == false)
		{
			int x, y;
			triangles.getLast()->data->GetPosition(x, y);
			App->renderer->Blit(triangles.getLast()->data->texture, x + 400, y + 750);
		}

		else
		{
			triangles.getLast()->data->hit_timer = 0;
			App->player->score += 10;
		}

	}
	
	if (pokemon.getFirst()->data->hit_timer > 0)
		{
			if (SDL_TICKS_PASSED(SDL_GetTicks(), pokemon.getFirst()->data->hit_timer) == false)
			{
				int x, y;
				pokemon.getFirst()->data->GetPosition(x, y);
				App->renderer->Blit(pokemon.getFirst()->data->texture, x - 54, y - 3);
			}

			else
			{
				pokemon.getFirst()->data->hit_timer = 0;
				App->player->score += 100;
			}
			
		}

	if (pokemon.getLast()->data->hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), pokemon.getLast()->data->hit_timer) == false)
		{
			int x, y;
			pokemon.getLast()->data->GetPosition(x, y);
			App->renderer->Blit(pokemon.getLast()->data->texture, x + 4, y - 19);
		}

		else
		{
			pokemon.getLast()->data->hit_timer = 0;
			App->player->score += 100;
		}

	}
	
	if (PsyDuck->hit_timer > 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), PsyDuck->hit_timer) == false)
		{
			int x, y;
			PsyDuck->GetPosition(x, y);
			App->renderer->Blit(PsyDuck->texture, x - 10, y);
		}

		else
		{
			PsyDuck->hit_timer = 0;
			App->player->score += 50;
		}
	}
	
	if (right_light->hit_timer < 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), right_light->hit_timer) == false)
			App->renderer->Blit(right_light->texture, 130, 489);
		else
		{
			right_light->hit_timer = 0;
			App->player->score += 40;
		}
	}

	if (left_light->hit_timer < 0)
	{
		if (SDL_TICKS_PASSED(SDL_GetTicks(), left_light->hit_timer) == false)
			App->renderer->Blit(left_light->texture, 442, 489);
		else
		{
			right_light->hit_timer = 0;
			App->player->score += 40;

		}
	}
	char title[50];
	sprintf_s(title, "Balls: %d Score: %06d", App->player->lives, App->player->score);
	App->window->SetTitle(title);


	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* body1, PhysBody* body2)
{
	p2List_item<PhysBody*>* tmp_1 = shellders.getFirst();
	while (tmp_1 != NULL)
	{
		if (tmp_1->data == body1)
		{
			tmp_1->data->hit_timer = SDL_GetTicks() + BOUNCER_TIME;
			App->audio->PlayFx(shellders.getFirst()->data->fx);
			LOG("Collision!");
			return;
		}
		tmp_1 = tmp_1->next;
	}

	
	if (triangles.getFirst()->data == body1)
		{
			triangles.getFirst()->data->hit_timer = SDL_GetTicks() + BOUNCER_TIME;
			App->audio->PlayFx(triangles.getLast()->data->fx);
			LOG("Collision!");
			return;
		}

	if (triangles.getLast()->data == body1)
	{
		triangles.getLast()->data->hit_timer = SDL_GetTicks() + BOUNCER_TIME;
		App->audio->PlayFx(triangles.getLast()->data->fx);
		LOG("Collision!");
		return;
	}

	if (PsyDuck == body1)
	{
		PsyDuck->hit_timer = SDL_GetTicks() + BOUNCER_TIME;
		App->audio->PlayFx(PsyDuck->fx);
		LOG("Collision!");
		return;
	}

	p2List_item<PhysBody*>* tmp_3 = pokemon.getFirst();
	while (tmp_3 != NULL)
	{
		if (tmp_3->data == body1)
		{
			tmp_3->data->hit_timer = SDL_GetTicks() + BOUNCER_TIME;
			App->audio->PlayFx(tmp_3->data->fx);
			LOG("Collision!");
			return;
		}
		tmp_3 = tmp_3->next;
	}

	if (right_light == body1)
	{
		right_light->hit_timer = SDL_GetTicks() + BOUNCER_TIME;
		LOG("Collision!");
		App->audio->PlayFx(right_light->fx);
		return;
	}

	if (left_light == body1)
	{
		left_light->hit_timer = SDL_GetTicks() + BOUNCER_TIME;
		LOG("Collision!");
		App->audio->PlayFx(right_light->fx);
		return;
	}
		return;
	}

void ModuleSceneIntro::CreatePinball()
{

	int pinball_body[80] = {
		640, 1100,
		641, 265,
		614, 194,
		574, 133,
		521, 80,
		449, 36,
		373, 18,
		262, 18,
		174, 44,
		110, 92,
		58, 158,
		34, 232,
		25, 308,
		21, 392,
		31, 468,
		55, 537,
		86, 589,
		86, 701,
		47, 720,
		29, 745,
		29, 888,
		204, 997,
		197, 1102,
		393, 1102,
		384, 994,
		552, 888,
		558, 742,
		538, 709,
		500, 696,
		498, 582,
		530, 537,
		554, 470,
		562, 394,
		562, 312,
		553, 234,
		529, 165,
		562, 208,
		581, 262,
		586, 313,
		582, 1100
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, pinball_body, 80));

	int upper_right[44] = {
		412, 115,
		458, 156,
		481, 199,
		495, 262,
		500, 329,
		500, 407,
		489, 475,
		459, 547,
		441, 550,
		407, 537,
		408, 508,
		421, 469,
		411, 447,
		318, 405,
		419, 401,
		417, 347,
		321, 348,
		416, 300,
		435, 280,
		437, 245,
		427, 218,
		411, 191
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, upper_right, 44));

	int  upper_left[50] = {
		171, 113,
		134, 146,
		112, 177,
		96, 224,
		88, 264,
		81, 303,
		82, 410,
		93, 470,
		107, 511,
		129, 547,
		140, 553,
		176, 538,
		176, 516,
		164, 481,
		168, 457,
		187, 445,
		263, 405,
		174, 399,
		175, 349,
		262, 346,
		165, 299,
		146, 278,
		145, 250,
		154, 221,
		171, 193
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, upper_left, 50));

	int left_triangle[6] = {
		178, 850,
		133, 750,
		128, 815
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, left_triangle, 6));

	int right_triangle[6] = {
		403, 851,
		453, 750,
		451, 819
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, right_triangle, 6));

	int right_thing[12] = {
		405, 929,
		392, 904,
		496, 835,
		498, 743,
		508, 743,
		509, 865
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, right_thing, 12));

	int left_thing[12] = {
		172, 927,
		188, 904,
		92, 837,
		85, 747,
		74, 747,
		75, 863
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, left_thing, 12));

	int bar1[8] = {
		238, 87,
		257, 87,
		259, 148,
		239, 148
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, bar1, 8));

	int bar2[8] = {
		327, 88,
		344, 88,
		344, 144,
		325, 144
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, bar2, 8));

	//Cloyster & Slowpoke
	pokemon.add(App->physics->CreateRectangle(160, 373, 36, 55, 1.2f, b2_staticBody))->data->listener = this;
	pokemon.getLast()->data->texture = App->textures->Load("pinball/slowpoke.png"); 
	pokemon.getLast()->data->fx = App->audio->LoadFx("pinball/bonus1.wav");
	pokemon.add(App->physics->CreateRectangle(430, 373, 40, 57, 1.2f, b2_staticBody))->data->listener = this;
	pokemon.getLast()->data->texture = App->textures->Load("pinball/big_cloyster.png"); 
	pokemon.getLast()->data->fx = App->audio->LoadFx("pinball/bonus1.wav");

	//PsyDuck
	PsyDuck = App->physics->CreateCircle(498, 644, 65, b2_staticBody, 1.0f);
	PsyDuck->listener = this;
	PsyDuck->texture = App->textures->Load("pinball/psyduck.png");
	PsyDuck->fx = App->audio->LoadFx("pinball/bonus3.wav");

	int boucing_rectangle_1[8] = {
		134, 761,
		142, 759,
		183, 836,
		172, 837
	};
	triangles.add(App->physics->CreatePinballChain(0, 0, boucing_rectangle_1, 8, 1.0f))->data->listener=this;
	triangles.getLast()->data->texture = App->textures->Load("pinball/left_triangle.png");
	triangles.getLast()->data->fx = App->audio->LoadFx("pinball/bonus2.wav");
	

	int boucing_rectangle_2[8] = {
		446, 763,
		436, 760,
		401, 830,
		409, 832
	};
	triangles.add(App->physics->CreatePinballChain(0, 0, boucing_rectangle_2, 8, 1.0f))->data->listener = this;
	triangles.getLast()->data->texture = App->textures->Load("pinball/right_triangle.png"); 
	triangles.getLast()->data->fx = App->audio->LoadFx("pinball/bonus2.wav");

	//Shellders
	shellders.add(App->physics->CreateCircle(215, 255, 40, b2_staticBody, 1.0f))->data->listener = this;
	shellders.getLast()->data->texture = App->textures->Load("pinball/jelly.png"); 
	shellders.getLast()->data->fx = App->audio->LoadFx("pinball/sound3.wav");
	shellders.add(App->physics->CreateCircle(291, 197, 40, b2_staticBody, 1.0f))->data->listener = this;
	shellders.getLast()->data->texture = App->textures->Load("pinball/jelly.png"); 
	shellders.getLast()->data->fx = App->audio->LoadFx("pinball/sound3.wav");
	shellders.add(App->physics->CreateCircle(368, 255, 40, b2_staticBody, 1.0f))->data->listener = this;
	shellders.getLast()->data->texture = App->textures->Load("pinball/jelly.png"); 
	shellders.getLast()->data->fx = App->audio->LoadFx("pinball/sound3.wav");



	pinball.add(App->physics->CreateCircle(292, 990, 2, b2_staticBody, 1.0f));

	int r_l[8] = {
		458, 546,
		412, 522,
		410, 540,
		442, 559
	};
	right_light = App->physics->CreatePinballChain(0, 0, r_l, 8);
	right_light->listener = this;
	right_light->texture = App->textures->Load("pinball/Zero.png");
	right_light->fx = App->audio->LoadFx("pinball/sound3.wav");

	int l_l[8] = {
		170, 513,
		122, 534,
		136, 550,
		176, 532
	};
	left_light = App->physics->CreatePinballChain(0, 0, r_l, 8);
	left_light->listener = this;
	left_light->texture = App->textures->Load("pinball/Zero.png");
	right_light->fx = App->audio->LoadFx("pinball/sound3.wav");

}

// TODO 8: Now just define collision callback for the circle and play bonus_fx audio
