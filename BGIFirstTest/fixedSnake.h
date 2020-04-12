#pragma once
#include "RigidBody.h"
#include "colliders.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define getLength(x) (sizeof(x) / sizeof(x[0]))

typedef struct SnakeSegment
{
	int xVelocity, yVelocity, distanceToHead;
	bool exists;
	RigidBody body;
}SnakeSegment;

typedef struct Snake
{
	SnakeSegment segments[100];
	int size;
	int speed;
}Snake;

void init_SnakeSegment(SnakeSegment* snakeSegment, int xPos, int yPos, int xVelocity, int yVelocity)
{
	RigidBody segmentBody = createRigidBody(xPos, yPos);
	snakeSegment->body = segmentBody;
	snakeSegment->xVelocity = xVelocity;
	snakeSegment->yVelocity = yVelocity;
}

SnakeSegment createSnakeSegment(int xPos, int yPos, int xVelocity, int yVelocity)
{
	SnakeSegment snakeSegment;
	init_SnakeSegment(&snakeSegment, xPos, yPos, xVelocity, yVelocity);
	snakeSegment.exists = true;

	return snakeSegment;
}

void init_segments(Snake* snake, int xPos, int yPos, int xVelocity, int yVelocity)
{
	snake->segments[0] = createSnakeSegment(xPos, yPos, xVelocity, yVelocity);
	snake->size = 1;
}

Snake createSnake(int xPos, int yPos, int snakeSpeed)
{
	Snake snake;
	snake.speed = snakeSpeed;
	init_segments(&snake, xPos, yPos, 0, snake.speed);

	return snake;
}

//	====================================================
//	Add a segment to the segment array
//void addSegment(Snake* snake, SnakeSegment newSegment)

int addNewSegment(Snake* snake)
{
	//debug
	printf("Body before - x: %d\t y: %d\n", snake->segments[0].body.x,
		snake->segments[0].body.y);
	printf("Pointer before - x: %d\t y: %d\n", snake->segments[0].body.x,
		snake->segments[0].body.y);
	printf("End body before - x: %d\t y: %d\n", snake->segments[snake->size - 1].body.x,
		snake->segments[snake->size - 1].body.y);
	printf("End pointer before - x: %d\t y: %d\n", snake->segments[snake->size - 1].body.x,
		snake->segments[snake->size - 1].body.y);

	if (snake->size < 100)
	{
		int newSnakeSize = snake->size + 1;
		int prevSegIndex = snake->size - 1;

		//debug
		int newSegIndex = snake->size;

		SnakeSegment newSnakeSegment;
		int newSegmentX;
		int newSegmentY;

		if (snake->segments[prevSegIndex].xVelocity == snake->speed)
		{
			newSegmentX = snake->segments[prevSegIndex].body.x - 40;
		}
		else if (snake->segments[prevSegIndex].xVelocity == -snake->speed)
		{
			newSegmentX = snake->segments[prevSegIndex].body.x + 40;
		}
		else
		{
			newSegmentX = snake->segments[prevSegIndex].body.x;
		}

		if (snake->segments[(prevSegIndex)].yVelocity == snake->speed)
		{
			newSegmentY = snake->segments[prevSegIndex].body.y - 40;
		}
		else if (snake->segments[(prevSegIndex)].yVelocity == -snake->speed)
		{
			newSegmentY = snake->segments[prevSegIndex].body.y + 40;
		}
		else
		{
			newSegmentY = snake->segments[prevSegIndex].body.y;
		}

		snake->segments[newSegIndex] = createSnakeSegment(newSegmentX, newSegmentY,
			snake->segments[prevSegIndex].xVelocity,
			snake->segments[prevSegIndex].yVelocity);

		snake->size++;

		//debug
		printf("Body after - x: %d\t y: %d\n", snake->segments[0].body.x,
			snake->segments[0].body.y);
		printf("Pointer after - x: %d\t y: %d\n", snake->segments[0].body.x,
			snake->segments[0].body.y);
		printf("End body after - x: %d\t y: %d\n", snake->segments[snake->size - 1].body.x,
			snake->segments[snake->size - 1].body.y);
		printf("End pointer after - x: %d\t y: %d\n", snake->segments[snake->size - 1].body.x,
			snake->segments[snake->size - 1].body.y);

		return 1;
	}
	else
		return 0;
}

void updateSnakePos(Snake* snake)
{
	for (int segment = 0; segment < snake->size; segment++)
	{
		snake->segments[segment].body.x += snake->segments[segment].xVelocity;   // try different values for the offset, e.g. x+=5 or x+=10
		snake->segments[segment].body.y += snake->segments[segment].yVelocity;
		updateCollider_c(&(snake->segments[segment].body));
	}
}

void verbose_updateSnakePos(Snake* snake)
{
	//debug
	printf("Body before - x: %d\t y: %d\n", snake->segments[0].body.x,
		snake->segments[0].body.y);
	printf("Pointer before - x: %d\t y: %d\n", snake->segments[0].body.x,
		snake->segments[0].body.y);
	printf("End body after - x: %d\t y: %d\n", snake->segments[snake->size - 1].body.x,
		snake->segments[snake->size - 1].body.y);
	printf("End pointer after - x: %d\t y: %d\n", snake->segments[snake->size - 1].body.x,
		snake->segments[snake->size - 1].body.y);

	for (int segment = 0; segment < snake->size; segment++)
	{
		snake->segments[segment].body.x += snake->segments[segment].xVelocity;   // try different values for the offset, e.g. x+=5 or x+=10
		snake->segments[segment].body.y += snake->segments[segment].yVelocity;
		updateCollider_c(&(snake->segments[segment].body));
	}

	//debug
	printf("Body after - x: %d\t y: %d\n", snake->segments[0].body.x,
		snake->segments[0].body.y);
	printf("Pointer after - x: %d\t y: %d\n", snake->segments[0].body.x,
		snake->segments[0].body.y);
	printf("End body after - x: %d\t y: %d\n", snake->segments[snake->size - 1].body.x,
		snake->segments[snake->size - 1].body.y);
	printf("End pointer after - x: %d\t y: %d\n", snake->segments[snake->size - 1].body.x,
		snake->segments[snake->size - 1].body.y);
}

//debug
void checkSnakePos(Snake* snake)
{
	for (int segment = 0; segment < snake->size; segment++)
	{
		printf("Snake segment %d x: %d\t", segment, snake->segments[segment].body.x);
		printf("y: %d\n", snake->segments[segment].body.y);
	}
}