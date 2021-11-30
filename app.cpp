/*----------Running lizard GAME----------*/

#include <graphics.h>
#include <stdint.h>  // For typedefs of fundamental integral types 
#include <stdlib.h>  // For string manipulation
#include <iostream>  // For debugging
#include <ctime> 	 // For Generating random numbers


// Preprocessor definitions
#define WIDTH 810
#define HEIGHT 600

enum DIR
{
	LEFT, 	// 0
	UP,   	// 1
	RIGHT,	// 2
	DOWN 	// 3
};

// Custom user defined Headers
#include "Utilities/position.h"
#include "Utilities/gameMenu.h"
#include "Characters/lizard.h"
#include "Characters/player.h"
#include "Characters/enemy.h"
#include "GameObjects/grid.h"
#include "GameObjects/food.h"
#include "GameObjects/edible.h"
#include "GameObjects/poison.h"

using namespace std;

// Drawing Screen UI
void drawKeys(int16_t, int16_t);
void drawLives(const int16_t, uint8_t&);
void drawInstruction(int16_t, int16_t, int16_t, int16_t);

// Utility Function
inline void GenerationHandler(Edible, Edible, Poison, Player);

int main()
{
	initwindow(WIDTH, HEIGHT, "Running Lizard");

start:
#pragma region Fields
	// GameObjects
	Grid* grid;
	Player player;
	Edible fruit[2] = { Edible(1), Edible(5) }; // Two Food objects initialized in a random position
	Poison poison;
	Enemy enemy(300, 300);
	GameMenu* menu = new GameMenu();

	int8_t page = 1;							// signed char  == int8_t | 1 byte | -128 to 127
	uint8_t bodyLength;							// unsigned char == uint8_t | 1 byte | 0 to 255
	int16_t delaySpeed = 50;    				// short int == int16_t | 2 bytes | -32,768 to 32,767
	int16_t lifeCount = 3;		
	uint8_t lifePadding = 0; 
	int totalHit = 0;
	bool isPlaying = false;
	bool revealEnemy = false;
	bool lizardColideItself=false; 					// will not show enemy , until the speed of lizard be insane
	bool collide = false;
	bool skipFrame = true;
	bool nextPage = true;
    bool drawMenu = true;
	bool keyDown = false;
	const int8_t fruitCount = Edible::getCount();
	char score[4] = "0";
	char speed[10] = "Normal";
	

#pragma endregion Fields

	// generate new pos for food
	for (uint8_t i = 0; i < fruitCount; i++)
		fruit[i].generate(player.getPosx(), player.getPosy());

	// generate/regenerate new pos for poison
	poison.generate(player.getPosx(), player.getPosy());
	GenerationHandler(fruit[0], fruit[1], poison, player);

	while (true)
	{
		while (drawMenu)
		{
			setactivepage(page);
			setvisualpage(1 - page);
			cleardevice();

			// menu.showMenu();
			menu->detectInput(isPlaying);
			if (GetAsyncKeyState(VK_RETURN)) {
				if (!keyDown)
				{
					if (menu->getKeyState() == 1) goto end;
					cleardevice();
					isPlaying=true;
					drawMenu = false;
					keyDown = true;
					break;
				}
			}
			else keyDown = false;
			page = 1 - page;
		};
		if(isPlaying)
		{
		setactivepage(page);
		setvisualpage(1 - page);
		cleardevice();
		setcolor(BLUE);
		setfillstyle(SOLID_FILL, BLUE);
		menu->showOption(nextPage,keyDown);  // show the option for the user
		delete menu;


		// Controls Handler
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))
			player.changeDir(LEFT);
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))
			player.changeDir(UP);
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))
			player.changeDir(RIGHT);
		if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))
			player.changeDir(DOWN);
		if (isPlaying == true && !player.update())
		{
					isPlaying = false;
					lizardColideItself=true;
		}
		page = 1 - page;
		
		if (revealEnemy)
		{
			if (!skipFrame)
				enemy.update();
			skipFrame = !skipFrame;

			// change the direciton randomly
			enemy.changeDir();

			// Decrease a life if there is any collsion between the body and enemy
			if (!enemy.checkBody(player) && collide == false)
			{
				lifeCount -= 1;
				collide = true;
				totalHit++;
			}
			if (enemy.checkBody(player)){
				collide = false;
			}
		}
		if (totalHit >= 3)
		{
			isPlaying = false;
		}

		/*-UI-*/
		// Create a grid in dynamic memory
		grid = new Grid();
		// Draw grid
		grid->draw();

		player.draw();
		if (revealEnemy)
			enemy.draw();

		for (uint8_t i = 0; i < fruitCount; i++)
		{
			if (fruit[i].update(player.getPosx(), player.getPosy()))
			{
				fruit[i].generate(player.getPosx(), player.getPosy());
				// bool played = PlaySound(TEXT("DieSound.wav"), NULL, SND_FILENAME | SND_ASYNC);
				// cout << boolalpha << played << endl;
				player.append();
				// to make the enemy lizard half the size of the lizard

				// if (run)
				// 	enemy.append(lizard);
			}
		}

		// Score
		settextstyle(font_names::SANS_SERIF_FONT, HORIZ_DIR, 1);
		setcolor(WHITE);

		// Calculate score from body length
		bodyLength = player.getLength();
		strncpy(score, to_string((bodyLength - 2) * 10).c_str(), 4);

		// Display score
		outtextxy(20, 545, (char*)"SCORE");
		outtextxy(90, 545, score);

		// Game State
		// Regenerate new poison position
		// Check if player ate 3 poison
		settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
		if (poison.update(player.getPosx(), player.getPosy()))
		{
			poison.generate(player.getPosx(), player.getPosy());
			GenerationHandler(fruit[0], fruit[1], poison, player);
			lifeCount--;
			totalHit=poison.getHit();
		}
		if (isPlaying)
		{
			settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
			outtextxy(160, 545, (char*)"PLAYING");
		}
		// Display Controls - WASD
		setcolor(WHITE);
		drawKeys(295, 545);

		settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
		outtextxy(295, 545, (char*)" W ");
		outtextxy(270, 568, (char*)" A  ");
		outtextxy(295, 568, (char*)" S  ");
		outtextxy(320, 568, (char*)" D  ");

		// Display Controls - Arrow Keys
		setcolor(BLACK);
		drawKeys(385, 545);
		// up arrow key
		line(388, 559, 395, 549);
		line(395, 549, 402, 559);
		// down arrow key
		line(388, 574, 395, 584);
		line(395, 584, 402, 574);
		// right arrow key
		line(415, 574, 428, 579);
		line(428, 579, 416, 584);
		// left arrow key
		line(378, 574, 365, 579);
		line(365, 579, 378, 584);

		// Progressive speed
		if (atoi(score) >= 100 && atoi(score)<200)
		{
			delaySpeed = 40;
			strcpy(speed, "Fast");
		}
		else if (atoi(score) >= 200)
		{
			delaySpeed = 25;
			strcpy(speed, "Insane");
			revealEnemy = true; // now enemy will be revealed
		}
		else
		{
			delaySpeed=90;
		 	strcpy(speed, "Normal");
		}

		// Display Speed
		setcolor(WHITE);
		settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
		outtextxy(20, 575, (char*)"Speed");
		outtextxy(90, 575, speed);
		// Draw lives
		drawLives(lifeCount, lifePadding);

		// Draw food
		for (uint8_t i = 0; i < fruitCount; i++)
			fruit[i].draw();

		// Draw poison
		poison.draw();

		// Display Exit Key
		setcolor(WHITE);
		settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
		// Draw instruction
		drawInstruction(680, 575, 20, 90);

		// Check if player reached max length -> Won
		if (player.getLength() == 32)
		{
			setcolor(WHITE);
			outtextxy(160, 545, (char*)"Victory!");
			settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
			outtextxy(155, 200, (char*)"You Won! Press R to Restart");
			isPlaying = false;
		}
		// Retry prompt
		if (!isPlaying && player.getLength() != 32 && totalHit >= 3||lizardColideItself)
		{
			setcolor(WHITE);
			outtextxy(160, 545, (char*)"GAME OVER");
			settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
			outtextxy(190, 200, (char *)" Press 'ESC' to back to menu ");
		}


		// Control speed between frames
		delay(delaySpeed);
		// delete grid from memory
		// Reset page
		// page = 1 - page;
		}
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			if (!isPlaying && totalHit >= 3||player.getLength() == 32||lizardColideItself)
			{
				poison.resetHit();
				totalHit = 0;
				lifeCount = 3;
				player.resetLength();
				revealEnemy=false;
				lifePadding=0;
				lizardColideItself=false;
				strncpy(score, to_string(0).c_str(), 4);
			}
			menu = new GameMenu();
			drawMenu = true;
			cleardevice();
		}
		delete grid;
		grid = NULL;
	}
end:
	getch();
	closegraph();
}

#pragma region Functions

// Draw key boxes
void drawKeys(int16_t x, int16_t y)
{
	for (int16_t i = 0; i < 4; i++)
	{
		rectangle(x, y, x + 22, y + 20);
		if (i == 0)
		{
			y += 25;
			x -= 50;
		}
		x += 25;
	}
}

// Draw lives left
void drawLives(const int16_t counter, uint8_t& padding)
{
	uint8_t temp = padding;
	setcolor(RED);
	for (int16_t j = 0; j < 3; j++)
	{
		arc(485 + padding, 555, 0, 180, 10);
		arc(465 + padding, 555, 0, 180, 10);
		arc(475 + padding, 555, 180, 360, 20);
		padding += 50;
	}
	padding = temp;
	for (int16_t i = 0; i < counter; i++)
	{
		setfillstyle(SOLID_FILL, RED);
		floodfill(475 + padding, 560, RED);
		padding += 50;
	}
	padding = 0;
}

// Draw instructions
void drawInstruction(int16_t x, int16_t y, int16_t size, int16_t offset)
{
	// Text
	setcolor(COLOR(255, 45, 0));
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
	outtextxy(630, 575, (char*)"FOOD");
	// Food
	setcolor(RED);
	rectangle(x, y, x + size, y + size);
	setfillstyle(INTERLEAVE_FILL, RED);
	floodfill(x + (size / 2), y + (size / 2), RED);
	// Text
	setcolor(COLOR(10, 255, 0));
	settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
	outtextxy(705, 575, (char*)"POISON");
	// Poison
	setcolor(GREEN);
	rectangle(x + offset, y, x + size + offset, y + size);
	setfillstyle(INTERLEAVE_FILL, GREEN);
	floodfill(x + (size / 2) + offset, y + (size / 2), GREEN);
}

// Using inline solves overhead costs. It is expanded in line by the compiler when it is invoked.
// Generates new position if the position is equal to the food pos
inline void GenerationHandler(Edible f1, Edible f2, Poison p, Player b)
{
	if ((f1.foodPos.x == p.foodPos.x && f1.foodPos.y == p.foodPos.y) ||
		(f2.foodPos.x == p.foodPos.x && f2.foodPos.y == p.foodPos.y))
	{
		p.generate(b.getPosx(), b.getPosy());
	}
}
#pragma endregion Functions
