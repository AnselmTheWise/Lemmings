#ifndef _MAINMENU_INCLUDE
#define _MAINMENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "InteractiveQuad.h"
#include "PlayMenu.h"
#include "TextScreen.h"


class MainMenu
{

public:
	MainMenu();
	~MainMenu();

	void init();
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	int getStatus();

private:
	void initShaders();

private:
	TexturedQuad* backgroundQuad;
	Texture backgroundTexture;
	Texture playButtonTexture, instructionsButtonTexture1, creditButtonTexture, exitButtonTexture;
	Texture instructionsButtonTexture;
	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
	InteractiveQuad* playButton;
	InteractiveQuad* instructionsButton;
	InteractiveQuad* creditsButton;
	InteractiveQuad* exitButton;
	PlayMenu playMenu;
	TextScreen instructions, credits;

	enum RENDERING_ELEMENT {MAIN_MENU, PLAY_MENU, INSTRUCTIONS, CREDITS};

	RENDERING_ELEMENT renderingElement;

	void selfRender();

};


#endif

