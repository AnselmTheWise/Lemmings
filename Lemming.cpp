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
	CLIMBING_LEFT, CLIMBING_RIGHT, BUILDING_LEFT, BUILDING_RIGHT, PLACING_PORTAL_LEFT, PLACING_PORTAL_RIGHT, 
	ORANGE_PORTAL, BLUE_PORTAL, EXITING, EXPLODING
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	shader = shaderProgram;
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
	for (int i = 0; i<8; i++)
		climbingSprite->addKeyframe(CLIMBING_RIGHT, glm::vec2(float(i) / 16, 0.0f));

	climbingSprite->setAnimationSpeed(CLIMBING_LEFT, 24);
	for (int i = 0; i<16; i++)
		climbingSprite->addKeyframe(CLIMBING_LEFT, glm::vec2(float(i) / 16, 0.5f));

	climbingSprite->changeAnimation(CLIMBING_RIGHT);
	climbingSprite->setPosition(initialPosition);

	buildingSpritesheet.loadFromFile("images/builder2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buildingSpritesheet.setMinFilter(GL_NEAREST);
	buildingSpritesheet.setMagFilter(GL_NEAREST);
	buildingSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625f, 0.5f), &buildingSpritesheet, &shaderProgram);
	buildingSprite->setNumberAnimations(16);

	buildingSprite->setAnimationSpeed(BUILDING_RIGHT, 24);
	for (int i = 0; i<16; i++)
		buildingSprite->addKeyframe(BUILDING_RIGHT, glm::vec2(float(i) / 16, 0.0f));

	buildingSprite->setAnimationSpeed(BUILDING_LEFT, 24);
	for (int i = 15; i>=0; i--)
		buildingSprite->addKeyframe(BUILDING_LEFT, glm::vec2(float(i) / 16, 0.5f));

	buildingSprite->changeAnimation(BUILDING_RIGHT);
	buildingSprite->setPosition(initialPosition);

	portalPlacingSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625f, 0.5f), &buildingSpritesheet, &shaderProgram);
	portalPlacingSprite->setNumberAnimations(16);

	portalPlacingSprite->setAnimationSpeed(PLACING_PORTAL_RIGHT, 24);
	for (int i = 0; i<16; i++)
		portalPlacingSprite->addKeyframe(PLACING_PORTAL_RIGHT, glm::vec2(float(i) / 16, 0.0f));

	portalPlacingSprite->setAnimationSpeed(PLACING_PORTAL_LEFT, 24);
	for (int i = 15; i >= 0; i--)
		portalPlacingSprite->addKeyframe(PLACING_PORTAL_LEFT, glm::vec2(float(i) / 16, 0.5f));

	portalPlacingSprite->changeAnimation(PLACING_PORTAL_RIGHT);
	portalPlacingSprite->setPosition(initialPosition);

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

	explodingSpritesheet.loadFromFile("images/explodingLemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	explodingSpritesheet.setMinFilter(GL_NEAREST);
	explodingSpritesheet.setMagFilter(GL_NEAREST);
	explodingSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1. / 14., 1), &exitSpritesheet, &shaderProgram);
	explodingSprite->setNumberAnimations(16);
	explodingSprite->setAnimationSpeed(EXPLODING, 24);
	for (int i = 0; i<14; i++)
		explodingSprite->addKeyframe(EXPLODING, glm::vec2(float(i) / 14., 0.0f));

	explodingSprite->changeAnimation(EXPLODING);
	explodingSprite->setPosition(initialPosition);

	woodTexture.loadFromFile("images/wood.png", TEXTURE_PIXEL_FORMAT_RGBA);
	woodTexture.setMinFilter(GL_NEAREST);
	woodTexture.setMagFilter(GL_NEAREST);

	spritesheetIQ.loadFromFile("images/lemmingUI.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetIQ.setMinFilter(GL_NEAREST);
	spritesheetIQ.setMagFilter(GL_NEAREST);
	yOffset = 3;
	status = 0;
	glm::vec2 initPos = initialPosition;
	interactiveQuad = InteractiveQuad::createInteractiveQuad(glm::vec2(initPos.x, initPos.y + yOffset), glm::vec2(16, 16), glm::vec2(1./3., 1), &spritesheetIQ, &shaderProgram);
	interactiveQuad->setOffsetIdle(glm::vec2(0.f, 0.f));
	interactiveQuad->setOffsetHover(glm::vec2(1./3., 0.f));
	interactiveQuad->setOffsetClick(glm::vec2(2. / 3., 0.f));
	fallingConsec = 0;
	upped = false;
	bluePortalSpritesheet.loadFromFile("images/bluePortal.png", TEXTURE_PIXEL_FORMAT_RGBA);
	orangePortalSpritesheet.loadFromFile("images/orangePortal.png", TEXTURE_PIXEL_FORMAT_RGBA);
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
		if (!collisionTestHigh("left", 18))
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
		if (!collisionTestHigh("right", 18))
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
		if (collisionTestHigh("left", 8))
		{
			state = CLIMBING_LEFT_STATE;
			climbingSprite->changeAnimation(CLIMBING_LEFT);
			sprite->position() -= glm::vec2(-1, -1);
			interactiveQuad->position() -= glm::vec2(-1, -1);
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
		if (collisionTestHigh("right", 8))
		{
			state = CLIMBING_RIGHT_STATE;
			climbingSprite->changeAnimation(CLIMBING_RIGHT);
			sprite->position() -= glm::vec2(1, -1);
			interactiveQuad->position() -= glm::vec2(1, -1);
		}
		else
		{
			cout << "No collision at my right" << endl;
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
				state = FALLING_RIGHT_STATE;
				fallingSprite->changeAnimation(FALLING_RIGHT);
			}
			if (fall == 0) {
				cout << "And next pixel is above" << endl;
			}
		}
		break;
	case CLIMBING_LEFT_STATE:
		climbingSprite->update(deltaTime);
		if (!collisionHandLevel("left")) {
			if (canClimb("left")) {
				//Final climb
				sprite->position() += glm::vec2(-6, -8);
				interactiveQuad->position() += glm::vec2(-1, -8);
				state = WALKING_LEFT_STATE;
				sprite->changeAnimation(WALKING_LEFT);
			}
			else {
				state = FALLING_LEFT_STATE;
				fallingSprite->changeAnimation(FALLING_LEFT);
			}
		}
		else if (collisionHeadLevel("left")) {
			state = FALLING_LEFT_STATE;
			fallingSprite->changeAnimation(FALLING_LEFT);
		}
		else {
			//Still climb
			sprite->position() += glm::vec2(0, -1);
			interactiveQuad->position() += glm::vec2(0, -1);
			climbingSprite->changeAnimation(CLIMBING_LEFT);
		}
		break;
	case CLIMBING_RIGHT_STATE:
		climbingSprite->update(deltaTime);
		if (!collisionHandLevel("right")) {
			cout << "No collision in hand level" << endl;
			if (canClimb("right")) {
				cout << "Can climb to the top" << endl;
				//Final climb
				sprite->position() += glm::vec2(6, -8);
				interactiveQuad->position() += glm::vec2(1, -8);
				state = WALKING_RIGHT_STATE;
				sprite->changeAnimation(WALKING_RIGHT);
			}
			else {
				cout << "Can't climb to the top, falling" << endl;
				state = FALLING_RIGHT_STATE;
				fallingSprite->changeAnimation(FALLING_RIGHT);
			}
		}
		else if (collisionHeadLevel("right")) {
			cout << "Collision with head level, falling" << endl;
			state = FALLING_RIGHT_STATE;
			fallingSprite->changeAnimation(FALLING_RIGHT);
		}
		else {
			//Still climb
			cout << "No conflicts at all, climbing" << endl;
			sprite->position() += glm::vec2(0, -1);
			interactiveQuad->position() += glm::vec2(0, -1);
			climbingSprite->changeAnimation(CLIMBING_RIGHT);
		}
		break;
	case BUILDING_LEFT_STATE:
		if (woodCount == 13) {
			state = WALKING_LEFT_STATE;
			sprite->position() += glm::vec2(2, -2);
			sprite->changeAnimation(WALKING_LEFT);
		}
		else {
			buildingSprite->update(deltaTime);
			if (buildingSprite->getFrame() == 15 && !upped) {
				sprite->position() += glm::vec2(-2, -1);
				upped = true;
			}
			else if (buildingSprite->getFrame() != 15) {
				upped = false;
			}
			if (buildingSprite->getFrame() == 9) {
				Sprite* newWood = Sprite::createSprite(glm::ivec2(4, 1), glm::vec2(1, 1), &woodTexture, &shader);
				newWood->setNumberAnimations(1);
				newWood->setAnimationSpeed(WOOD, 24);
				newWood->addKeyframe(WOOD, glm::vec2(0.0f, 0.0f));
				newWood->changeAnimation(WOOD);
				glm::vec2 woodPos = sprite->position() + glm::vec2(2, 13);
				newWood->setPosition(woodPos);
				woods.push_back(newWood);
				buildWood(woodPos);
				++woodCount;
			}
		}
		break;
	case BUILDING_RIGHT_STATE:
		if (woodCount == 13) {
			state = WALKING_RIGHT_STATE;
			sprite->position() += glm::vec2(2, -2);
			sprite->changeAnimation(WALKING_RIGHT);
		}
		else {
			buildingSprite->update(deltaTime);
			if (buildingSprite->getFrame() == 15 && !upped) {
				sprite->position() += glm::vec2(2, -1);
				upped = true;
			}
			else if (buildingSprite->getFrame() != 15) {
				upped = false;
			}
			if (buildingSprite->getFrame() == 9) {
				Sprite* newWood = Sprite::createSprite(glm::ivec2(4, 1), glm::vec2(1, 1), &woodTexture, &shader);
				newWood->setNumberAnimations(1);
				newWood->setAnimationSpeed(WOOD, 24);
				newWood->addKeyframe(WOOD, glm::vec2(0.0f, 0.0f));
				newWood->changeAnimation(WOOD);
				glm::vec2 woodPos = sprite->position() + glm::vec2(9, 13);
				newWood->setPosition(woodPos);
				woods.push_back(newWood);
				buildWood(woodPos);
				++woodCount;
			}
			break;
	case PLACING_PORTAL_LEFT_STATE:
		portalPlacingSprite->update(deltaTime);
		if (portalPlacingSprite->getFrame() == 9) {
			placePortal("left");
		}
		else if (portalPlacingSprite->getFrame() == 15) {
			state = WALKING_LEFT_STATE;
		}
		break;
	case PLACING_PORTAL_RIGHT_STATE:
		portalPlacingSprite->update(deltaTime);
		if (portalPlacingSprite->getFrame() == 9) {
			placePortal("right");
		}
		else if (portalPlacingSprite->getFrame() == 15) {
			state = WALKING_RIGHT_STATE;
		}
		break;
		}
	}
	exitSprite->position() = sprite->position();
	fallingSprite->position() = sprite->position();
	stoppingSprite->position() = glm::vec2(sprite->position().x, sprite->position().y+1);
	diggingSprite->position() = sprite->position();
	bashingSprite->position() = sprite->position();
	climbingSprite->position() = sprite->position();
	buildingSprite->position() = sprite->position();
	portalPlacingSprite->position() = sprite->position();
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
		if (state != FALLING_LEFT_STATE && state != FALLING_RIGHT_STATE) {
			if (state == WALKING_LEFT_STATE && collisionTestHigh("left", 18)) {
				state = BASHING_LEFT_STATE;
				bashingSprite->changeAnimation(BASHING_LEFT);
				return true;
			}
			else if (state == WALKING_RIGHT_STATE && collisionTestHigh("right", 18)) {
				state = BASHING_RIGHT_STATE;
				bashingSprite->changeAnimation(BASHING_RIGHT);
				return true;
			}
		}
	}
	else if (power == 3) {
		if (state != FALLING_LEFT_STATE && state != FALLING_RIGHT_STATE) {
			if (state == WALKING_RIGHT_STATE) {
				if (collisionTestHigh("right", 18)) {
					state = WALKING_RIGHT_TO_CLIMB_STATE;
					return true;
				}
			}
			else if (state == WALKING_LEFT_STATE) {
				if (collisionTestHigh("left", 18)) {
					state = WALKING_LEFT_TO_CLIMB_STATE;
					return true;
				}
			}
		}
	}
	else if (power == 4) {
		if (state != FALLING_LEFT_STATE && state != FALLING_RIGHT_STATE) {
			woodCount = 0;
			if (state == WALKING_RIGHT_STATE) {
				state = BUILDING_RIGHT_STATE;
				buildingSprite->changeAnimation(BUILDING_RIGHT);
				Sprite* newWood = Sprite::createSprite(glm::ivec2(4, 1), glm::vec2(1, 1), &woodTexture, &shader);
				newWood->setNumberAnimations(1);
				newWood->setAnimationSpeed(WOOD, 24);
				newWood->addKeyframe(WOOD, glm::vec2(0.0f, 0.0f));
				newWood->changeAnimation(WOOD);
				glm::vec2 woodPos = sprite->position() + glm::vec2(7, 14);
				newWood->setPosition(woodPos);
				woods.push_back(newWood);
				buildWood(woodPos);
				Sprite* newerWood = Sprite::createSprite(glm::ivec2(4, 1), glm::vec2(1, 1), &woodTexture, &shader);
				newerWood->setNumberAnimations(1);
				newerWood->setAnimationSpeed(WOOD, 24);
				newerWood->addKeyframe(WOOD, glm::vec2(0.0f, 0.0f));
				newerWood->changeAnimation(WOOD);
				woodPos = sprite->position() + glm::vec2(5, 15);
				newerWood->setPosition(woodPos);
				woods.push_back(newerWood);
				buildWood(woodPos);
				return true;
			}
			else if (state == WALKING_LEFT_STATE) {
				state = BUILDING_LEFT_STATE;
				buildingSprite->changeAnimation(BUILDING_LEFT);
				Sprite* newWood = Sprite::createSprite(glm::ivec2(4, 1), glm::vec2(1, 1), &woodTexture, &shader);
				newWood->setNumberAnimations(1);
				newWood->setAnimationSpeed(WOOD, 24);
				newWood->addKeyframe(WOOD, glm::vec2(0.0f, 0.0f));
				newWood->changeAnimation(WOOD);
				glm::vec2 woodPos = sprite->position() + glm::vec2(0, 14);
				newWood->setPosition(woodPos);
				woods.push_back(newWood);
				buildWood(woodPos);
				Sprite* newerWood = Sprite::createSprite(glm::ivec2(4, 1), glm::vec2(1, 1), &woodTexture, &shader);
				newerWood->setNumberAnimations(1);
				newerWood->setAnimationSpeed(WOOD, 24);
				newerWood->addKeyframe(WOOD, glm::vec2(0.0f, 0.0f));
				newerWood->changeAnimation(WOOD);
				woodPos = sprite->position() + glm::vec2(2, 15);
				newerWood->setPosition(woodPos);
				woods.push_back(newerWood);
				buildWood(woodPos);
				return true;
			}
		}
	}
	else if (power == 5) {
		if (state != FALLING_LEFT_STATE && state != FALLING_RIGHT_STATE) {
			if (state == WALKING_RIGHT) {
				state = PLACING_PORTAL_RIGHT_STATE;
				portalPlacingSprite->changeAnimation(PLACING_PORTAL_RIGHT);
			}
			else if (state == WALKING_LEFT) {
				state = PLACING_PORTAL_LEFT_STATE;
				portalPlacingSprite->changeAnimation(PLACING_PORTAL_LEFT);
			}
			return true;
		}
	}
	else if (power == 6) {
		//state = EXPLODING;
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
	else if (state == CLIMBING_RIGHT_STATE || state == CLIMBING_LEFT_STATE) {
		if (state == CLIMBING_RIGHT_STATE) {
			climbingSprite->position().x += 5;
			climbingSprite->render();
			climbingSprite->position().x -= 5;
		}
		else {
			climbingSprite->position().x += -5;
			climbingSprite->render();
			climbingSprite->position().x -= -5;
		}
	}
	else if (state == BUILDING_RIGHT_STATE || state == BUILDING_LEFT_STATE) {
		buildingSprite->render();
	}
	else if (state == PLACING_PORTAL_RIGHT_STATE || state == PLACING_PORTAL_LEFT_STATE) {
		portalPlacingSprite->render();
	}
	else {
		sprite->render();
	}
	interactiveQuad->render();
	for (int i = 0; i < woods.size(); ++i) {
		woods[i]->render();
	}
	for (int i = 0; i < portals.size(); ++i) {
		portals[i]->render();
	}
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

bool Lemming::collisionTestHigh(string way, int dist)
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	posBase += glm::ivec2(7, 14); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
	posBase = sprite->position() + glm::vec2(120, 0);
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

bool Lemming::collisionHandLevel(string way) {
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	if (way == "right") posBase += glm::ivec2(17, 8); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	else if (way == "left") posBase += glm::ivec2(-1, 8);
	if (mask->pixel(posBase.x, posBase.y) == 255) return true;
	return false;
}

bool Lemming::collisionHeadLevel(string way) {
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	if (way == "right") posBase += glm::ivec2(14, -1); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	else if (way == "left") posBase += glm::ivec2(0, -1);
	if (mask->pixel(posBase.x, posBase.y) == 255) return true;
	return false;
}

bool Lemming::canClimb(string way) {
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	if (way == "right") {
		posBase += glm::ivec2(17, 8);
		for (int i = posBase.x; i < posBase.x + 6; ++i) {
			for (int j = posBase.y; j < posBase.y + 16; ++j) {
				if (mask->pixel(posBase.x, posBase.y) == 255) return false;
			}
		}
	}
	if (way == "left") {
		posBase += glm::ivec2(-1, 8);
		for (int i = posBase.x; i > posBase.x - 1; --i) {
			for (int j = posBase.y; j < posBase.y + 16; ++j) {
				if (mask->pixel(posBase.x, posBase.y) == 255) return false;
			}
		}
	}
	return true;
}

void Lemming::buildWood(glm::vec2 pos) {
	for (int i = pos.x + 120; i < pos.x + 4 + 120; ++i) {
		mask->setPixel(i, pos.y, 255);
	}
}

void Lemming::placePortal(string way) {
	Sprite* portalSprite = Sprite::createSprite(glm::ivec2(4, 16), glm::vec2(0.25, 1), &bluePortalSpritesheet, &shader);
	portalSprite->setNumberAnimations(16);
	portalSprite->setAnimationSpeed(BLUE_PORTAL, 4);
	for (int i = 0; i<4; i++)
		exitSprite->addKeyframe(BLUE_PORTAL, glm::vec2(float(i) / 4., 0.0f));
	portalSprite->changeAnimation(BLUE_PORTAL);
	if (way == "right") {
		portalSprite->position() = glm::vec2(sprite->position().x + 16, sprite->position().y);
	}
	else if (way == "left") {
		portalSprite->position() = glm::vec2(sprite->position().x - 4, sprite->position().y);
	}
}

void Lemming::setScenePortalPos(glm::vec2 portalPos) {
	scenePortalPos = portalPos;
}

void Lemming::setExit(glm::vec2 exitPos) {
	exitDoorPosition = exitPos;
}