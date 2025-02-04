//animation header
#pragma once
#include "raylib.h"
#include "movementHeader.h"

//float frameSize[2] = { 0,0 };

struct coordinates 
{
	float x = 0;
	float y = 0;
};

struct sheetInfo 
{
	Texture2D texture;
	int frameAmount;
};

struct Spritesheets
{
	sheetInfo idle = { LoadTexture("Assets/B_witch_idle.png"), 6 };
	sheetInfo walk = { LoadTexture("Assets/B_witch_run.png"), 8 };
};

enum AnimationState {idle, walk};

class Animation 
{
	public:
		int facing = 1;
		float imagesizeMultiplier = 1.5;
		float minImageSizeMultiplier = 0.5;
		float maxImageSizeMultiplier = 4;
		AnimationState playerState = idle;

		void ShowAnimation(Movement movement);
		void SetAnimationSources(AnimationState animState, Texture2D texture, int amountOfFrames);

		Texture2D CalculateFrame();
		coordinates GetImageSize();

	protected:

	private:
		coordinates imagePosition;
		coordinates imageSize;
		Spritesheets animations;
		int currentFrame = 0;
		Texture2D spritesheet = LoadTexture("Assets/B_witch_idle.png");
		float timer = 0.0;
		Rectangle animRec{ 0.0,0.0,100.0,100.0 };
};
