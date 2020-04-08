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
	SnakeSegment* segments;
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

	return snakeSegment;
}

void init_segments(Snake* snake, int xPos, int yPos, int xVelocity, int yVelocity)
{
	snake->segments = (SnakeSegment*) calloc(1, sizeof(SnakeSegment));
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

void addNewSegment(Snake* snake)
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

	newSnakeSegment = createSnakeSegment(newSegmentX, newSegmentY, 
		snake->segments[prevSegIndex].xVelocity,
		snake->segments[prevSegIndex].yVelocity);

	SnakeSegment* newSegments = (SnakeSegment*) calloc(newSnakeSize, sizeof(SnakeSegment));


	for (int segment = 0; segment < snake->size; segment++)
	{
		newSegments[segment] = snake->segments[segment];
	}

	for (int segment = snake->size; segment < newSnakeSize; segment++)
	{
		newSegments[segment] = newSnakeSegment;
	}

	//debug
	printf("Snake segment array size: %d\n", snake->size);

	free(snake->segments);
	snake->segments = newSegments;
	snake->size++;
}

void new_addNewSegment(Snake* snake)
{
	int newSnakeSize = snake->size + 1;
	int prevSegNum = snake->size - 1;
	SnakeSegment newSnakeSegment = createSnakeSegment(snake->segments[prevSegNum].x - 20,
		snake->segments[prevSegNum].y - 20, snake->segments[prevSegNum].xVelocity,
		snake->segments[prevSegNum].yVelocity);

	SnakeSegment* ptr_newSegments = malloc((newSnakeSize) * sizeof(newSnakeSegment));


	for (int segment = 0; segment < snake->size; segment++)
	{
		*(ptr_newSegments + segment) = snake->segments[segment];
	}

	for (int segment = snake->size; segment < newSnakeSize; segment++)
	{
		*(ptr_newSegments + segment) = newSnakeSegment;
	}

	//TODO - make first print check return amount of elements in the array
	//debug
	int newSegmentIndex = snake->size - 1;
	printf("New segment %d x: %d\n", snake->size, snake->segments[newSegmentIndex].x);

	//free(snake->segments);
	snake->segments = ptr_newSegments;
	snake->size++;
	printf("New array size: %d\n", snake->size);
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