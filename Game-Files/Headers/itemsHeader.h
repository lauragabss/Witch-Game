#pragma once

#include "header.h"

using namespace std;

class Potion : public Pawn
{
public:
	using Pawn::Pawn;

	void BeginPlay() override;

	virtual void CollideWithPlayer(Player* player) override;

	int buffValue = 2;
};
