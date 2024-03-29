// BGIFirstTest.c : Example graphic application
//

// S - ub-par,
// N - on-linux
// A - PI that
// K - ills
// E - njoyment

//TODO -
//	Snake
//Implement head and tail segments as separate structs
//Use concatenation to increase tail length instead of a defined, invisible length with 'exists' fields
//		Research if arrays can be concatenated
//Implement hitboxes
//		store hitboxes in an 2d array - hitMap
//		move hitboxes according to the object they are attached to
//		if a hitbox tries to set an index as a part of itself when that array is already part of another hitbox, return a collision
//		note: ensure loop does not lead to hitboxes endlessly overwriting each other
//		note: algorithm becomes very resoource intensive with large hitboxes
//		algorithm
//			store an array of hitboxes vertices with there addresses
//			compare the positions of each vertext of each hitbox
//			when any vertex of one hitbox is inside the coordinates of another hitbox, a collision is detected
//	Animations
//Try the vanilla BGI library
//Try importing images
//Try using fill functions instead of line-drawing ones
//Make game pretty
//	Score table
//display current score
//	itoa functions are being difficult
//add high scores as an array of 100 scores
//delete scores from the scores file when there are more than 100 of them
//order the scores array to write back to the file
//	instead of a sorting algorithm, can use binary search to find the numbers a new score fits between
//		algorithm
//			binary search to find place where either the score fits between two consecutive numbers or where it is equal
//			when a score is equal, the algorithm will end
//			when a score fits between two numbers
//				copy all scores below the current score - except the lowest of them all - into a new array of lower scores
//				copy all the scores higher than the current score into an array of higher scores
//				combine the two arrays
//			when a score is lower than everything else but higher than the lowest score
//				the lowest score is overwritten

//DONE
//Fix disappearing tail segments
//Make segments follow the head's path
	//Calculate row and col from x and y coords


#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>   
#include <stdbool.h>
#include <string.h>
#include <errno.h>

//definitions for the pathMap values
#define PATH_N 1
#define PATH_NE 2
#define PATH_E 3
#define PATH_SE 4
#define PATH_S 5
#define PATH_SW 6
#define PATH_W 7
#define PATH_NW 8
#define PATH_EMPTY 0
#define ROWS 501
#define COLS 501

#define getLength(x) (sizeof(x) / sizeof(x[0]))

void drawAvatar(int x, int y);
void drawObstacle(int x, int y);
void populateIntArray(int* arr, int cols, int rows, int val);
void printArray(int* arr, int cols, int rows);
void setArrVal(int* ptr_arr, int val, int row, int col, int array_rows, int array_cols);
int getArrVal(int* ptr_arr, int row, int col, int array_rows, int array_cols);



struct SnakeSegment
{
	int x, y, xVelocity, yVelocity, distanceFromHead;
	bool exists;
};

void main(void)
{
	// Initialise graphic window
	int gd = DETECT, gm = 0;
	int arenaOffset = 50;
	int arenaX1 = arenaOffset;
	int arenaY1 = arenaOffset;
	int arenaX2 = 500;
	int arenaY2 = 500;
	int tailCurrentLen = 0;
	int pathMap[ROWS][COLS];
	int* ptr_pathMap = &pathMap;

	populateIntArray(&pathMap, ROWS, COLS, 0);

	// Debug - testing 2DArray functions
	//setArrVal(&pathMap, 4, 500, 500, 501, 501);
	//printf("Value at 5, 5: %d\n", getArrVal(&pathMap, 5, 5, 501, 501) );
	//printArray(&pathMap, ROWS, COLS);

	struct SnakeSegment segmentArr[10];

	int snakeSpeed = 5;
	segmentArr[0].x = 100;
	segmentArr[0].y = 100;
	segmentArr[0].exists = true;
	segmentArr[0].xVelocity = snakeSpeed;
	segmentArr[0].yVelocity = 0;


	int xo = 200;
	int yo = 300;

	initgraph(&gd, &gm, "");

	int c = 0; // for input from keyboard

	// read high score from data file
	FILE *scoreInput, *scoreOutput;
	int score = 0;
	int highScore;
	errno_t err;
	
	if ( (err = fopen_s(&scoreInput, "scores.dat", "r")) != 0 )
	{
		fprintf(stderr, "ERROR: can't open file\n");
		return;
	}
	
	if ((fscanf_s(scoreInput, "%d\n", &highScore)))
	{
		printf("Score: %d\n", score);
		printf("High score: %d\n", highScore);
		fclose(scoreInput);
	}

	//save high score to data file
	if ( (err = fopen_s(&scoreOutput, "scores.dat", "a")) != 0 )
	{
		fprintf(stderr, "ERROR: can't open file\n");
		return;
	}

	int i = 0;
	int distance;
	
	/*int snakeXVelocity = snakeSpeed;
	int snakeYVelocity = 0;*/

	do {
		setactivepage(i % 2);
		clearviewport();
		line(arenaX1, arenaY1, arenaX1, arenaY2);
		line(arenaX1, arenaY2, arenaX2, arenaY2);
		line(arenaX2, arenaY2, arenaX2, arenaY1);
		line(arenaX2, arenaY1, arenaX1, arenaY1);

		drawObstacle(xo, yo);
		// draw circle in current position
		for (int i = 0; i <= tailCurrentLen; i++)
		{
			drawAvatar(segmentArr[i].x, segmentArr[i].y);
		}

		//drawAvatar(x2, 150);
		// update position of circle of a fixed offset
		setvisualpage(i % 2);
		i++;

		for (int i = 0; i < getLength(segmentArr); i++)
		{
			if (segmentArr[i].exists)
			{
				segmentArr[i].x += segmentArr[i].xVelocity;   // try different values for the offset, e.g. x+=5 or x+=10
				segmentArr[i].y += segmentArr[i].yVelocity;

				// debug
				/*printf("Segment %d-X: %d\n", i, segmentArr[i].x);
				printf("Segment %d-Y: %d\n", i, segmentArr[i].y);
				printf("Segment %d-X velocity: %d\n", i, segmentArr[i].xVelocity);
				printf("Segment %d-Y: velocity %d\n", i, segmentArr[i].yVelocity);*/
			}
		}

		// Send the serpent to the opposite side of the arena if it touches a wall
		// TODO - make more accurate
		for (int segment = 0; segment <= tailCurrentLen; segment++)
		{
			if (segmentArr[segment].x > arenaX2)
				segmentArr[segment].x = arenaX1;
			else if (segmentArr[segment].x < arenaX1)
				segmentArr[segment].x = arenaX2;

			if (segmentArr[segment].y > arenaY2)
				segmentArr[segment].y = arenaY1;
			else if (segmentArr[segment].y < arenaY1)
				segmentArr[segment].y = arenaY2;
		}

		distanceToGoodie = (segmentArr[0].x - xo) * (segmentArr[0].x - xo) + (segmentArr[0].y - yo) * (segmentArr[0].y - yo);
		//printf("Distance between %d and %d is %d\n", segmentArr[0].x, xo, distance);

		if (distanceToGoodie < 900) {
			printf("COLLISION!!!\n");
			xo = arenaX1 + (rand() % (arenaX2 - 50));
			yo = arenaY1 + (rand() % (arenaY2 - 50));
			score++;
			tailCurrentLen++;
			segmentArr[tailCurrentLen].exists = true;

			// debug
			/*printf("Tail length: %d\n", tailCurrentLen);
			printf("Tail length - 1: %d\n", (tailCurrentLen - 1));
			printf("Tail exists: %d\n", segmentArr[tailCurrentLen].exists);
			printf("Previous link x vel: %d\n", segmentArr[tailCurrentLen - 1].xVelocity);
			printf("Previous link y vel: %d\n", segmentArr[tailCurrentLen - 1].yVelocity);*/

			segmentArr[tailCurrentLen].xVelocity = segmentArr[tailCurrentLen - 1].xVelocity;
			segmentArr[tailCurrentLen].yVelocity = segmentArr[tailCurrentLen - 1].yVelocity;

			if (segmentArr[tailCurrentLen - 1].xVelocity == snakeSpeed)
			{
				segmentArr[tailCurrentLen].x = segmentArr[tailCurrentLen - 1].x - 40;
			}
			else if (segmentArr[tailCurrentLen - 1].xVelocity == -snakeSpeed)
			{
				segmentArr[tailCurrentLen].x = segmentArr[tailCurrentLen - 1].x + 40;
			}
			else
			{
				segmentArr[tailCurrentLen].x = segmentArr[tailCurrentLen - 1].x;
			}

			if (segmentArr[(tailCurrentLen - 1)].yVelocity == snakeSpeed)
			{
				segmentArr[tailCurrentLen].y = segmentArr[tailCurrentLen - 1].y - 40;
			} 
			else if (segmentArr[(tailCurrentLen - 1)].yVelocity == -snakeSpeed)
			{
				segmentArr[tailCurrentLen].y = segmentArr[tailCurrentLen - 1].y + 40;
			}
			else
			{
				segmentArr[tailCurrentLen].y = segmentArr[tailCurrentLen - 1].y;
			}

			printf("Score: %d\n", score);

		}

		for (int i = 1; i < tailCurrentLen; i++)
		{
			segmentArr[i].distanceFromHead = segmentArr[i].x - segmentArr[0].x)* (segmentArr[i].x - segmentArr[0].x) + segmentArr[i].y - segmentArr[0].y)* (segmentArr[i].y - segmentArr[0].y);
			printf("Tail %d distance to head: %d\n", i, segmentArr[i).distanceToHead);
		}

		distanceToGoodie = (segmentArr[0].x - xo) * (segmentArr[0].x - xo) + (segmentArr[0].y - yo) * (segmentArr[0].y - yo);

		delay(100); // wait 50 ms , try different values for this delay

		// change head's velocity on user input
		if (_kbhit()) { // see http://www.programmingsimplified.com/c/conio.h/kbhit
			c = _getch();
			switch (c) {
			case 119: //key w
				segmentArr[0].yVelocity = -snakeSpeed;
				segmentArr[0].xVelocity = 0;
				setArrVal(&pathMap, PATH_N, (segmentArr[0].y - arenaOffset ), ( segmentArr[0].x - arenaOffset), ROWS, COLS);
				//printf("North marker placed at:  (%d, %d)\n", (segmentArr[0].y - arenaOffset), (segmentArr[0].x - arenaOffset) );
				break;
			case 115: //key s
				segmentArr[0].yVelocity = snakeSpeed;
				segmentArr[0].xVelocity = 0;
				setArrVal(&pathMap, PATH_S, (segmentArr[0].y - arenaOffset), (segmentArr[0].x - arenaOffset), ROWS, COLS);
				//printf("South marker placed at:  (%d, %d)\n", (segmentArr[0].y - arenaOffset), (segmentArr[0].x - arenaOffset) );
				break;
			case 97: //key a
				segmentArr[0].xVelocity = -snakeSpeed;
				segmentArr[0].yVelocity = 0;
				setArrVal(&pathMap, PATH_W, (segmentArr[0].y - arenaOffset), (segmentArr[0].x - arenaOffset), ROWS, COLS);
				//printf("West marker placed at:  (%d, %d)\n", (segmentArr[0].y - arenaOffset), (segmentArr[0].x - arenaOffset) );
				break;
			case 100: //key d
				segmentArr[0].xVelocity = snakeSpeed;
				segmentArr[0].yVelocity = 0;
				setArrVal(&pathMap, PATH_E, (segmentArr[0].y - arenaOffset), (segmentArr[0].x - arenaOffset), ROWS, COLS);
				//printf("East marker placed at:  (%d, %d)\n", (segmentArr[0].y - arenaOffset), (segmentArr[0].x - arenaOffset) );
				break;
			}
			//printf("key pressed is %d", c);
		}

		// change segment's velocity to follow the path of prev ious segments
		// trigger a change when the tail segment's coordinate on the path map has a directional flag
		for (int segmentNum = 0; segmentNum <= tailCurrentLen; segmentNum++)
		{
			// find direction change when the segment crosses a point where the snake has changed direction
			int xPos = segmentArr[segmentNum].x - arenaOffset;
			int yPos = segmentArr[segmentNum].y - arenaOffset;
			// the direction the path map's
			int pathVal = getArrVal(&pathMap, yPos, xPos, ROWS, COLS);

			if (segmentNum > 0)
			{

				switch (pathVal) {
				case PATH_EMPTY:
					//printf("No change\n");
					break;
				case PATH_S: //down
					//printf("This should turn South\n");
					segmentArr[segmentNum].yVelocity = snakeSpeed;
					segmentArr[segmentNum].xVelocity = 0;
					break;
				case PATH_N: //upward
					segmentArr[segmentNum].yVelocity = -snakeSpeed;
					segmentArr[segmentNum].xVelocity = 0;
					break;
				case PATH_W: //to the left
					segmentArr[segmentNum].xVelocity = -snakeSpeed;
					segmentArr[segmentNum].yVelocity = 0;
					break;
				case PATH_E: //to the right
					segmentArr[segmentNum].xVelocity = snakeSpeed;
					segmentArr[segmentNum].yVelocity = 0;
					break;
				default:
					break;
				}
			}
			if (segmentNum == tailCurrentLen)
			{
				setArrVal(&pathMap, PATH_EMPTY, yPos, xPos, ROWS, COLS); // clear direction change marker from the map
				
				//debug
				//printf("Deleting segment number %d at (%d, %d)\n", segmentNum, xPos, yPos);
				printf("============================================================================\n")
			}
		}

	} while (c != KEY_ESCAPE);

	if (fprintf(scoreOutput, "%d\n", score) == 1)
	{
		printf("score %d written\n", score);
	}

	if (scoreOutput)
	{
		fclose(scoreOutput);
	}
}

void drawAvatar(int x, int y)
{
	circle(x, y, 20);
}

void drawObstacle(int x, int y)
{
	circle(x, y, 10);
}

void populateIntArray(int* arr, int cols, int rows, int val)
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			*(arr + (row * cols) + col) = val;
		}
	}
}

void printArray(int* ptr_arr, int cols, int rows)
{
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			printf("Row:%d\t Col: %d\t %d\n", row, col, *(ptr_arr + (row * cols) + col));
		}
	}
}

void setArrVal(int* ptr_arr, int val, int row, int col, int array_rows, int array_cols)
{
	// TODO - validate array length with address
	*(ptr_arr + (row * array_cols) + col) = val;
}

int getArrVal(int* ptr_arr, int row, int col, int array_rows, int array_cols)
{
	// TODO - validate array length with address
	return *(ptr_arr + (row * array_cols) + col);
}

//void setArrVal(int* ptr_arr, int val, int row, int col, int array_rows, int array_cols);
//{
//	*(ptr_arr + (row * cols) + col) = val;
//}
//
//int getArrVal(int* ptr_arr, int row, int col, int array_rows, int array_cols);
//{
//	return *(ptr_arr + (row * cols) + col);
//}