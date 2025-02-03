//animation functions
#include "header.h"
#include "animationHeader.h"
#include <cstdio>


Texture2D Animation::CalculateFrame() 
{
	
	float spriteWidth, spriteHeight;
	sheetInfo currentSheet;
	if (playerState == idle) 
	{
		currentSheet = animations.idle;
	}
	else if (playerState == walk) 
	{
		currentSheet = animations.walk;
	}
	else 
	{
		currentSheet = animations.walk;
	}
	spritesheet = currentSheet.texture;
	
	//Calculate frame time
	timer += GetFrameTime();

	if (timer >= 0.2f)
	{
		timer = 0;
		currentFrame += 1;
	}

	currentFrame = currentFrame % currentSheet.frameAmount;

	//calculate width and height of frames
	spriteWidth = spritesheet.width;
	spriteHeight = spritesheet.height / currentSheet.frameAmount;

	//calculate position of current sprite
	imagePosition.x = spritesheet.width;
	imagePosition.y = spriteHeight * currentFrame;
	imageSize.x = spriteWidth;
	imageSize.y = spriteHeight;

	//Update animRec
	animRec.x = imagePosition.x;
	animRec.y = imagePosition.y;
	animRec.width = imageSize.x;
	animRec.height = imageSize.y;

	return currentSheet.texture;
	
}

void Animation::SetAnimationSources(AnimationState animState, Texture2D texture, int amountOfFrames)
{
	if (animState == idle) 
	{
		animations.idle.texture = texture;
		animations.idle.frameAmount = amountOfFrames;
	}
	else if (animState == walk) 
	{
		animations.walk.texture = texture;
		animations.walk.frameAmount = amountOfFrames;
	}
}

void Animation::ShowAnimation(Movement movement)
{
	Rectangle drawnRect = animRec;
	drawnRect.width = drawnRect.width * facing;

	DrawTexturePro(
		CalculateFrame(),
		drawnRect,
		{ movement.position.x, movement.position.y, imageSize.x * imagesizeMultiplier, imageSize.y * imagesizeMultiplier },
		Vector2{0,0},
		0,
		WHITE
	);
}

coordinates Animation::GetImageSize() 
{
	return imageSize;
}
