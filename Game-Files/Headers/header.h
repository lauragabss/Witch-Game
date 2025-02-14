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

struct ability 
{
	float skillCooldown = 3;
	float currentCooldownTime = 1;
	bool canUse = true;
	int abilityCost = 6;
	int* manaPool = nullptr;
};

struct ScreenSize
{
	int width = 1200;
	int height = 800;
};

bool ActivateAbility(ability* hab);

class Game;
class Player;

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
	~Pawn();
	Animation animation;
	Rectangle rec = { 0, 0, 1, 1 };
	float collisionThreshold = 1.0;
	bool drawCollisions = false;
	int index;
	int life = 5;
	int maxLife = 5;
	bool shouldDrawLife = true;
	virtual classTag GetClassTag();

	//*** movement ***
	virtual void UpdateMovement();
	virtual void CollideWithPlayer(Player* player);
	void SetMovement(Movement mov);
	Movement GetMovement();
	void ResetCollisions();
	void AddVelocity(float velX, float velY);

	//*** collision ***
	virtual void Collision(Rectangle rec, Pawn* pawn);
	virtual bool CheckCollision(Pawn *playerRect);
	virtual void CheckOverlap(Pawn* overlappedPawn);
	void CheckCollisionWithScreen();

	//*** event ***
	void Tick() override;
	void ReceiveDamage(int damage);
	void ReceiveBlessing(int blessingAmount);
	void CalculateVulnerability();

	// Called when collision happens
	virtual void CollisionEvent(Pawn* target);

protected:
	virtual void Die();
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
	void Inputs();
	void UpdateMovement() override;
	void Collision(Rectangle rec, Pawn* pawn) override;
	void BeginPlay() override;
	void Tick() override;
	void CollisionEvent(Pawn* target) override;
	classTag GetClassTag() override;

	// Getters
	int GetMana();
	int GetMaxMana();

	// Setters
	void AddMana(int val);

protected:
	void Die() override;

private:
	void SpawnCat();
	ability spawnCatAbility;

	int mana = 10;
	int maxMana = 10;
	int magicCost = 6;
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
	bool MoveToPawn(Pawn* target);
	void RunFromPawn(Pawn* target);
	Player* playerRef = nullptr;
};

//Enemies are characters that attack players
class Enemy : public NPCs 
{
public:
	Enemy(Vector2 position, Player* playerRef);
	classTag clTag = enemy;
	void BeginPlay() override;
	void UpdateMovement() override;
	classTag GetClassTag() override;
	void CollisionEvent(Pawn* target) override;

private:
	void CollideWithPlayer(Player* player) override;
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
	void CollideWithPlayer(Player* player) override;
	bool isRunningFromPlayer = false;
};

//Game has the general game data that has to be accessed by many functions
class Game
{
public:
	Game();
	ScreenSize screenSize;
	vector<Pawn*> gamePawns = {};
	vector<Ally*>allies;
	Player* playerActor = nullptr;
	int score = 0;
	int maxScore = 10;
	bool winGame = false;
	void AddPawn(Pawn* pawn);
	void RemovePawn(int index);
	void BeginPlay();
	void Tick();
	void Collisions();
	void InitializeGame(int enemiesAmount);
	void ClearGame();
	void SpawnEnemy();
};
