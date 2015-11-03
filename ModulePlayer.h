#pragma once
#include "Module.h"
#include "ModuleTextures.h"
#include "Globals.h"
#include "p2Point.h"

struct Object
{
	Object(){ body = NULL; texture = NULL; m_contacting = false; }
	PhysBody* body;
	SDL_Texture* texture;
	bool m_contacting;
	void startContact(){ m_contacting = true; }
	void endContact(){ m_contacting = false; }
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	uint push_value = 0;
	uint lives = 3;
	uint score = 000000000;
	Object Ball;
	Object r_flipper;
	Object l_flipper;
	Object pusher;

	PhysBody* r_flipper_anchor;
	PhysBody* l_flipper_anchor;
	PhysBody* pusher_anchor;
};