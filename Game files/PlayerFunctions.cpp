//player functions
#pragma once
#include "header.h"


void Player::SpawnCat()
{
	if (spawnCatHability.canUse)
	{
		//create ally definition
		Ally ally;
		Movement mov = ally.GetMovement();
		Vector2 position = { movement.position.x + movement.velocity.x * 10 , movement.position.y + movement.velocity.y * 10 };
		mov.position = position;
		ally.SetMovement(mov);
		ally.AssignPlayer(this);

		//try to allocate the ally

		gameRef->allies.push_back(ally);
		int size = gameRef->allies.size();
		gameRef->AddPawn(&(gameRef->allies[size - 1]));

	}
}
