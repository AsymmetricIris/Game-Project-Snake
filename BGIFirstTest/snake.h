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
}Snake;

Snake* addSegment(Snake snakeSnake, SnakeSegment newSnakeSegment)
{

	int snake.size = snakeSnake.size;
	int newSnakeSize = snake.size + 1;


	SnakeSegment* ptr_newSnake = malloc((snake.size + 1) * sizeof(newSnakeSegment));


	for (int segment = 0; segment < snake.size; segment++)
	{
		*(ptr_newSnake + segment) = snakeSnake.segments[segment];
	}

	for (int segment = snake.size; segment < newSnakeSize; segment++)
	{
		*(ptr_newSnake + segment) = newSnakeSegment;
	}

	printf("Input array size: %d\n", sizeof(snakeSnake));
	printf("Snake segment array size: %d\n", snake.size);
	printf("New array size: %d\n", newSnakeSize);
	return ptr_newSnake;
}

Snake* addNewSegment(Snake snake)
{
	int newSnakeSize = snake.size + 1;


	SnakeSegment* ptr_newSnake = malloc((newSnakeSize) * sizeof(newSnakeSegment));


	for (int segment = 0; segment < snake.size; segment++)
	{
		*(ptr_newSnake + segment) = snake.segments[segment];
	}

	for (int segment = snake.size; segment < newSnakeSize; segment++)
	{
		*(ptr_newSnake + segment) = newSnakeSegment;
	}

	printf("Input array size: %d\n", sizeof(snake));
	printf("Snake segment array size: %d\n", snake.size);
	printf("New array size: %d\n", newSnakeSize);
	return ptr_newSnake;
}