#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "InteractiveQuad.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setMapMask(VariableTexture *mapMask);

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	int getStatus();

	bool setPower(int power); //0 -> Stopper //1 -> Digger

	bool isClicked();
	
private:
	int collisionFloor(int maxFall);
	bool collision();
	bool collisionTest(string way, int dist);
	void addBlocking();
	void digMask();
	void bashMask(string way);
	bool canClimb(string way);
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, STOPPING_STATE, DIGGING_STATE, 
		BASHING_LEFT_STATE, BASHING_RIGHT_STATE, CLIMBING_LEFT_STATE, CLIMBING_RIGHT_STATE, WALKING_LEFT_TO_CLIMB_STATE,
		WALKING_RIGHT_TO_CLIMB_STATE, EXITING_STATE
	};

	LemmingState state;
	Texture spritesheet, spritesheetIQ, exitSpritesheet, fallingSpritesheet, stoppingSpritesheet, diggingSpritesheet, bashingSpritesheet,
		climbingSpritesheet;
	Sprite *sprite;
	Sprite *exitSprite;
	Sprite *fallingSprite;
	Sprite *stoppingSprite;
	Sprite *diggingSprite;
	Sprite *bashingSprite;
	Sprite *climbingSprite;
	InteractiveQuad *interactiveQuad;
	VariableTexture *mask;

	int status; //0 -> alive //1 -> exited //2 -> dead
	int timeExiting;
	int fallingConsec;

	glm::vec2 exitDoorPosition;

	int yOffset; //Offset so the InteractiveQuad covers all the Lemming in the center

};


#endif // _LEMMING_INCLUDE


