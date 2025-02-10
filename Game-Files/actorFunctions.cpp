//actor functions
#include "Headers/header.h"


void CalculateCountdown(ability* hab)
{
	if (!hab->canUse)
	{
		hab->currentCooldownTime -= GetFrameTime();

		if (hab->currentCooldownTime <= 0.0f)    
		{
			hab->currentCooldownTime = hab->skillCooldown;
			hab->canUse = true;
		}
	}
}

bool ActivateAbility(ability* hab)
{
	if(hab->canUse)
	{
		if (*hab->manaPool >= hab->abilityCost)
		{
			hab->canUse = false;
			*hab->manaPool = *hab->manaPool - hab->abilityCost;
			return true;
		}
		return false;
	}

	return false;
}

//--------------Constructors-------------
Pawn::Pawn(Vector2 position) 
{
	movement.position = position;
}

Enemy::Enemy(Vector2 position, Player* playerRef)
{
	movement.position = position;
	AssignPlayer(playerRef);
}


Pawn::Pawn() {}

Pawn::~Pawn()
{
	// Pawn destructor
	printf("\nDESTROY PAWN\n");
}

Player::Player() 
{
	spawnCatAbility.manaPool = &mana;

	maxLife = 3;
	life = maxLife;
}

Game::Game() {}
//--------------Collision----------------

//Activate events based on collision
void Pawn::CollisionEvent(Pawn* target) 
{

}

void Enemy::CollisionEvent(Pawn* target)
{
	if (target->GetClassTag() == ally)
	{
		ReceiveDamage(5);
	}
}

void Player::CollisionEvent(Pawn* target)
{
	if (!target || target==nullptr)
	{
		return;
	}

	if (target->GetClassTag() == enemy)
	{
		ReceiveDamage(1);
	}
}

//Collided with something, return the collided object
void Pawn::Collision(Rectangle rec, Pawn* pawn) 
{
	if (rec.x > movement.position.x) 
	{
		movement.collision.right = true;
	}
	else
	{
		movement.collision.left = true;
	}

	if (rec.y < movement.position.y) 
	{
		movement.collision.top = true;
	}
	else 
	{
		movement.collision.bottom = true;
	}
	CollisionEvent(pawn);
}


void Player::Collision(Rectangle rec, Pawn* pawn)
{
	Pawn::Collision(rec, pawn);
}

void Pawn::CheckCollisionWithScreen()
{
	if (gameRef != nullptr) 
	{
		ScreenSize scSize = gameRef->screenSize;
		//collides on top
		if (rec.y <= 0)
		{
			//collided to top
			movement.collision.top = 1;
		}
		if (rec.y + rec.height >= scSize.height)
		{
			//collided bottom
			movement.collision.bottom = 1;
		}
		if (rec.x <= 0)
		{
			//collided left
			movement.collision.left = 1;
		}
		if (rec.x + rec.width >= scSize.width)
		{
			//collided right
			movement.collision.right = 1;
		}
	}
	
}

//Verify if collision boundaries collided with something and returns true or false
bool Pawn::CheckCollision(Pawn *target) 
{

	//Check if collided with player
	if (!(target == nullptr)) 
	{
		//check if collided with screen
		CheckCollisionWithScreen();
		coordinates collisionResult = { 0,0 };

		//player collision X axis	
		if (!((rec.x + rec.width) < target->rec.x)
			&& !((target->rec.x + target->rec.width) < rec.x))
		{
			//collide in the X axis
			collisionResult.x = 1;
		}

		//Calculate collision in Y
		if (!(rec.y < (target->rec.y - rec.height)) && !(rec.y > target->rec.y + target->rec.height))
		{
			//collided in the Y axis
			collisionResult.y = 1;
		}

		if ((collisionResult.x == 1) && (collisionResult.y == 1))
		{
			//collision happenned
			//player->Collision(rec, dynamic_cast<Pawn*>(this));
			Collision(target->rec, target);
			Player* tempPlayer = dynamic_cast<Player*>(target);
			if (!(tempPlayer == nullptr))
			{
				CollideWithPlayer(target);
			}
			return true;
		}
		else
		{
			//collision didn't happen
			return false;
		}
	}
	return false;
	
}

void Pawn::CheckOverlap(Pawn* overlappedPawn)
{
	CheckCollision(overlappedPawn);
}

void Ally::CheckOverlap(Pawn* overlappedPawn, int pawnIndex)
{
	if (CheckCollision(overlappedPawn))
	{
		Enemy* enemyRef = dynamic_cast<Enemy*>(overlappedPawn);
	}
}

void Pawn::CollideWithPlayer(Pawn* player) 
{

}

void Enemy::CollideWithPlayer(Pawn* player) 
{
	//dynamic_cast<Player*>(player)->ReceiveDamage(5);
}

void Ally::CollideWithPlayer(Pawn* player) 
{
	//isRunningFromPlayer = true;
	Player* player_ = dynamic_cast<Player*>(player);
	if(player_ && player_ != nullptr)
	{
		player_->ReceiveBlessing();
	}
}

//--------------Movement------------------

//update movement for rec
void Pawn::UpdateMovement() 
{
	//Update rec
	rec.x = movement.position.x;
	rec.y = movement.position.y;

	//Update animation
	animation.CalculateFrame();
	rec.width = (animation.GetImageSize().x) * animation.imagesizeMultiplier;
	rec.height = (animation.GetImageSize().y) * animation.imagesizeMultiplier;
}

//Add a velocity to a movement object (receives a target and an intended velocity)
void Pawn :: AddVelocity(float velX, float velY)
{
	//Check facing direction
	if (velX > 0) 
	{
		animation.facing = 1;
	}
	else if (velX < 0) 
	{
		animation.facing = -1;
	}

	//update velocity
	if ((velX > 0) && !(movement.collision.right)) 
	{
		velX = velX;
	}
	else if ((velX < 0) && !(movement.collision.left))
	{
		velX = velX;
	}
	else 
	{
		velX = 0;
	}

	if ((velY < 0) && !(movement.collision.top))
	{
		velY = velY;
	}
	else if ((velY > 0) && !(movement.collision.bottom))
	{
		velY = velY;
	}
	else
	{
		velY = 0;
	}

	//move
	movement.position.x += velX * movement.speed;
	movement.position.y += velY * movement.speed;

}


void Player::Inputs()
{
	//Movement inputs
	bool isMoving = false;
		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		{
			AddVelocity( 1, 0);
			isMoving = true;
		}
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
		{
			AddVelocity( -1, 0);
			isMoving = true;
		}
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
		{
			AddVelocity( 0, -1);
			isMoving = true;
		}
		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
		{
			AddVelocity( 0, 1);
			isMoving = true;
		}

		if (isMoving) 
		{
			animation.playerState = walk;
		}
		else 
		{
			animation.playerState = idle;
		}

		//Habilities
		if(IsKeyReleased(KEY_SPACE))
		{
			//Call function to activate hability
			SpawnCat();
		}
}

void Pawn::ReceiveDamage(int damage) 
{
	if (canReceiveDamage)
	{
		life = life - damage;
		if (life <= 0)
		{
			shouldDestroy = true;
		}
		damageCountdown = 1;
		canReceiveDamage = false;
	}
}

void Pawn::CalculateVulnerability() 
{
	if (!canReceiveDamage)
	{
		damageCountdown -= GetFrameTime();

		if (damageCountdown <= 0.0f)
		{
			damageCountdown = 1;
			canReceiveDamage = true;
		}
	}
}

void Pawn::ReceiveBlessing() 
{
	if (animation.imagesizeMultiplier < animation.maxImageSizeMultiplier) 
	{ 
		// Growth blessing
		//animation.imagesizeMultiplier = animation.imagesizeMultiplier * 2;

		// Life blessing
		int tempLife = life + 5;
		if (tempLife > maxLife)
		{
			life = maxLife;
		}
		else
		{
			life = tempLife;
		}
	}
}

classTag Pawn::GetClassTag()
{
	return pawn;
}

classTag Player::GetClassTag()
{
	return player;
}

classTag NPCs::GetClassTag()
{
	return NPC;
}

classTag Ally::GetClassTag()
{
	return ally;
}

classTag Enemy::GetClassTag()
{
	return enemy;
}

float NPCs::GetTargetDistance(Pawn* target) 
{
	float distX, distY, distance;
	distX = target->GetMovement().position.x - movement.position.x;
	distY = target->GetMovement().position.y - movement.position.y;
	distance = pow(distX, 2) + pow(distY, 2);
	distance = sqrt(distance);
	return distance;
}


bool NPCs::MoveToPawn(Pawn* target)
{
	if (target == nullptr || !target->isActive)
	{
		return false;
	}

	int velX, velY;
	//X axis
	if (target->GetMovement().position.x > movement.position.x)
	{
		velX = 1;
	}
	else if (target->GetMovement().position.x < movement.position.x)
	{
		velX = -1;
	}
	else
	{
		velX = 0;
	}

	//Y axis
	if (target->GetMovement().position.y > movement.position.y)
	{
		velY = 1;
	}
	else if (target->GetMovement().position.y < movement.position.y)
	{
		velY = -1;
	}
	else
	{
		velY = 0;
	}

	if (velX == 0 && velY == 0)
	{
		return false;
	}

	AddVelocity(velX, velY);

	return true;
}


void Pawn::SetMovement(Movement mov) 
{
	movement = mov;
}


Movement Pawn::GetMovement() 
{
	return movement;
}

void Player::UpdateMovement() 
{
	Player::Inputs();
	Pawn::UpdateMovement();
}

void Ally::UpdateMovement() 
{
	if (playerRef != nullptr)
	{

		float distance;
		distance = GetTargetDistance(playerRef);
		if (distance < 50)
		{
			animation.playerState = walk;
			int velX, velY;
			if (playerRef->GetMovement().position.x > movement.position.x)
			{
				velX = -1;
			}
			else
			{
				velX = 1;
			}

			if (playerRef->GetMovement().position.y > movement.position.y)
			{
				velY = -1;
			}
			else
			{
				velY = 1;
			}

			AddVelocity(velX, velY);
		}
		else if (distance > 200)
		{
			if (MoveToPawn(playerRef))
			{
				animation.playerState = walk;
			}
			else
			{
				animation.playerState = idle;
			}
		}
		else
		{
			animation.playerState = idle;
		}
	}

	Pawn::UpdateMovement();
	
}

void NPCs::AssignPlayer(Player* player) 
{
	playerRef = player;
}

//----------------Begin play---------------
void Actor::BeginPlay() 
{
	//begin play
}

void Player::BeginPlay() 
{
	clTag = player;
}

void Enemy::BeginPlay() 
{
	animation.SetAnimationSources(idle, LoadTexture("Assets/ChikBoy_idle.png"), 6);
	animation.SetAnimationSources(walk, LoadTexture("Assets/ChikBoy_run.png"), 10);
	Movement initMov = { movement.position, {0,0}, 1 };
	SetMovement(initMov);
}

void Ally::BeginPlay() 
{
	animation.SetAnimationSources(idle, LoadTexture("Assets/MeowKnight/Meow-Knight_Idle.png"), 6);
	animation.SetAnimationSources(walk, LoadTexture("Assets/MeowKnight/Meow-Knight_Run.png"), 10);
	Movement initMov = { movement.position, {0,0}, 2 };
	SetMovement(initMov);
	clTag = ally;
}

// ----------------Tick---------------
void Actor::Tick() 
{
	//Tick
}

void Pawn::Tick() 
{
	//Pawn tick
	UpdateMovement();
	animation.ShowAnimation(GetMovement());

	if(drawCollisions)
	{
		DrawRectangle(rec.x, rec.y, rec.width, rec.height, WHITE);
	}
	
	ResetCollisions();
	CheckCollisionWithScreen();
	CalculateVulnerability();
}

void Player::Tick() 
{
	Pawn::Tick();
	
	//Player Tick
	CalculateCountdown(&spawnCatAbility);

}

void NPCs::Tick() 
{
	Pawn::Tick();
	if (!(playerRef == nullptr))
	{
		//CheckCollision(dynamic_cast<Pawn*>(playerRef));
	}
}

void Enemy::Tick() 
{
	NPCs::Tick();
	if (GetTargetDistance(playerRef) < 500) 
	{
		if (MoveToPawn(playerRef))
		{
			animation.playerState = walk;
		}
		else
		{
			animation.playerState = idle;
		}
	}
	else 
	{
		animation.playerState = idle;
	}
}

void Pawn::ResetCollisions() 
{
	movement.collision.top = false;
	movement.collision.bottom = false;
	movement.collision.left = false;
	movement.collision.right = false;
}
