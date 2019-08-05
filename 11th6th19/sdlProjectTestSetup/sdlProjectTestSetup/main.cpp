//#include "SDL.h"
#include <SDL.h>
#include <iostream>
//#include <SDL/SDL_ttf.h>
//#include <SDL_ttf.h>
#include <string>
#include<sstream>
using namespace std;
bool bChangeStateToGo = false;
void winning();
void Destroy();
void GameOver();
SDL_Window *window;
SDL_Renderer *renderer;

SDL_Surface *bat;
SDL_Surface *ball;
SDL_Surface *bk;

string gameState = "idle";
// dont forget to sourcetree 
SDL_Texture *ballTexture;
SDL_Texture *bkTexture;
SDL_Texture *batTexture;

SDL_Event event;
bool bQuit = false;
bool bBallSpeedUp;
bool bBallHitBat = false;
int bounceBrickx = 100;
int bounceBRicky = 20;
int	ballx = 1;
int bally = -70;
int ballVelx = 1;
int ballVely = 1;
// background setup
int bkw = 800;
int bkh = 600;
int brickW = 80;
int brickH = 35;
int bkwmin = 0;
int bkhmin = 0;
int score = 0;
int deleteBrickCount = 0;
int numberOfBricks = 14;
SDL_Surface *brick;
SDL_Surface *bounceBrick;
SDL_Texture *brickTexture;
SDL_Texture *bounceBrickTexture;
SDL_Rect bounceBrickRect;
SDL_Rect brickRect[2][7];
SDL_Rect ballrect;

//TTF_Font *times;
SDL_Color fontColor{ 255,0,255 };
// location of score 
SDL_Rect playerScoreRect;

// bat
// divide by 2 to because bat should update on center of sceen and a just above bottum ofscreen
int batx = bkw / 2;
// -30 to get bat to apear just above the buttom of the screen
int baty = bkh - 30;



void Destroy()
{
	SDL_DestroyTexture(batTexture);
	SDL_DestroyTexture(brickTexture);
	SDL_DestroyTexture(bkTexture);
	SDL_DestroyTexture(ballTexture);
	SDL_FreeSurface(bat);
	SDL_FreeSurface(brick);
	SDL_FreeSurface(bk);
	SDL_FreeSurface(ball);
	//SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(window);
}
void Initialize()
{

	

	//Destroy();
		// need a better way to do this maybe for loop
		// dynamic array
	playerScoreRect.x = 50;
	playerScoreRect.y = 700;
	bBallHitBat = false;
	score = 0;
	deleteBrickCount = 0;


	brickRect[0][0] = { 50, 50, brickW, brickH };
	brickRect[0][1] = { 150, 50, brickW, brickH };
	brickRect[0][2] = { 250, 50, brickW, brickH };
	brickRect[0][3] = { 350, 50, brickW, brickH };
	brickRect[0][4] = { 450, 50, brickW, brickH };
	brickRect[0][5] = { 550, 50, brickW, brickH };
	brickRect[0][6] = { 650, 50, brickW, brickH };

	brickRect[1][0] = { 50, 100, brickW, brickH };
	brickRect[1][1] = { 150, 100, brickW, brickH };
	brickRect[1][2] = { 250, 100, brickW, brickH };
	brickRect[1][3] = { 350, 100, brickW, brickH };
	brickRect[1][4] = { 450, 100, brickW, brickH };
	brickRect[1][5] = { 550, 100, brickW, brickH };
	brickRect[1][6] = { 650, 100, brickW, brickH };

}

void EventHandler()
{
	if (gameState == "idle")
	{
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			bChangeStateToGo = true;
		}
	}
	SDL_PollEvent(&event);

	if (event.type == SDL_QUIT)
	{
		bQuit = true;
	}
	else if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_LEFT && batx > 0)
		{
			
			batx = batx - 4;
		}
		if (event.key.keysym.sym == SDLK_RIGHT && batx < bkw - 60)
		{
			batx = batx + 4;
		}
	}
}

void moveBall()
{
	if (gameState == "play")
	{
	
		ballx = ballx + ballVelx;
		bally = bally + ballVely;
	}
	if (gameState == "idle")
	{
		ballVelx = 0;
		ballVely = 0;
	}
}
void GameOver()
{
	bBallHitBat = false;
	gameState = "idle";
	SDL_Surface *goSurface = SDL_LoadBMP("BK.bmp");
	SDL_Texture *goTexture = SDL_CreateTextureFromSurface(renderer, goSurface);
	SDL_Rect goRect = { 0,0, bkw, bkh };
	SDL_RenderCopy(renderer, goTexture, NULL, &goRect);
	SDL_RenderPresent(renderer);
	SDL_Delay(2);
	//	Destroy();
		//SDL_Quit();

	deleteBrickCount = 0;
	score = 0;
	ballx = 1;
	bally = -70;
	// bat
// divide by 2 to because bat should update on center of sceen and a just above bottum ofscreen
	int batx = bkw / 2;
	// -30 to get bat to apear just above the buttom of the screen
	int baty = bkh - 30;

	Initialize();

}
void ballCollisoin()
{
	// if the ball goes out of screen bounce it on x 
	// -30 is the width of the ball 
	if (bBallHitBat == true)
	{
		if (ballx < bkwmin || ballx> bkw - 30)
		{
			ballVelx = -ballVelx;
		}
		// if the ball goes out of screen bounce it on y 
		// -30 is the width of the ball 
		if (bally < bkhmin)
		{
			ballVely = -ballVely;
		}

	}
	if (bally > bkh + 60|| ballx >bkw)
	{
		gameState = "idle";
		bBallHitBat = false;
		GameOver();

	}

	int ballScaling = 20;
	// ball scalin means the ball will hit top of the bat 
	if (bally + ballScaling >= baty && bally + ballScaling <= baty + 30 && ballx + ballScaling >= batx && ballx + ballScaling <= batx + 60)
	{															// +30 for top side of bat
		ballVely = -ballVely;
		bBallHitBat = true;
	}

}


bool ballBrickColDetect(SDL_Rect rect1, SDL_Rect rect2)
{
	if (rect1.x > rect2.x + rect1.w)
	{
		return false;
	}
	if (rect1.x + rect1.w < rect2.x)
	{
		return false;
	}
	if (rect1.y > rect2.y + rect2.h)
	{
		return false;
	}
	if (rect1.y + rect1.h < rect2.y)
	{
		return false;
	}
	return true;
}

void ballBrickCol()
{
	if (bBallHitBat == true)
	{
		bool a;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				a = ballBrickColDetect(brickRect[i][j], ballrect);
				if (a == true)
				{
					// get off screen man 
					brickRect[i][j].x = 3000;
					ballVely = ballVely;
					score += 10;
					deleteBrickCount++;
				}
			}
		}
	}
}

void winning()
{

	gameState = "idle";
		
	if (bBallSpeedUp == true)
	{
		ballVelx = 2;
		ballVely = 2;
		bBallSpeedUp = false;
	}
	Initialize();
	//SDL_Surface *winSurface = SDL_LoadBMP("bk.bmp");
	//SDL_Texture *winTexture = SDL_CreateTextureFromSurface(renderer, winSurface);
	//SDL_Rect winRect{ 250,200, 350, 350 };
	//SDL_RenderCopy(renderer, winTexture, NULL, &winRect);
	//SDL_RenderPresent(renderer);
	//SDL_Delay(30000);
	///Destroy();
	//SDL_Quit();


}

// brickrect		// ballrect

int main(int argc, char *argv[])
{

	SDL_Init(SDL_INIT_VIDEO);
	// make sure the text loads init
	//if (TTF_Init() < 0)

	//	std::cout << "ERROR:" << TTF_GetError() << std::endl;

	//}
	// create window instance

	// convert score to string
	stringstream pScore;
	pScore << score;
	//times = TTF_OpenFont("times.ttf", 14);
	playerScoreRect.x = 50.0f;
	playerScoreRect.y = 50.0f;
	//SDL_Surface *PlayerScoreSurface = TTF_RenderText_Solid(times, pScore.str().c_str(), fontColor);
//	SDL_BlitSurface(PlayerScoreSurface, NULL,bk, &playerScoreRect);


	window = SDL_CreateWindow("game ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	// -1 means first diplay detected
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Rect bkrect = { 0,0, 800, 600 };
	Initialize();
	//SDL_BlitSurface(PlayerScoreSurface, NULL,window, &playerScoreRect);
	bounceBrick = SDL_LoadBMP("bat.bmp");
	bat = SDL_LoadBMP("bat.bmp");
	ball = SDL_LoadBMP("ball.bmp");
	bk = SDL_LoadBMP("bk.bmp");
	brick = SDL_LoadBMP("brick.bmp");
	bounceBrickTexture = SDL_CreateTextureFromSurface(renderer, bounceBrick);
	ballTexture = SDL_CreateTextureFromSurface(renderer, ball);
	bkTexture = SDL_CreateTextureFromSurface(renderer, bk);
	batTexture = SDL_CreateTextureFromSurface(renderer, bat);
	brickTexture = SDL_CreateTextureFromSurface(renderer, brick);
	// no need to get pos because its in in ballrect
	// game loop
	while (!bQuit)
	{
		if (bChangeStateToGo == true)
		{
			gameState = "play";
			bChangeStateToGo = false;

		}

		

		// controls speed of objects sdl delay 
		SDL_Delay(5);
		SDL_Rect batrect = { batx, baty, 60, 30 };
		ballrect = { ballx, bally, 25,25 };
		bounceBrickRect = { bounceBrickx, bounceBRicky,  90,30 };

		EventHandler();
		if (gameState == "play")
		{
		
			moveBall();
			ballCollisoin();
			ballBrickCol();


			if (deleteBrickCount == numberOfBricks)
			{
				bBallSpeedUp = true;
				winning();
				//GameOver();
			}
		}
		// not sure this is a good one, renderering background everyframe
		// need to check this later
		SDL_RenderCopy(renderer, bkTexture, NULL, &bkrect);
		SDL_RenderCopy(renderer, ballTexture, NULL, &ballrect);
		SDL_RenderCopy(renderer, bounceBrickTexture, NULL, &bounceBrickRect);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[0][0]);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[0][1]);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[0][2]);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[0][3]);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[0][4]);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[0][5]);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[0][6]);

		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[1][0]);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[1][1]);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[1][2]);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[1][3]);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[1][4]);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[1][5]);
		SDL_RenderCopy(renderer, brickTexture, NULL, &brickRect[1][6]);

		// renderpresent does not need sdlrendercopy only once
		// bat needs to be constant update 
		SDL_RenderCopy(renderer, batTexture, NULL, &batrect);
		SDL_RenderPresent(renderer);
		// clear ball 
		SDL_RenderClear(renderer);
	}
	Destroy();
	SDL_Quit();

	return 0;
}