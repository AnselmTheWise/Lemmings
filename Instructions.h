#ifndef _INSTRUCTIONS_INCLUDE
#define _INSTRUCTIONS_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Texture.h"
#include "InteractiveQuad.h"


class Instructions
{

public:
	Instructions();
	~Instructions();

	void init();
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
