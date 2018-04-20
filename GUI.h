#ifndef _GUI_INCLUDE
#define _GUI_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "TexturedQuad.h"
#include "InteractiveQuad.h"
#include "Sprite.h"



class GUI 
{

public:
	GUI();
	~GUI();

	void init();
	void render();
	void update(int deltaTime); 

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	int getSpeedState();

	int getButtonClicked();

private:
	void initShaders();

private:
	enum ANIM_ID {
		STOPPER, DIGGER, BASHER, CLIMBER, BUILDER, EXPLOSION, PLAY, PAUSE, FAST
	};

	enum TEXT {
		NONE, STOPPERT, DIGGERT, BASHERT, CLIMBERT, BUILDERT, SURRENDERT, PLAYPAUSET, FASTT 
	};

	Texture helpSheet; 
	Sprite *helpSprite; 

	Texture stopperSheet, diggerSheet, basherSheet, climberSheet, builderSheet, explosionSheet, playPauseSheet, fastSheet;
	Sprite *stopperSprite, *diggerSprite, *basherSprite, *climberSprite, *builderSprite, *explosionSprite, *playPauseSprite, *fastSprite;

	TexturedQuad* barQuad;
	Texture interfaceBar;
	Texture stopperButtonTexture, diggerButtonTexture, basherButtonTexture, climberButtonTexture, builderButtonTexture, surrenderButtonTexture, playPauseButtonTexture, fastButtonTexture;
	InteractiveQuad *stopperButton, *diggerButton, *basherButton, *climberButton, *builderButton, *surrenderButton, *playPauseButton, *fastButton; 
	ShaderProgram simpleTexProgram;
	glm::mat4 projection;

	bool paused, fastForward, notext; 
};


#endif // _SCENE_INCLUDE

