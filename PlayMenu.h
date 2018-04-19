#ifndef _PLAYMENU_INCLUDE
#define _PLAYMENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "InteractiveQuad.h"


class PlayMenu
{

public:
	PlayMenu();
	~PlayMenu();

	void init();
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	int getStatus();

private:
	void initShaders();

private:
	TexturedQuad* backgroundQuad;
	Texture backgroundTexture;
	Texture level1ButtonTexture;
	Texture level2ButtonTexture;
	Texture level3ButtonTexture;
	Texture backButtonTexture;
	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
	InteractiveQuad* level1Button;
	InteractiveQuad* level2Button;
	InteractiveQuad* level3Button;
	InteractiveQuad* backButton;

};


#endif

