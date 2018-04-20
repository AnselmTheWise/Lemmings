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
	WALKING_LEFT, WALKING_RIGHT, FALLING_LEFT, FALLING_RIGHT, STOPPING, DIGGING, BASHING_LEFT, BASHING_RIGHT,
	CLIMBING_LEFT, CLIMBING_RIGHT, EXITING
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

	stoppingSpritesheet.loadFromFile("images/stopper.png", TEXTURE_PIXEL_FORMAT_RGBA);
	stoppingSpritesheet.setMinFilter(GL_NEAREST);
	stoppingSpritesheet.setMagFilter(GL_NEAREST);
	stoppingSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625f, 1.f), &stoppingSpritesheet, &shaderProgram);
	stoppingSprite->setNumberAnimations(16);

	stoppingSprite->setAnimationSpeed(STOPPING, 12);
	for (int i = 0; i<16; i++)
		stoppingSprite->addKeyframe(STOPPING, glm::vec2(float(i) / 16, 0.0f));

	stoppingSprite->changeAnimation(STOPPING);
	stoppingSprite->setPosition(initialPosition);

	diggingSpritesheet.loadFromFile("images/digger.png", TEXTURE_PIXEL_FORMAT_RGBA);
	diggingSpritesheet.setMinFilter(GL_NEAREST);
	diggingSpritesheet.setMagFilter(GL_NEAREST);
	diggingSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125f, 1.f), &diggingSpritesheet, &shaderProgram);
	diggingSprite->setNumberAnimations(16);

	diggingSprite->setAnimationSpeed(DIGGING, 24);
	for (int i = 0; i<8; i++)
		diggingSprite->addKeyframe(DIGGING, glm::vec2(float(i) / 8, 0.0f));

	diggingSprite->changeAnimation(DIGGING);
	diggingSprite->setPosition(initialPosition);

	bashingSpritesheet.loadFromFile("images/basher2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	bashingSpritesheet.setMinFilter(GL_NEAREST);
	bashingSpritesheet.setMagFilter(GL_NEAREST);
	bashingSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.5), &bashingSpritesheet, &shaderProgram);
	bashingSprite->setNumberAnimations(16);

	bashingSprite->setAnimationSpeed(BASHING_RIGHT, 32);
	for (int i = 0; i<16; i++)
		bashingSprite->addKeyframe(BASHING_RIGHT, glm::vec2(float(i) / 16, 0.0f));

	bashingSprite->setAnimationSpeed(BASHING_LEFT, 32);
	for (int i = 0; i<16; i++)
		bashingSprite->addKeyframe(BASHING_LEFT, glm::vec2(float(i) / 16, 0.5f));

	bashingSprite->changeAnimation(BASHING_RIGHT);
	bashingSprite->setPosition(initialPosition);

	climbingSpritesheet.loadFromFile("images/climber2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	climbingSpritesheet.setMinFilter(GL_NEAREST);
	climbingSpritesheet.setMagFilter(GL_NEAREST);
	climbingSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625f, 0.5f), &climbingSpritesheet, &shaderProgram);
	climbingSprite->setNumberAnimations(16);

	climbingSprite->setAnimationSpeed(CLIMBING_RIGHT, 24);
	for (int i = 0; i<16; i++)
		climbingSprite->addKeyframe(DIGGING, glm::vec2(float(i) / 16, 0.0f));

	climbingSprite->setAnimationSpeed(CLIMBING_LEFT, 24);
	for (int i = 0; i<16; i++)
		climbingSprite->addKeyframe(CLIMBING_LEFT, glm::vec2(float(i) / 16, 0.5f));

	climbingSprite->changeAnimation(CLIMBING_LEFT);
	climbingSprite->setPosition(initialPosition);

	exitSpritesheet.loadFromFile("images/exitLemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	exitSpritesheet.setMinFilter(GL_NEAREST);
	exitSpritesheet.setMagFilter(GL_NEAREST);
	exitSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 1), &exitSpritesheet, &shaderProgram);
	exitSprite->setNumberAnimations(16);
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
		cout << "Updating exit" << endl;
		exitSprite->update(deltaTime);
		if (timeExiting > 800*(12.f/30.f)) {
			status = 1;
		}
		break;
	case STOPPING_STATE:
		stoppingSprite->update(deltaTime);
		break;
	case DIGGING_STATE:
		fall = collisionFloor(3);
		if (fall > 1) {
			state = FALLING_RIGHT_STATE;
			fallingSprite->changeAnimation(FALLING_RIGHT);
		}
		else {
			diggingSprite->update(deltaTime);
			digMask();
			sprite->position().y += 0.5;
		}
		break;
	case BASHING_LEFT_STATE:
		sprite->position() += glm::vec2(-4, -1);
		interactiveQuad->position() += glm::vec2(-4, -1);
		if (!collisionTest("left", 8))
		{
			sprite->position() -= glm::vec2(-4, -1);
			interactiveQuad->position() -= glm::vec2(-4, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall > 0) {
				sprite->position() += glm::vec2(3, 1);
				interactiveQuad->position() += glm::vec2(0, 1);
				bashMask("left");
				bashingSprite->update(deltaTime);
			}
			if (fall > 1) {
				sprite->position() += glm::vec2(3, 1);
				interactiveQuad->position() += glm::vec2(0, 1);
				bashMask("left");
				bashingSprite->update(deltaTime);
			}
			if (fall > 2) {
				state = FALLING_LEFT_STATE;
				fallingSprite->changeAnimation(FALLING_LEFT);
			}
			if (fall == 0) {
				sprite->position() += glm::vec2(3, 1);
				interactiveQuad->position() += glm::vec2(0, 1);
				bashMask("left");
				bashingSprite->update(deltaTime);
			}
		}
		break;
	case BASHING_RIGHT_STATE:
		sprite->position() += glm::vec2(4, -1);
		interactiveQuad->position() += glm::vec2(4, -1);
		if (!collisionTest("right", 8))
		{
			sprite->position() -= glm::vec2(4, -1);
			interactiveQuad->position() -= glm::vec2(4, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall > 0) {
				sprite->position() += glm::vec2(-3, 1);
				interactiveQuad->position() += glm::vec2(0, 1);
				bashMask("right");
				bashingSprite->update(deltaTime);
			}
			if (fall > 1) {
				sprite->position() += glm::vec2(-3, 1);
				interactiveQuad->position() += glm::vec2(0, 1);
				bashMask("right");
				bashingSprite->update(deltaTime);
			}
			if (fall > 2) {
				state = FALLING_RIGHT_STATE;
				fallingSprite->changeAnimation(FALLING_RIGHT);
			}
			if (fall == 0) {
				sprite->position() += glm::vec2(-3, 1);
				interactiveQuad->position() += glm::vec2(0, 1);
				bashMask("right");
				bashingSprite->update(deltaTime);
			}
		}
		break;
	case WALKING_LEFT_TO_CLIMB_STATE:
		if (sprite->position().x <= exitDoorPosition.x + 10 && sprite->position().x >= exitDoorPosition.x - 10 &&
			sprite->position().y <= exitDoorPosition.x + 10 && sprite->position().y >= exitDoorPosition.y - 10) {
			status = 0;
			sprite->changeAnimation(EXITING);
			state = EXITING_STATE;
			break;
		}
		sprite->position() += glm::vec2(-1, -1);
		interactiveQuad->position() += glm::vec2(-1, -1);
		if (collision() && canClimb("left"))
		{
			state = CLIMBING_LEFT_STATE;
			climbingSprite->changeAnimation(CLIMBING_LEFT);
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
	case WALKING_RIGHT_TO_CLIMB_STATE:
		if (sprite->position().x <= exitDoorPosition.x + 10 && sprite->position().x >= exitDoorPosition.x - 10 &&
			sprite->position().y <= exitDoorPosition.x + 10 && sprite->position().y >= exitDoorPosition.y - 10) {
			status = 0;
			sprite->changeAnimation(EXITING);
			state = EXITING_STATE;
			break;
		}
		sprite->position() += glm::vec2(1, -1);
		interactiveQuad->position() += glm::vec2(1, -1);
		if (collision() && canClimb("right"))
		{
			state = CLIMBING_RIGHT_STATE;
			climbingSprite->changeAnimation(CLIMBING_RIGHT);
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
	}
	exitSprite->position() = sprite->position();
	fallingSprite->position() = sprite->position();
	stoppingSprite->position() = glm::vec2(sprite->position().x, sprite->position().y+1);
	diggingSprite->position() = sprite->position();
	bashingSprite->position() = sprite->position();
	climbingSprite->position() = sprite->position();
	interactiveQuad->position() = sprite->position();
}

bool Lemming::setPower(int power) {
	if (power == 0) {
		if (state != FALLING_LEFT_STATE && state != FALLING_RIGHT_STATE) {
			state = STOPPING_STATE;
			addBlocking();
			return true;
		}
	}
	else if (power == 1) {
		if (state != FALLING_LEFT_STATE && state != FALLING_RIGHT_STATE) {
			state = DIGGING_STATE;
			return true;
		}
	}
	else if (power == 2) {
		if (state != FALLING_LEFT_STATE && state != FALLING_RIGHT_STATE) { //Check if collision
			if (state == WALKING_LEFT_STATE) {
				state = BASHING_LEFT_STATE;
				bashingSprite->changeAnimation(BASHING_LEFT);
			}
			else if (state == WALKING_RIGHT_STATE) {
				state = BASHING_RIGHT_STATE;
				bashingSprite->changeAnimation(BASHING_RIGHT);
			}
			return true;
		}
	}
	else if (power == 3) {
		if (state != FALLING_LEFT_STATE && state != FALLING_RIGHT_STATE) {
			if (state == WALKING_RIGHT_STATE) {
				if (collisionTest("right", 8)) {
					state = WALKING_RIGHT_TO_CLIMB_STATE;
				}
			}
			else if (state == WALKING_LEFT_STATE) {
				if (collisionTest("left", 8)) {
					state = WALKING_LEFT_TO_CLIMB_STATE;
				}
			}
		}
	}
	return false;
}

int Lemming::getStatus() {
	return status;
}

void Lemming::render()
{
	if (state == EXITING_STATE) {
		cout << "Rendering exit" << endl;
		exitSprite->render();
	}
	else if (state == FALLING_RIGHT_STATE || state == FALLING_LEFT_STATE) {
		fallingSprite->render();
	}
	else if (state == STOPPING_STATE) {
		stoppingSprite->render();
	}
	else if (state == DIGGING_STATE) {
		diggingSprite->render();
	}
	else if (state == BASHING_RIGHT_STATE || state == BASHING_LEFT_STATE) {
		bashingSprite->render();
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

bool Lemming::isClicked() {
	return interactiveQuad->isClicked();
}

void Lemming::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton) {
	interactiveQuad->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
}

void Lemming::addBlocking() {
	for (int i = sprite->position().x + 4; i < sprite->position().x + 12; ++i) {
		for (int j = sprite->position().y + 6; j < sprite->position().y + 16; ++j) {
			mask->setPixel((i*960/CAMERA_WIDTH) / 3 + 120, (j*960 / CAMERA_WIDTH) / 3, 255);
		}
	}
}

void Lemming::digMask() {
	int x = sprite->position().x;
	int y = sprite->position().y + 16;
	for (int i = x + 2; i < x + 12; ++i) {
		mask->setPixel((i * 960 / CAMERA_WIDTH) / 3 + 120, (y * 960 / CAMERA_WIDTH) / 3, 0);
	}
}

void Lemming::bashMask(string way) {
	for (int i = sprite->position().x + 4; i < sprite->position().x + 12; ++i) {
		for (int j = sprite->position().y + 6; j < sprite->position().y + 16; ++j) {
			mask->setPixel((i * 960 / CAMERA_WIDTH) / 3 + 120, (j * 960 / CAMERA_WIDTH) / 3, 0);
		}
	}
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

bool Lemming::collisionTest(string way, int dist)
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	posBase += glm::ivec2(7, 15);
	if (way == "right") {
		for (int i = posBase.x; i < posBase.x + dist; ++i) {
			if (mask->pixel(i, posBase.y) == 255) return true;
		}
	}
	else if (way == "left") {
		for (int i = posBase.x; i > posBase.x - dist; --i) {
			if (mask->pixel(i, posBase.y) == 255) return true;
		}
	}
	return false;
}

bool Lemming::canClimb(string way) {
	return false;
}





