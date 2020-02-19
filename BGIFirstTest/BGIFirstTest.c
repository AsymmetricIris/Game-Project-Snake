// BGIFirstTest.c : Example graphic application
//


#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>   
#include <stdbool.h>
#include <string.h>
#include <errno.h>


#define getLength(x) (sizeof(x) / sizeof(x[0]))

void drawAvatar(int x, int y);

void drawObstacle(int x, int y);

void main(void)
{
	// Initialise graphic window
	int gd = DETECT, gm = 0;
	int arenaX1 = 50;
	int arenaY1 = 50;
	int arenaX2 = 500;
	int arenaY2 = 500;
	int tailCurrentLen = 0;
	char pathMap[550][550][2];

	struct SnakeSegment
	{
		int x, y, xVelocity, yVelocity;
		bool exists;
	};

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

	FILE *scoreInput, *scoreOutput;
	int score = 0;
	errno_t err;
	printf("Before the IO bits\n");
	if ( (err = fopen_s(&scoreInput, "scores.dat", "r")) != 0 )
	{
		fprintf(stderr, "ERROR: can't open file\n");
		return;
	}
	
	printf("After read 1\n");
	if ( (fscanf_s(scoreInput, "%d\n", &score)) )
		printf("Score: %d\n", score);
	printf("After writing\n");
	fclose(scoreInput);

	if ( (err = fopen_s(&scoreOutput, "scores.dat", "a")) != 0 )
	{
		fprintf(stderr, "ERROR: can't open file\n");
		return;
	}
	printf("After read 2\n");

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
			}
		}

		// Send the serpent to the opposite side of the arena if it touches a wall
		// TODO - make more accurate
		if (segmentArr[0].x > arenaX2)
			segmentArr[0].x = arenaX1;
		else if (segmentArr[0].x < arenaX1)
			segmentArr[0].x = arenaX2;

		if (segmentArr[0].y > arenaY2)
			segmentArr[0].y = arenaY1;
		else if (segmentArr[0].y < arenaY1)
			segmentArr[0].y = arenaY2;

		distance = (segmentArr[0].x - xo) * (segmentArr[0].x - xo) + (segmentArr[0].y - yo) * (segmentArr[0].y - yo);
		//printf("Distance between %d and %d is %d\n", segmentArr[0].x, xo, distance);

		if (distance < 900) {
			printf("COLLISION!!!\n");
			//snakeXVelocity = 0;
			//snakeYVelocity = 0;
			xo = arenaX1 + (rand() % (arenaX2 - 50));
			yo = arenaY1 + (rand() % (arenaY2 - 50));
			score++;
			tailCurrentLen++;
			segmentArr[tailCurrentLen].exists = true;
			
			switch (segmentArr[(tailCurrentLen - 1)].xVelocity)
			{
			case 5:
				segmentArr[tailCurrentLen].x = segmentArr[tailCurrentLen - 1].x - 20;
				break;
			case -5:
				segmentArr[tailCurrentLen].x = segmentArr[tailCurrentLen - 1].x + 20;
				break;
			default:
				segmentArr[tailCurrentLen].x = segmentArr[tailCurrentLen - 1].x;
			}

			switch (segmentArr[(tailCurrentLen - 1)].yVelocity)
			{
			case 5:
				segmentArr[tailCurrentLen].y = segmentArr[tailCurrentLen - 1].y - 20;
				break;
			case -5:
				segmentArr[tailCurrentLen].y = segmentArr[tailCurrentLen - 1].y + 20;
				break;
			default:
				segmentArr[tailCurrentLen].y = segmentArr[tailCurrentLen - 1].y;
			}
			printf("Score: %d\nTail length: %d\n", score, tailCurrentLen);

		}

		delay(100); // wait 50 ms , try different values for this delay

		if (_kbhit()) { // see http://www.programmingsimplified.com/c/conio.h/kbhit
			c = _getch();
			switch (c) {
			case 119: //key w
				segmentArr[0].yVelocity = -snakeSpeed;
				break;
			case 115: //key s
				segmentArr[0].yVelocity = snakeSpeed;
				break;
			case 97: //key a
				segmentArr[0].xVelocity = -snakeSpeed;
				break;
			case 100: //key d
				segmentArr[0].xVelocity = snakeSpeed;
				break;
			}
			//printf("key pressed is %d", c);
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