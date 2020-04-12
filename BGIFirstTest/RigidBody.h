#pragma once
#include "colliders.h"

typedef struct RigidBody {
	int x, y;
	CircleCollider collider_c;
	BoxCollider collider_b;
}RigidBody;

void init_RigidBody(RigidBody* body, int xPos, int yPos)
{
	body->x = xPos;
	body->y = yPos;
}

RigidBody createRigidBody(int xPos, int yPos)
{
	RigidBody body;
	init_RigidBody(&body, xPos, yPos);
	return body;
}

void updateCollider_c(RigidBody* ptr_body)
{
	ptr_body->collider_c.x = ptr_body->x;
	ptr_body->collider_c.y = ptr_body->y;
}

void updateCollider_b(RigidBody* ptr_body)
{
	int halfX = (ptr_body->collider_b.sizeX / 2);
	int halfY = (ptr_body->collider_b.sizeY / 2);
	ptr_body->collider_b.x1 = ptr_body->x - halfX;
	ptr_body->collider_b.y1 = ptr_body->y - halfY;
	ptr_body->collider_b.x2 = ptr_body->x + halfX;
	ptr_body->collider_b.y2 = ptr_body->y + halfY;
	ptr_body->collider_b.centreX = ptr_body->x;
	ptr_body->collider_b.centreY = ptr_body->y;
}

void addCollider_c(RigidBody* ptr_body, int radius)
{
	CircleCollider collider_c = createCircleCollider(radius, 0, 0);
	ptr_body->collider_c = collider_c;
	updateCollider_c(ptr_body);
}

void addCollider_b(RigidBody* ptr_body, int sizeX, int sizeY)
{
	BoxCollider collider_b = createBoxCollider(ptr_body->x, ptr_body->y, sizeX, sizeY);
	ptr_body->collider_b = collider_b;
	updateCollider_b(ptr_body);
}