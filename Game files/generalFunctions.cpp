//general functions
#include <cstdio>
#include <iostream>
#include "header.h"

using namespace std;

//vector<Pawn*> gamePawns;
//int score = 0;

Game gameRef;

void DrawLife(int life, int maxLife, float posX, float posY, int size)
{
	DrawText(TextFormat("%i / %i", life, maxLife), posX, posY, size, RED);
}

int RandomNumInRange(int min, int max) 
{
	return(min + (rand() % max));
}


void Game::AddPawn(Pawn* pawn)
{
	printf("\n+++++++++++++++++++++HERE+++++++++++++++++++++++\n");
	int pawnsAmount = gamePawns.size();
	bool assigned = false;
	pawn->gameRef = this;

	for (int i = 0; i < pawnsAmount; i++)
	{
		if(!gamePawns[i]->isActive)
		{
			printf("\nSubstitute!\n");
			//delete gamePawns[i];
			gamePawns[i] = pawn;
			pawn->index = i;
			assigned = true;
		}
	}

	if (assigned == false)
	{
		printf("\nAdd new one!\n");
		gamePawns.push_back(pawn);
		printf("-here-");
		pawn->index = pawnsAmount + 1;
		printf("fyeah");
	}
}

void Game::RemovePawn(int index)
{
	gamePawns[index]->isActive = false;
	gamePawns[index]->shouldDestroy = false;
}

void Game::BeginPlay()
{
	int pawnsAmount = gamePawns.size();
	for (int i = 0; i < pawnsAmount; i++)
	{
		gamePawns[i]->BeginPlay();
	}
}

void Game::Tick()
{
	int pawnsAmount = gamePawns.size();
	for (int i = 0; i < pawnsAmount; i ++)
	{
		if(gamePawns[i]->shouldDestroy)
		{
			RemovePawn(i);
		}
		else
		{
			if(gamePawns[i]->isActive)
			{
				gamePawns[i]->Tick();
				DrawLife(gamePawns[i]->life, gamePawns[i]->maxLife, gamePawns[i]->rec.x, gamePawns[i]->rec.y - 10, 20);
			}
		}
	}
}

void Game::Collisions()
{
	int pawnsAmount = gamePawns.size();
	for (int i = 0; i < pawnsAmount; i++)
	{
		if (gamePawns[i]->isActive) 
		{
			for (int j = 0; j < pawnsAmount; j++)
			{
				if ((i != j) && (gamePawns[j]->isActive))
				{
					gamePawns[i]->CheckOverlap(gamePawns[j]);
				}
			}
		}
		
	}
}

