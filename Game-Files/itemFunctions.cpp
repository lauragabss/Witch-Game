#include "Headers/itemsHeader.h"
#include "Headers/generalFunctions.h"


// *** POTION ***

void Potion::BeginPlay()
{
    string filName = "Assets/" + spriteName;
    animation.SetAnimationSources(idle, LoadTexture(filName.c_str()), 4);
    animation.SetAnimationSources(walk, LoadTexture(filName.c_str()), 4);
    Movement initMov = { movement.position, {0,0}, 1 };
    SetMovement(initMov);
    shouldDrawLife = false;
}

void Potion::CollideWithPlayer(Player* player)
{
    Pawn::CollideWithPlayer(player);

    // Destroy self
    shouldDestroy = true;
}

// *** MANA POTION ***

void ManaPotion::BeginPlay()
{
    Potion::BeginPlay();

    buffValue = thisBuffValue;
}

void ManaPotion::CollideWithPlayer(Player* player)
{
    // Call effect
    player->AddMana(buffValue);

    if (gameRef != nullptr && !gameRef->winGame)
    {
        ManaPotion* potion = new ManaPotion(MakeRandomLocation(gameRef->screenSize, 100));
        gameRef->AddPawn(potion);

        // create enemies
        for (int i = 0; i < gameRef->allies.size(); i++)
        {
            gameRef->SpawnEnemy();
        }

        // chance of creating life potion
        int randomNumber = GetRandomValue(0, 10);
        if(randomNumber <= 2)
        {
            LifePotion* lpotion = new LifePotion(MakeRandomLocation(gameRef->screenSize, 100));
            gameRef->AddPawn(lpotion);
        }
    }

    Potion::CollideWithPlayer(player);
}

// *** LIFE POTION ***

void LifePotion::BeginPlay()
{
    buffValue = thisBuffValue;
    spriteName = "RedPotion.png";

    Potion::BeginPlay();
}

void LifePotion::CollideWithPlayer(Player* player)
{
    // Call effect
    player->ReceiveBlessing(buffValue);

    Potion::CollideWithPlayer(player);
}
