#pragma once

typedef struct CircleCollider {
	int radius, x, y;
	void* body;
}CircleCollider;

typedef struct BoxCollider {
	int radius, x1, y1, x2, y2;
	void* body;
}BoxCollider;

void init_CircleCollider(BoxCollider* collider, int radius, int xPos, int yPos, void* body)
{
	collider->radius = radius;
	collider->x1 = xPos;
	collider->y = yPos;
	collider->body = body;
}

CircleCollider createCircleCollider(int radius, int xPos, int yPos, void* body)
{
	CircleCollider collider;
	init_CircleCollider(&collider, radius, xPos, yPos, body);

	return collider;
}