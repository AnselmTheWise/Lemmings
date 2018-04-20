#ifndef _WINLOSESCREEN_INCLUDE
#define _WINLOSESCREEN_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "InteractiveQuad.h"


class WinLoseScreen
{

public:
	WinLoseScreen();
	~WinLoseScreen();

	void init(string kind);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	int getStatus();

private:
	void initShaders();

private:
	TexturedQuad* backgroundQuad;
	Texture backgroundTexture;
	Texture mainMenuButtonTexture, nextReplayButtonTexture;
	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
	InteractiveQuad* mainMenuButton;
	InteractiveQuad* nextReplayButton;
	string kind;

};


#endif
