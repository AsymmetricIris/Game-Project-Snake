#pragma once
#include <stdbool.h>
#include <math.h>
//#define collided(_1, _2) _Generic( (_1), BoxCollider:	_Generic((_2), BoxCollider: collided_bb, CircleCollider: collided_bc ), CircleCollider : _Generic((_2), CircleCollider: collided_cc, BoxCollider : collided_cb ) ) (_1, _2)
//#define checkColliderPos(x) _Generic( (x), BoxCollider: checkColliderPos_b, CircleCollider: checkColliderPos_c, default: checkColliderPos_c) (x)												

typedef struct CircleCollider {
	int radius, x, y;
}CircleCollider;

typedef struct BoxCollider {
	int x1, y1, x2, y2, sizeX, sizeY, centreX, centreY;
}BoxCollider;

void init_CircleCollider(CircleCollider* collider, int radius, int xPos, int yPos)
{
	collider->radius = radius;
	collider->x = xPos;
	collider->y = yPos;
}

CircleCollider createCircleCollider(int radius, int xPos, int yPos)
{
	CircleCollider collider;
	init_CircleCollider(&collider, radius, xPos, yPos);

	return collider;
}

void init_BoxCollider(BoxCollider* ptr_collider, int x1, int y1,
	int x2, int y2)
{
	
	ptr_collider->x1 = x1;
	ptr_collider->y1 = y1;
	ptr_collider->x2 = x2;
	ptr_collider->y2 = y2;
	ptr_collider->sizeX = x2 - x1;
	ptr_collider->sizeY = y1 - y2;
	ptr_collider->centreX = x1 + (ptr_collider->sizeX / 2);
	ptr_collider->centreY = y1 - (ptr_collider->sizeY / 2);
}

BoxCollider createBoxCollider(int xPos, int yPos, int x2, int y2)
{
	BoxCollider collider;
	init_BoxCollider(&collider, xPos, yPos, x2, y2);

	return collider;
}

void init_BodyBoxCollider(BoxCollider* ptr_collider, int xPos, int yPos,
	int sizeX, int sizeY)
{
	ptr_collider->sizeX = sizeX;
	ptr_collider->sizeY = sizeY;
	ptr_collider->x1 = xPos - (sizeX / 2);
	ptr_collider->y1 = yPos - (sizeY / 2);
	ptr_collider->x2 = xPos + (sizeX / 2);
	ptr_collider->y2 = yPos + (sizeY / 2);
	ptr_collider->centreX = xPos;
	ptr_collider->centreY = yPos;
}

BoxCollider createBodyBoxCollider(int xPos, int yPos, int sizeX, int sizeY)
{
	BoxCollider collider;
	init_BodyBoxCollider(&collider, xPos, yPos, sizeX, sizeY);

	return collider;
}

bool collided_cc(CircleCollider collider1, CircleCollider collider2)
{
	float distanceBetween, detectThreshold;
	distanceBetween = sqrt((collider1.x - collider2.x) * (collider1.x - collider2.x) + (collider1.y - collider2.y) * (collider1.y - collider2.y));
	detectThreshold = collider1.radius + collider2.radius;
	if (distanceBetween <= detectThreshold)
		return true;
	else
		return false;
}

void checkColliderPos_c(CircleCollider collider)
{
	printf("Circle collider x: %d\t y: %d\t radius: %d\n", collider.x, collider.y, collider.radius);
	printf("x1: %d\t y1: %d\t x2: %d\t y2: %d\n", collider.x - collider.radius,
		collider.y + collider.radius, collider.x + collider.radius,
		collider.y - collider.radius);
}

void checkColliderPos_b(BoxCollider collider)
{
	printf("Box collider x: %d\t y: %d\t sizeX: %d\t sizeY: %d\n", collider.centreX,
		collider.centreY, (collider.x2 - collider.x1), (collider.y2 - collider.y1));
	printf("x1: %d\t y1: %d\t x2: %d\t y2: %d\n", collider.x1, collider.y1,
		collider.x2, collider.y2);
}

// TODO - make more accurate
bool collided_cb(CircleCollider c_Collider, BoxCollider b_Collider)
{
	if (
		(
			// collider 1 inside collider 2
				// x1, y1 of circle's box
		(c_Collider.x - c_Collider.radius >= b_Collider.x1)
			&& (c_Collider.x - c_Collider.radius <= b_Collider.x2)
			&& (c_Collider.y + c_Collider.radius <= b_Collider.y1)
			&& (c_Collider.y + c_Collider.radius >= b_Collider.y2)
			) || (
				// x2, y2 of circle's box
			(c_Collider.x + c_Collider.radius >= b_Collider.x1)
				&& (c_Collider.x + c_Collider.radius <= b_Collider.x2)
				&& (c_Collider.y - c_Collider.radius <= b_Collider.y1)
				&& (c_Collider.y - c_Collider.radius >= b_Collider.y2)
				) || (
					// for collider 2 inside collider 1
						// is x1, y1 inside collider 1
				(b_Collider.x1 >= c_Collider.x - c_Collider.radius)
					&& (b_Collider.x1 <= c_Collider.x + c_Collider.radius)
					&& (b_Collider.y1 <= c_Collider.y + c_Collider.radius)
					&& (b_Collider.y1 >= c_Collider.y - c_Collider.radius)
					) || (
						// is x2, y2 inside collider 1
					(b_Collider.x2 >= c_Collider.x - c_Collider.radius)
						&& (b_Collider.x2 <= c_Collider.x + c_Collider.radius)
						&& (b_Collider.y2 <= c_Collider.y + c_Collider.radius)
						&& (b_Collider.y2 >= c_Collider.y - c_Collider.radius)
						)
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// TODO - make more accurate
bool collided_bc(BoxCollider b_Collider, CircleCollider c_Collider)
{
	if (
		(
			// collider 1 inside collider 2
				// x1, y1 of circle's box
		(c_Collider.x - c_Collider.radius >= b_Collider.x1)
			&& (c_Collider.x - c_Collider.radius <= b_Collider.x2)
			&& (c_Collider.y + c_Collider.radius >= b_Collider.y1)
			&& (c_Collider.y + c_Collider.radius <= b_Collider.y2)
			) || (
				// x2, y2 of circle's box
			(c_Collider.x + c_Collider.radius >= b_Collider.x1)
				&& (c_Collider.x + c_Collider.radius <= b_Collider.x2)
				&& (c_Collider.y - c_Collider.radius >= b_Collider.y1)
				&& (c_Collider.y - c_Collider.radius <= b_Collider.y2)
				) || (
					// for collider 2 inside collider 1
						// is x1, y1 inside collider 1
				(b_Collider.x1 >= c_Collider.x - c_Collider.radius)
					&& (b_Collider.x1 <= c_Collider.x + c_Collider.radius)
					&& (b_Collider.y1 >= c_Collider.y - c_Collider.radius)
					&& (b_Collider.y1 <= c_Collider.y + c_Collider.radius)
					) || (
						// is x2, y2 inside collider 1
					(b_Collider.x2 >= c_Collider.x - c_Collider.radius)
						&& (b_Collider.x2 <= c_Collider.x + c_Collider.radius)
						&& (b_Collider.y2 >= c_Collider.y - c_Collider.radius)
						&& (b_Collider.y2 <= c_Collider.y + c_Collider.radius)
						)

		)
	{
		//printf("Collided: True\n");
		return true;
	}		
	else
	{
		/*checkColliderPos_c(c_Collider);
		checkColliderPos_b(b_Collider);
		printf("Collided: False\n");*/
		return false;
	}
		
}

bool collided_bb(BoxCollider collider1, BoxCollider collider2)
{
	if (
		(
			// collider 1 inside collider 2
				// x1, y1 of circle's box
		(collider1.x1 >= collider2.x1)
			&& (collider1.x1 <= collider2.x2)
			&& (collider1.y1 >= collider2.y2)
			&& (collider1.y1 <= collider2.y1)
			) || (
				// x2, y2 of circle's box
			(collider1.x2 >= collider2.x1)
				&& (collider1.x2 <= collider2.x2)
				&& (collider1.y2 >= collider2.y2)
				&& (collider1.y2 <= collider2.y1)
				) || (
					// for collider 2 inside collider 1
						// is x1, y1 inside collider 1
				(collider2.x1 >= collider1.x1)
					&& (collider2.x1 <= collider1.x2)
					&& (collider2.y1 >= collider1.y2)
					&& (collider2.y1 <= collider1.y1)
					) || (
						// is x2, y2 inside collider 1
					(collider2.x2 >= collider1.x1)
						&& (collider2.x2 <= collider1.x2)
						&& (collider2.y2 >= collider1.y2)
						&& (collider2.y2 <= collider1.y1)
						)
		)
	{
		printf("Collided: True\n");
		return true;
	}
	else
	{
		printf("Collided: False\n");
		return false;
	}
}

