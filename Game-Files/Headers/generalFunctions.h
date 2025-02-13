//general functions
#include <cstdio>
#include <iostream>
#include "raylib.h"

struct ScreenSize;

// extern Game gameRef;

void DrawLife(int life, int maxLife, float posX, float posY, int size);

int RandomNumInRange(int min, int max);

int Clamp(int value, int min, int max);

Vector2 MakeRandomLocation(ScreenSize screenSize, int screenOffset);
