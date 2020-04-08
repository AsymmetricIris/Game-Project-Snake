#pragma once
#include <stdlib.h>
#include <stdbool.h>
#define getLength(x) (sizeof(x) / sizeof(x[0]))

typedef struct SnakeSegment
{
	int x, y, xVelocity, yVelocity, distanceToHead;
	bool exists;
}SnakeSegment;

typedef struct Snake
{
	SnakeSegment segments[100];
	int size;
	int speed;
}Snake;

void init_SnakeSegment(SnakeSegment* snakeSegment, int xPos, int yPos, int xVelocity, int yVelocity)
{
	snakeSegment->x = xPos;
	snakeSegment->y = yPos;
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
	init_segments(&snake, xPos, yPos, snake.speed, 0);

	return snake;
}

//	====================================================
//	Add a segment to the segment array
//void addSegment(Snake* snake, SnakeSegment newSegment)

int addNewSegment(Snake* snake)
{
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
			newSegmentX = snake->segments[prevSegIndex].x - 40;
		}
		else if (snake->segments[prevSegIndex].xVelocity == -snake->speed)
		{
			newSegmentX = snake->segments[prevSegIndex].x + 40;
		}
		else
		{
			newSegmentX = snake->segments[prevSegIndex].x;
		}

		if (snake->segments[(prevSegIndex)].yVelocity == snake->speed)
		{
			newSegmentY = snake->segments[prevSegIndex].y - 40;
		}
		else if (snake->segments[(prevSegIndex)].yVelocity == -snake->speed)
		{
			newSegmentY = snake->segments[prevSegIndex].y + 40;
		}
		else
		{
			newSegmentY = snake->segments[prevSegIndex].y;
		}

		snake->segments[newSegIndex] = createSnakeSegment(newSegmentX, newSegmentY,
			snake->segments[prevSegIndex].xVelocity,
			snake->segments[prevSegIndex].yVelocity);

		snake->size++;
		return 1;
	}
	else
		return 0;
}

void updateSnakePos(Snake* snake)
{
	for (int segment = 0; segment < snake->size; segment++)
	{
		snake->segments[segment].x += snake->segments[segment].xVelocity;   // try different values for the offset, e.g. x+=5 or x+=10
		snake->segments[segment].y += snake->segments[segment].yVelocity;
	}
}

//debug
void checkSnakePos(Snake* snake)
{
	for (int segment = 0; segment < snake->size; segment++)
	{
		printf("Snake segment %d x: %d\t", segment, snake->segments[segment].x);
		printf("Snake segment %d y: %d\n", segment, snake->segments[segment].y);
	}
}