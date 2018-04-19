#ifndef _GUI_INCLUDE
#define _GUI_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "TexturedQuad.h"
#include "InteractiveQuad.h"



class GUI 
{

public:
	GUI();
	~GUI();

	void init();
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();

private:
	TexturedQuad* barQuad;
	Texture interfaceBar;
	Texture stopperButtonTexture, diggerButtonTexture, basherButtonTexture, climberButtonTexture, builderButtonTexture;
	InteractiveQuad* stopperButton, *diggerButton, *basherButton, *climberButton, *builderButton; 
	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
};


#endif // _SCENE_INCLUDE

