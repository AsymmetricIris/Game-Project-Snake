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
//
//	Score table
//display current score
//	itoa functions are being difficult
//add high scores as an array of 100 scores
//delete scores from the scores file when there are more than 100 of them
//order the scores array to write back to the file
//	instead of a sorting algorithm, can use binary search to find the numbers a new score fits between
//	preventative maintenance
//		algorithm
//			binary search to find a place where either the score fits between two consecutive numbers or where it is equal
//			when a score is equal, the algorithm will end
//			when a score fits between two numbers
//				copy all scores below the current score - except the lowest of them all - into a new array of lower scores
//				copy all the scores higher than the current score into an array of higher scores
//				combine the two arrays
//			when a score is lower than everything else but higher than the lowest score
//				the lowest score is overwritten
//	Animations
//Try the vanilla BGI library
//Try importing images
//Try using fill functions instead of line-drawing ones
//Make game pretty

//DONE
//Fix disappearing tail segments
//Make segments follow the head's path
	//Calculate row and col from x and y coords


#include "graphics.h"
//include "snake.h"
#include "fixedSnake.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>   
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <math.h>

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
#define ROWS 449
#define COLS 501

#define getLength(x) (sizeof(x) / sizeof(x[0]))

void drawAvatar(int x, int y);
void drawObstacle(int x, int y);
void populateIntArray(int* arr, int cols, int rows, int val);
void printArray(int* arr, int cols, int rows);
void setArrVal(int* ptr_arr, int val, int row, int col, int array_rows, int array_cols);
int getArrVal(int* ptr_arr, int row, int col, int array_rows, int array_cols);
bool askToPlayAgain();

void main(void)
{
	// Initialise graphic window
	int gd = DETECT, gm = 0;
	int arenaOffset = 50;
	int arenaX1 = arenaOffset;
	int arenaY1 = arenaOffset;
	int arenaX2 = 500;
	int arenaY2 = 448;
	int pathMap[ROWS][COLS];
	int* ptr_pathMap = &pathMap;

	populateIntArray(&pathMap, ROWS, COLS, 0);

	// Debug - testing 2DArray functions
	//setArrVal(&pathMap, 4, 500, 500, 501, 501);
	//printf("Value at 5, 5: %d\n", getArrVal(&pathMap, 5, 5, 501, 501) );
	//printArray(&pathMap, ROWS, COLS);

	Snake playerSnake;

	initgraph(&gd, &gm, "");

	int keyPressed = 0; // for input from keyboard
	bool gameContinue = true;
	bool gamePlay = true;

	// read high score from data file
	FILE* scoreInput, * scoreOutput;
	errno_t err;

	int score = 0;
	int highScore;

	if ((err = fopen_s(&scoreInput, "scores.dat", "r")) != 0)
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

	do {
		printf("Playing game\n");
		playerSnake = createSnake(100, 100, 10);

		int xo = 200;
		int yo = 300;

		int i = 0;
		int distanceToGoodie;

		//save high score to data file
		if ((err = fopen_s(&scoreOutput, "scores.dat", "a")) != 0)
		{
			fprintf(stderr, "ERROR: can't open file\n");
			return;
		}

		do {
			setactivepage(i % 2);
			clearviewport();
			line(arenaX1, arenaY1, arenaX1, arenaY2);
			line(arenaX1, arenaY2, arenaX2, arenaY2);
			line(arenaX2, arenaY2, arenaX2, arenaY1);
			line(arenaX2, arenaY1, arenaX1, arenaY1);

			drawObstacle(xo, yo);
			// draw circle in current position
			for (int i = 0; i <= playerSnake.size - 1; i++)
			{
				drawAvatar(playerSnake.segments[i].x, playerSnake.segments[i].y);
			}

			// update position of circle of a fixed offset
			setvisualpage(i % 2);
			i++;

			updateSnakePos(&playerSnake);


			// End the game if the snake touches a wall
			// TODO - make more accurate
			for (int segment = 0; segment <= playerSnake.size; segment++)
			{
				if (playerSnake.segments[segment].x > arenaX2)
					playerSnake.segments[segment].x = arenaX1;
				else if (playerSnake.segments[segment].x < arenaX1)
					playerSnake.segments[segment].x = arenaX2;

				if (playerSnake.segments[segment].y > arenaY2)
					playerSnake.segments[segment].y = arenaY1;
				else if (playerSnake.segments[segment].y < arenaY1)
					playerSnake.segments[segment].y = arenaY2;
			}

			distanceToGoodie = sqrt((playerSnake.segments[0].x - xo) * (playerSnake.segments[0].x - xo) + (playerSnake.segments[0].y - yo) * (playerSnake.segments[0].y - yo));

			if (distanceToGoodie < 20) {
				printf("COLLISION!!!\n");
				xo = arenaX1 + (rand() % (arenaX2 - 50));
				yo = arenaY1 + (rand() % (arenaY2 - 50));
				score++;
				addNewSegment(&playerSnake);
			}

			for (int i = 0; i < playerSnake.size - 1; i++)
			{
				int snakeIndex = i + 1;
				playerSnake.segments[snakeIndex].distanceToHead = sqrt((playerSnake.segments[snakeIndex].x - playerSnake.segments[0].x) * (playerSnake.segments[snakeIndex].x - playerSnake.segments[0].x) + (playerSnake.segments[snakeIndex].y - playerSnake.segments[0].y) * (playerSnake.segments[snakeIndex].y - playerSnake.segments[0].y));
				int distanceToHead = playerSnake.segments[snakeIndex].distanceToHead;

				//debug
				//printf("Tail %d distance to head: %d\n", i, distanceToHead);

				if (distanceToHead < 15)
				{
					gameContinue = false;
				}
			}

			delay(100); // wait 50 ms , try different values for this delay

			// change head's velocity on user input
			if (_kbhit()) { // see http://www.programmingsimplified.com/c/conio.h/kbhit
				keyPressed = _getch();
				switch (keyPressed) {
				case 119: //key w
					playerSnake.segments[0].yVelocity = -playerSnake.speed;
					playerSnake.segments[0].xVelocity = 0;
					setArrVal(&pathMap, PATH_N, (playerSnake.segments[0].y - arenaOffset), (playerSnake.segments[0].x - arenaOffset), ROWS, COLS);
					//debug
					//printf("North marker placed at:  (%d, %d)\n", (playerSnake.segments[0].y - arenaOffset), (playerSnake.segments[0].x - arenaOffset) );
					break;
				case 115: //key s
					playerSnake.segments[0].yVelocity = playerSnake.speed;
					playerSnake.segments[0].xVelocity = 0;
					setArrVal(&pathMap, PATH_S, (playerSnake.segments[0].y - arenaOffset), (playerSnake.segments[0].x - arenaOffset), ROWS, COLS);
					//debug
					//printf("South marker placed at:  (%d, %d)\n", (playerSnake.segments[0].y - arenaOffset), (playerSnake.segments[0].x - arenaOffset) );
					break;
				case 97: //key a
					playerSnake.segments[0].xVelocity = -playerSnake.speed;
					playerSnake.segments[0].yVelocity = 0;
					setArrVal(&pathMap, PATH_W, (playerSnake.segments[0].y - arenaOffset), (playerSnake.segments[0].x - arenaOffset), ROWS, COLS);
					//debug
					//printf("West marker placed at:  (%d, %d)\n", (playerSnake.segments[0].y - arenaOffset), (playerSnake.segments[0].x - arenaOffset) );
					break;
				case 100: //key d
					playerSnake.segments[0].xVelocity = playerSnake.speed;
					playerSnake.segments[0].yVelocity = 0;
					setArrVal(&pathMap, PATH_E, (playerSnake.segments[0].y - arenaOffset), (playerSnake.segments[0].x - arenaOffset), ROWS, COLS);
					//debug
					//printf("East marker placed at:  (%d, %d)\n", (playerSnake.segments[0].y - arenaOffset), (playerSnake.segments[0].x - arenaOffset) );
					break;
				}
				//printf("key pressed is %d", c);
			}

			// change segment's velocity to follow the path of prev ious segments
			// trigger a change when the tail segment's coordinate on the path map has a directional flag
			for (int segmentNum = 0; segmentNum <= playerSnake.size - 1; segmentNum++)
			{
				// find direction change when the segment crosses a point where the snake has changed direction
				int xPos = playerSnake.segments[segmentNum].x - arenaOffset;
				int yPos = playerSnake.segments[segmentNum].y - arenaOffset;
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
						playerSnake.segments[segmentNum].yVelocity = playerSnake.speed;
						playerSnake.segments[segmentNum].xVelocity = 0;
						break;
					case PATH_N: //upward
						playerSnake.segments[segmentNum].yVelocity = -playerSnake.speed;
						playerSnake.segments[segmentNum].xVelocity = 0;
						break;
					case PATH_W: //to the left
						playerSnake.segments[segmentNum].xVelocity = -playerSnake.speed;
						playerSnake.segments[segmentNum].yVelocity = 0;
						break;
					case PATH_E: //to the right
						playerSnake.segments[segmentNum].xVelocity = playerSnake.speed;
						playerSnake.segments[segmentNum].yVelocity = 0;
						break;
					default:
						break;
					}
				}
				if (segmentNum == playerSnake.size - 1)
				{
					setArrVal(&pathMap, PATH_EMPTY, yPos, xPos, ROWS, COLS); // clear direction change marker from the map

					//debug
					//printf("Deleting segment number %d at (%d, %d)\n", segmentNum, xPos, yPos);
				}
			}

		} while ((keyPressed != KEY_ESCAPE) && (gameContinue));
		keyPressed = 0;

		printf("Game over\nYour score is: %d\n", score);

		//ask if the user wants to play again
		gamePlay = askToPlayAgain();
		gameContinue = gamePlay;

		if (fprintf(scoreOutput, "%d\n", score) == 1)
		{
			printf("score %d written\n", score);
		}

		if (scoreOutput)
		{
			fclose(scoreOutput);
		}

	} while ((keyPressed != KEY_ESCAPE) && (gamePlay));
	keyPressed = 0;
	gamePlay = true;
}

bool askToPlayAgain()
{
	bool playAgain = false;
	char userAnswer;
	printf("Would you like to play again?\nAnswer (y/n): ");
	userAnswer = _getch();
	printf("Answer: %c\n", userAnswer);
	if (userAnswer == 'y')
	{
		playAgain = true;
	}
	

	return playAgain;
}

void drawAvatar(int x, int y)
{
	circle(x, y, 15);
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