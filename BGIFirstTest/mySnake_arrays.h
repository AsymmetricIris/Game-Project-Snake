#pragma once
#define getLength(x) (sizeof(x) / sizeof(x[0]))

struct SnakeSegment
{
	int x, y, xVelocity, yVelocity, distanceToHead;
	bool exists;
};

struct *snakeSegment[] concatSegment(struct SnakeSegment snakeSegmentArray[], struct SnakeSegment snakeSegment)
{
	int segArrSize = getLength(snakeSegmentArray);
	snakeSegment[segArrSize + 1];
	return 
}