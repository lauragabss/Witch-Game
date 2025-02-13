#include "Headers/itemsHeader.h"


void Potion::BeginPlay()
{
    animation.SetAnimationSources(idle, LoadTexture("Assets/Potion.png"), 4);
    animation.SetAnimationSources(walk, LoadTexture("Assets/Potion.png"), 4);
    Movement initMov = { movement.position, {0,0}, 1 };
    SetMovement(initMov);
    shouldDrawLife = false;
}

void Potion::CollideWithPlayer(Player* player)
{
    Pawn::CollideWithPlayer(player);

    // Call effect
    player->AddMana(buffValue);

    // Destroy self
    shouldDestroy = true;
}
