//Movement header
#pragma once
#include <cstdio>
#include <iostream>

using namespace std;

//Stores sides where object has collision
struct collisionCoordinates
{
	bool top = 0;
	bool bottom = 0;
	bool left = 0;
	bool right = 0;
};

//Movement struct holds information for movement of an object
struct Movement
{
public:
	Vector2 position = { 0,0 };
	Vector2 velocity = { 0,0 };
	float speed = 5;
	collisionCoordinates collision;
};
