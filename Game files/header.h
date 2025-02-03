#pragma once
#include "raylib.h"
#include <vector>
#include <cstdio>
#include <iostream>
#include <string>
#include "animationHeader.h"
#include "movementHeader.h"

using namespace std;

enum classTag { pawn = 1, player = 2, NPC = 3, ally= 4, enemy= 5};

struct hability 
{
	float skillCooldown = 1;
	float currentCooldownTime = 1;
	bool canUse = true;
};

struct ScreenSize
{
	int width = 1200;
	int height = 800;
};


class Game;


class Actor
{
public:
	//Actor CreateActor(Color color, int size, Movement movement);
	float size = 100;
	Color actorTint = WHITE;
	virtual void BeginPlay();
	virtual void Tick();
	bool shouldDestroy = false;
	bool isActive = true;
	Game* gameRef = nullptr;
};

//Pawn are moveable objects
class Pawn : public Actor 
{
public:
	Pawn(Vector2 position);
	Pawn();
	Animation animation;
	Rectangle rec = { 0, 0, 1, 1 };
	float collisionThreshold = 1.0;
	bool drawCollisions = false;
	int index;
	int life = 100;
	int maxLife = 100;
	virtual classTag GetClassTag();
	virtual void CollisionEvent(Pawn* target);

	//movement
	virtual void UpdateMovement();
	virtual void CollideWithPlayer(Pawn *player);
	void SetMovement(Movement mov);
	Movement GetMovement();
	void ResetCollisions();
	void AddVelocity(float velX, float velY);

	//collision
	virtual void Collision(Rectangle rec, Pawn* pawn);
	virtual bool CheckCollision(Pawn *playerRect);
	virtual void CheckOverlap(Pawn* overlappedPawn);
	void CheckCollisionWithScreen();

	//event
	void Tick() override;
	void ReceiveDamage(int damage);
	void ReceiveBlessing();
	void CalculateVulnerability();

protected:
	Movement movement;

private:
	bool canReceiveDamage = true;
	float damageCountdown = 1;
};


//Player is player and receives inputs
class Player : public Pawn
{
public:
	Player();
	classTag clTag = player;
	int mana = 1;
	int maxMana = 10;
	int magicCost = 6;
	void Inputs();
	void UpdateMovement() override;
	void Collision(Rectangle rec, Pawn* pawn) override;
	void BeginPlay() override;
	void Tick() override;
	void CollisionEvent(Pawn* target) override;
	classTag GetClassTag() override;

private:
	void SpawnCat();
	hability spawnCatHability;
};

//NPCs are non playable characters
class NPCs : public Pawn 
{
public:
	classTag clTag;
	void Tick() override;
	void AssignPlayer(Player* player);
	classTag GetClassTag() override;

protected:
	float GetTargetDistance(Pawn* target);
	void MoveToPawn(Pawn* target);
	void RunFromPawn(Pawn* target);
	Player* playerRef = nullptr;
};

//Enemies are characters that attack players
class Enemy : public NPCs 
{
public:
	Enemy(Vector2 position, Player* playerRef);
	classTag clTag = enemy;
	void Tick() override;
	void BeginPlay() override;
	classTag GetClassTag() override;
	void CollisionEvent(Pawn* target) override;

private:
	void CollideWithPlayer(Pawn* player) override;
};

//Allies are characters to help the player
class Ally : public NPCs 
{
public:
	classTag clTag = ally;
	void BeginPlay() override;
	void UpdateMovement() override;
	void CheckOverlap(Pawn* overlappedPawn, int pawnIndex);
	classTag GetClassTag() override;

private:
	void CollideWithPlayer(Pawn* player) override;
	bool isRunningFromPlayer = false;
};

//Game has the general game data that has to be accessed by many functions
class Game
{
public:
	Game();
	ScreenSize screenSize;
	vector<Pawn*> gamePawns = {};
	vector<Ally>allies;
	int score = 0;
	void AddPawn(Pawn* pawn);
	void RemovePawn(int index);
	void BeginPlay();
	void Tick();
	void Collisions();
};
