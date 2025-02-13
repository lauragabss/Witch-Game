#include "Headers/header.h"
#include "Headers/animationHeader.h"
#include "Headers/itemsHeader.h"
#include <iostream>
#include "Headers/generalFunctions.h"

float playerPos[2] = {500,500};

using namespace std;

int main()
{
	Game gameRef;
	//window config
	InitWindow(gameRef.screenSize.width, gameRef.screenSize.height, "Animation Game");
	SetTargetFPS(60);
	const int amountOfEnemies = 10;

	Player* playerActor = new Player();
	gameRef.AddPawn(playerActor);

	Enemy* tempEnemies[amountOfEnemies];
	//Starter enemies
	for (int i=0; i < amountOfEnemies; i++)
	{
		// Location
		tempEnemies[i] = new Enemy(MakeRandomLocation(gameRef.screenSize, 100), playerActor);

		gameRef.AddPawn(tempEnemies[i]);
	}

	Potion* potion = new Potion(MakeRandomLocation(gameRef.screenSize, 100));
	gameRef.AddPawn(potion);

	//Begin play
	gameRef.BeginPlay();

	//Game loop
	while(!WindowShouldClose())
	{
		BeginDrawing();
		
		//Background color
		ClearBackground(PURPLE);


		//Tick
		gameRef.Tick();

		//Collisions
		gameRef.Collisions();

		//Draw UI
		DrawText(TextFormat("Mana %i / %i", playerActor->GetMana(), playerActor->GetMaxMana()), 10, 30, 20, BLUE);
		DrawText(TextFormat("SCORE %i", gameRef.score), 10, 0, 20, WHITE);
		DrawText(TextFormat("LIFE %i / %i", playerActor->life, playerActor->maxLife), 10, 60, 20, RED);

		//end
		EndDrawing();
	}

	//Close game
	EndDrawing();

	CloseWindow();
	return 0;
}
