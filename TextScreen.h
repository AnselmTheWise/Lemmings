#ifndef _TEXTSCREEN_INCLUDE
#define _TEXTSCREEN_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "InteractiveQuad.h"


class TextScreen
{

public:
	TextScreen();
	~TextScreen();

	void init(string kind);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	int getStatus();

private:
	void initShaders();

private:
	TexturedQuad* backgroundQuad;
	Texture backgroundTexture;
	Texture backButtonTexture;
	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
	InteractiveQuad* backButton;

};


#endif
