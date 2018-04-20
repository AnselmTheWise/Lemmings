#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	WALKING_LEFT, WALKING_RIGHT, FALLING_LEFT, FALLING_RIGHT, EXITING
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	state = FALLING_RIGHT_STATE;
	spritesheet.loadFromFile("images/lemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);
	
		sprite->setAnimationSpeed(WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 8, 0.0f));
		
		sprite->setAnimationSpeed(WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 8, 0.5f));
		
	sprite->changeAnimation(WALKING_RIGHT);
	sprite->setPosition(initialPosition);

	fallingSpritesheet.loadFromFile("images/falling.png", TEXTURE_PIXEL_FORMAT_RGBA);
	fallingSpritesheet.setMinFilter(GL_NEAREST);
	fallingSpritesheet.setMagFilter(GL_NEAREST);
	fallingSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 0.5), &fallingSpritesheet, &shaderProgram);
	fallingSprite->setNumberAnimations(16);

	fallingSprite->setAnimationSpeed(FALLING_RIGHT, 24);
	for (int i = 0; i<4; i++)
		fallingSprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / 4, 0.0f));

	fallingSprite->setAnimationSpeed(FALLING_LEFT, 24);
	for (int i = 0; i<4; i++)
		fallingSprite->addKeyframe(FALLING_LEFT, glm::vec2(float(i) / 4, 0.5f));

	fallingSprite->changeAnimation(FALLING_RIGHT);
	fallingSprite->setPosition(initialPosition);

	exitSpritesheet.loadFromFile("images/exitLemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	exitSpritesheet.setMinFilter(GL_NEAREST);
	exitSpritesheet.setMagFilter(GL_NEAREST);
	exitSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 1), &exitSpritesheet, &shaderProgram);
	exitSprite->setNumberAnimations(8);
	exitSprite->setAnimationSpeed(EXITING, 24);
	for (int i = 0; i<8; i++)
		exitSprite->addKeyframe(EXITING, glm::vec2(float(i) / 8., 0.0f));

	exitSprite->changeAnimation(EXITING);
	exitSprite->setPosition(initialPosition);
	spritesheetIQ.loadFromFile("images/lemmingUI.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetIQ.setMinFilter(GL_NEAREST);
	spritesheetIQ.setMagFilter(GL_NEAREST);
	yOffset = 3;
	status = 0;
	exitDoorPosition = glm::vec2(238.5f, 121.5f);
	glm::vec2 initPos = initialPosition;
	interactiveQuad = InteractiveQuad::createInteractiveQuad(glm::vec2(initPos.x, initPos.y + yOffset), glm::vec2(16, 16), glm::vec2(1./3., 1), &spritesheetIQ, &shaderProgram);
	interactiveQuad->setOffsetIdle(glm::vec2(0.f, 0.f));
	interactiveQuad->setOffsetHover(glm::vec2(1./3., 0.f));
	interactiveQuad->setOffsetClick(glm::vec2(2. / 3., 0.f));
	fallingConsec = 0;
}

void Lemming::update(int deltaTime)
{
	int fall;

	if(sprite->update(deltaTime) == 0)
		return;
	
	if (fallingConsec > 100) {
		status = 2;
	}

	switch(state)
	{
	case FALLING_LEFT_STATE:
		fallingSprite->update(deltaTime);
		fall = collisionFloor(2);
		if (fall > 0) {
			sprite->position() += glm::vec2(0, fall);
			interactiveQuad->position() += glm::vec2(0, fall);
			++fallingConsec;
		}
		else {
			state = WALKING_LEFT_STATE;
			sprite->changeAnimation(WALKING_LEFT);
			fallingConsec = 0;
		}
		break;
	case FALLING_RIGHT_STATE:
		fallingSprite->update(deltaTime);
		fall = collisionFloor(2);
		if (fall > 0) {
			sprite->position() += glm::vec2(0, fall);
			interactiveQuad->position() += glm::vec2(0, fall);
			++fallingConsec;
		}
		else {
			state = WALKING_RIGHT_STATE;
			sprite->changeAnimation(WALKING_RIGHT);
			fallingConsec = 0;
		}
		break;
	case WALKING_LEFT_STATE:
		if (sprite->position().x <= exitDoorPosition.x + 10 && sprite->position().x >= exitDoorPosition.x - 10 &&
			sprite->position().y <= exitDoorPosition.x + 10 && sprite->position().y >= exitDoorPosition.y - 10) {
			status = 0;
			sprite->changeAnimation(EXITING);
			state = EXITING_STATE;
			break;
		}
		sprite->position() += glm::vec2(-1, -1);
		interactiveQuad->position() += glm::vec2(-1, -1);
		if(collision())
		{
			sprite->position() -= glm::vec2(-1, -1);
			interactiveQuad->position() -= glm::vec2(-1, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall > 0) {
				sprite->position() += glm::vec2(0, 1);
				interactiveQuad->position() += glm::vec2(0, 1);
			}
			if (fall > 1) {
				sprite->position() += glm::vec2(0, 1);
				interactiveQuad->position() += glm::vec2(0, 1);
			}
			if (fall > 2) {
				state = FALLING_LEFT_STATE;
				fallingSprite->changeAnimation(FALLING_LEFT);
			}
		}
		break;
	case WALKING_RIGHT_STATE:
		if (sprite->position().x <= exitDoorPosition.x + 10 && sprite->position().x >= exitDoorPosition.x - 10 &&
			sprite->position().y <= exitDoorPosition.x + 10 && sprite->position().y >= exitDoorPosition.y - 10) {
			status = 0;
			sprite->changeAnimation(EXITING);
			state = EXITING_STATE;
			break;
		}
		sprite->position() += glm::vec2(1, -1);
		interactiveQuad->position() += glm::vec2(1, -1);
		if(collision())
		{
			sprite->position() -= glm::vec2(1, -1);
			interactiveQuad->position() -= glm::vec2(1, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall < 3) {
				sprite->position() += glm::vec2(0, fall);
				interactiveQuad->position() += glm::vec2(0, fall);
			}
			else {
				state = FALLING_RIGHT_STATE;
				fallingSprite->changeAnimation(FALLING_RIGHT);
			}
		}
		break;
	case EXITING_STATE:
		timeExiting += deltaTime;
		exitSprite->update(deltaTime);
		if (timeExiting > 800*(12.f/30.f)) {
			status = 1;
		}
		break;
	}
	exitSprite->position() = sprite->position();
	fallingSprite->position() = sprite->position();
}

int Lemming::getStatus() {
	return status;
}

void Lemming::render()
{
	if (state == EXITING) {
		exitSprite->render();
	}
	else if (state == FALLING_RIGHT_STATE || state == FALLING_LEFT_STATE) {
		fallingSprite->render();
	}
	else {
		sprite->render();
	}
	interactiveQuad->render();
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

void Lemming::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton) {
	interactiveQuad->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) == 0) && (mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}







