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
	gameRef.InitializeGame(amountOfEnemies);

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
		DrawText(TextFormat("Mana %i / %i", gameRef.playerActor->GetMana(), gameRef.playerActor->GetMaxMana()), 10, 30, 20, BLUE);
		DrawText(TextFormat("SCORE %i", gameRef.score), 10, 0, 20, WHITE);
		DrawText(TextFormat("LIFE %i / %i", gameRef.playerActor->life, gameRef.playerActor->maxLife), 10, 60, 20, RED);

		//end
		EndDrawing();
	}

	//Close game
	EndDrawing();

	CloseWindow();
	return 0;
}
