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
	
private:
	int collisionFloor(int maxFall);
	bool collision();
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE
	};

	LemmingState state;
	Texture spritesheet, spritesheetIQ;
	Sprite *sprite;
	InteractiveQuad *interactiveQuad;
	VariableTexture *mask;

	int yOffset; //Offset so the InteractiveQuad covers all the Lemming in the center

};


#endif // _LEMMING_INCLUDE


