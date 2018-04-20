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
	bool collisionTestHigh(string way, int dist);
	void addBlocking();
	void digMask();
	void bashMask(string way);
	bool canClimb(string way);
	bool collisionHandLevel(string way);
	bool collisionHeadLevel(string way);
	void buildWood(glm::vec2);
	void placePortal(string way);
	void setScenePortalPos(glm::vec2 portalPos);
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, STOPPING_STATE, DIGGING_STATE, 
		BASHING_LEFT_STATE, BASHING_RIGHT_STATE, CLIMBING_LEFT_STATE, CLIMBING_RIGHT_STATE, WALKING_LEFT_TO_CLIMB_STATE,
		WALKING_RIGHT_TO_CLIMB_STATE, BUILDING_LEFT_STATE, BUILDING_RIGHT_STATE, EXITING_STATE, PLACING_PORTAL_LEFT_STATE,
		PLACING_PORTAL_RIGHT_STATE, WOOD
	};

	LemmingState state;
	Texture spritesheet, spritesheetIQ, exitSpritesheet, fallingSpritesheet, stoppingSpritesheet, diggingSpritesheet, bashingSpritesheet,
		climbingSpritesheet, buildingSpritesheet, woodTexture, orangePortalSpritesheet, bluePortalSpritesheet;
	Sprite *sprite;
	Sprite *exitSprite;
	Sprite *fallingSprite;
	Sprite *stoppingSprite;
	Sprite *diggingSprite;
	Sprite *bashingSprite;
	Sprite *climbingSprite;
	Sprite *buildingSprite;
	Sprite *portalPlacingSprite;
	InteractiveQuad *interactiveQuad;
	VariableTexture *mask;

	ShaderProgram shader;

	int status; //0 -> alive //1 -> exited //2 -> dead
	int timeExiting;
	int fallingConsec;

	glm::vec2 exitDoorPosition;
	vector<Sprite*> woods;
	vector<Sprite*> portals;
	bool upped;
	int woodCount;
	glm::vec2 scenePortalPos;
	glm::vec2 portalPos;

	int yOffset; //Offset so the InteractiveQuad covers all the Lemming in the center

};


#endif // _LEMMING_INCLUDE


