//general functions
#include <cstdio>
#include <iostream>
#include "Headers/header.h"

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
	int pawnsAmount = gamePawns.size();
	bool assigned = false;
	pawn->gameRef = this;

	for (int i = 0; i < pawnsAmount; i++)
	{
		if(gamePawns[i] == nullptr)
		{
			//delete gamePawns[i];
			gamePawns[i] = pawn;
			pawn->index = i;
			assigned = true;
			break;
		}
	}

	if (assigned == false)
	{
		gamePawns.push_back(pawn);
		pawn->index = pawnsAmount;
	}

	pawn->BeginPlay();
}

void Game::RemovePawn(int index)
{
	gamePawns[index]->isActive = false;
	gamePawns[index]->shouldDestroy = false;
	gamePawns[index] = nullptr;
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
		if (gamePawns[i] != nullptr)
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
}

void Game::Collisions()
{
	int pawnsAmount = gamePawns.size();
	for (int i = 0; i < pawnsAmount; i++)
	{
		if ((gamePawns[i] != nullptr) && (!gamePawns[i]->shouldDestroy))
		{
			for (int j = 0; j < pawnsAmount; j++)
			{
				if ((i != j) && (gamePawns[j] != nullptr) && (!gamePawns[j]->shouldDestroy))
				{
					gamePawns[i]->CheckOverlap(gamePawns[j]);
				}
			}
		}
		
	}
}

