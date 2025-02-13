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

protected:
	string spriteName = "Potion.png";
};


class ManaPotion : public Potion
{
public:
	using Potion::Potion;

	void BeginPlay() override;

	void CollideWithPlayer(Player* player) override;

private:
	int thisBuffValue = 3;
};

class LifePotion : public Potion
{
public:
	using Potion::Potion;

	void BeginPlay() override;

	void CollideWithPlayer(Player* player) override;

private:
	int thisBuffValue = 1;
};
