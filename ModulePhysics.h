#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:

	PhysBody();
	~PhysBody();

	void GetPosition(int& x, int &y) const;
	void SetPosition(int x, int y);
	float GetRotation() const;
	void SetLinearSpeed(int x, int y);
	void SetAngularSpeed(float speed);
	void Push(float x, float y);
	void Turn(int degrees);
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	SDL_Texture* texture;
	uint fx;

	SDL_Rect rect;
	b2BodyType type;
	float hit_timer;
};


class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type, float restitution = 0.0f);
	PhysBody* CreateRectangle(int x, int y, int width, int height, float restitution, b2BodyType type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreatePolygon(const SDL_Rect& rect, int* points, uint count, b2BodyType type, float density, float restitution, bool isSensor);
	PhysBody* CreateChain(int x, int y, int* points, int size);
	PhysBody* CreatePinballChain(int x, int y, int* points, int size, float restitution = 0.0f);

	void CreateRevoluteJoint(PhysBody* body_1, PhysBody* body_2, int x_pivot_1 = 0, int y_pivot_1 = 0, int x_pivot_2 = 0, int y_pivot_2 = 0, int max_angle = INT_MAX, int min_angle = INT_MIN);
	void CreateLineJoint(PhysBody* body_1, PhysBody* body_2, int x_pivot_1 = 0, int y_pivot_1 = 0, int x_pivot_2 = 0, int y_pivot_2 = 0, float frequency = 15.0f, float damping = 0.5f);


	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);


private:

	bool debug;
	p2List<PhysBody*> bodies;
	b2World* world;
};