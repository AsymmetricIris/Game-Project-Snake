#pragma once
#include "RigidBody.h"
#include "colliders.h"

typedef struct Fruit
{
	int radius;
	RigidBody body;
}Fruit;

void init_fruit(Fruit* ptr_fruit, int radius, int xPos, int yPos)
{
	ptr_fruit->radius = radius;
	ptr_fruit->body = createRigidBody(xPos, yPos);
	ptr_fruit->body.collider_c = createCircleCollider(ptr_fruit->radius, ptr_fruit->body.x, ptr_fruit->body.y);
}

Fruit createFruit(int radius, int xPos, int yPos)
{
	Fruit fruit;
	init_fruit(&fruit, radius, xPos, yPos);

	return fruit;
}

void setFruitPos(Fruit* ptr_fruit, int xPos, int yPos)
{
	ptr_fruit->body.x = xPos;
	ptr_fruit->body.y = yPos;
	updateCollider_c(&(ptr_fruit->body));
}