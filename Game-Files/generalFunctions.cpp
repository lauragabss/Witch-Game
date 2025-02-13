#include "Headers/generalFunctions.h"
#include "Headers/header.h"
#include "Headers/itemsHeader.h"

using namespace std;

//vector<Pawn*> gamePawns;
//int score = 0;


void DrawLife(int life, int maxLife, float posX, float posY, int size)
{
	DrawText(TextFormat("%i / %i", life, maxLife), posX, posY, size, RED);
}

int RandomNumInRange(int min, int max) 
{
	return(min + (rand() % max));
}

int Clamp(int value, int min, int max)
{
	if(value < min)
	{
		return min;
	}
	else if (value > max)
	{
		return max;
	}
	return value;
}


Vector2 MakeRandomLocation(ScreenSize screenSize, int screenOffset)
{
	Vector2 randomLocation;
	int maxWidth = screenSize.width - screenOffset;
	int maxHeight = screenSize.height - screenOffset;
	randomLocation.x = Clamp(RandomNumInRange(screenOffset, maxWidth), screenOffset, maxWidth);
	randomLocation.y = Clamp(RandomNumInRange(screenOffset, maxHeight), screenOffset, maxHeight);

	return randomLocation;
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
	delete gamePawns[index];
	gamePawns[index] = nullptr;
}

void Game::BeginPlay()
{
	int pawnsAmount = gamePawns.size();
	for (int i = 0; i < pawnsAmount; i++)
	{
		if (gamePawns[i] != nullptr)
		{
			gamePawns[i]->BeginPlay();
		}
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
					if (gamePawns[i]->shouldDrawLife)
					{
						DrawLife(gamePawns[i]->life, gamePawns[i]->maxLife, gamePawns[i]->rec.x, gamePawns[i]->rec.y - 10, 20);
					}
				}
			}
		}
	}

	if (winGame)
	{
		DrawText(TextFormat("                   CONGRATULATIONS!\n YOU ARE NOW GRADUATED IN CAT WITCHCRAFT"), 100, (screenSize.height / 2) - 40, 40, WHITE);
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

void Game::InitializeGame(int enemiesAmount)
{
	// Delete all from game
	ClearGame();

	winGame = false;

	// Spawn character
	Player* player = new Player();
	playerActor = player;
	playerActor->gameRef = this;
	AddPawn(player);

	std::vector<Enemy*> tempEnemies(enemiesAmount);
	// Starter enemies
	for (int i = 0; i < enemiesAmount; i++)
	{
		// Location
		tempEnemies[i] = new Enemy(MakeRandomLocation(screenSize, 100), playerActor);
		AddPawn(tempEnemies[i]);
	}

	allies = {};

	// Spawn initial potion
	ManaPotion* potion = new ManaPotion(MakeRandomLocation(screenSize, 100));
	AddPawn(potion);

	//Begin play
	BeginPlay();
}

void Game::ClearGame()
{
	for (Pawn* pawn : gamePawns)
	{
		if (pawn != nullptr)
		{
			pawn->shouldDestroy = true;
		}
	}

	/*
	for (Ally* ally : allies)
	{
		ally->shouldDestroy = true;
	}
	*/
}

void Game::SpawnEnemy()
{
	bool gotValidLocation = false;
	Vector2 playerPosition = playerActor->GetMovement().position;
	Vector2 location;
	while(!gotValidLocation)
	{
		location = MakeRandomLocation(screenSize, 100);

		// Check if it's too close from player
		gotValidLocation = !(location.x >= playerPosition.x - 500 && location.x <= playerPosition.x + 500 &&
			location.y >= playerPosition.y - 500 && location.y <= playerPosition.y + 500);

	}
	printf("\nplayer location: (%f/%f)     random location: (%f/%f) ", playerPosition.x, playerPosition.y, location.x, location.y);

	Enemy* enemy = new Enemy(MakeRandomLocation(screenSize, 100), playerActor);
	AddPawn(enemy);
}
