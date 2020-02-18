// BGIFirstTest.c : Example graphic application
//


#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>   
#include <stdbool.h>

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

	struct SnakeSegment
	{
		int x, y;
		bool exists;
	};

	struct SnakeSegment segmentArr[10];

	segmentArr[0].x = 100;
	segmentArr[0].y = 100;
	segmentArr[0].exists = true;


	int xo = 200;
	int yo = 300;

	initgraph(&gd, &gm, "");

	int c = 0; // for input from keyboard

	int i = 0;
	int distance;
	int score = 0;
	int snakeSpeed = 5;
	int snakeXVelocity = snakeSpeed;
	int snakeYVelocity = 0;

	do {
		setactivepage(i % 2);
		clearviewport();
		line(arenaX1, arenaY1, arenaX1, arenaY2);
		line(arenaX1, arenaY2, arenaX2, arenaY2);
		line(arenaX2, arenaY2, arenaX2, arenaY1);
		line(arenaX2, arenaY1, arenaX1, arenaY1);

		drawObstacle(xo, yo);
		// draw circle in current position
		for (int i = 0; i < getLength(segmentArr); i++)
		{
			if (segmentArr[i].exists)
			{
				drawAvatar(segmentArr[i].x, segmentArr[i].y);
			}
		}
		

		//drawAvatar(x2, 150);
		// update position of circle of a fixed offset
		setvisualpage(i % 2);
		i++;

		segmentArr[0].x += snakeXVelocity;   // try different values for the offset, e.g. x+=5 or x+=10
		segmentArr[0].y += snakeYVelocity;


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
			printf("Score: %d\nTail length: %d\n", score, tailCurrentLen);
			segmentArr[tailCurrentLen].exists = true;
			segmentArr[tailCurrentLen].x = segmentArr[0].x;
			segmentArr[tailCurrentLen].y = segmentArr[0].y;
		}

		delay(100); // wait 50 ms , try different values for this delay

		if (_kbhit()) { // see http://www.programmingsimplified.com/c/conio.h/kbhit
			c = _getch();
			switch (c) {
			case 119: //key w
				snakeYVelocity = -snakeSpeed;
				break;
			case 115: //key s
				snakeYVelocity = snakeSpeed;
				break;
			case 97: //key a
				snakeXVelocity = -snakeSpeed;
				break;
			case 100: //key d
				snakeXVelocity = snakeSpeed;
				break;
			}
			//printf("key pressed is %d", c);
		}

	} while (c != KEY_ESCAPE);


}

void drawAvatar(int x, int y)
{
	circle(x, y, 20);
}

void drawObstacle(int x, int y)
{
	circle(x, y, 10);
}