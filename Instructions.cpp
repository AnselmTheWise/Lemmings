#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Instructions.h"


Instructions::Instructions()
{
}

Instructions::~Instructions()
{
}


void Instructions::init()
{
	initShaders();

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	backgroundQuad = TexturedQuad::createTexturedQuad(geom, texCoords, simpleTexProgram);

	backgroundTexture.loadFromFile("images/Instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backgroundTexture.setMagFilter(GL_NEAREST);
	backgroundTexture.setMinFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	backButtonTexture.loadFromFile("images/TestPlayTexture.jpg", TEXTURE_PIXEL_FORMAT_RGB);

	backButton = InteractiveQuad::createInteractiveQuad(glm::vec2(750.f*CAMERA_WIDTH / 960.f, 400.f*CAMERA_HEIGHT / 480.f), glm::vec2(CAMERA_WIDTH / 3.f, CAMERA_HEIGHT / 10.f), glm::vec2(0.5f, 1.f), &backButtonTexture, &simpleTexProgram);
	backButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	backButton->setOffsetHover(glm::vec2(0.5f, 0.f));

}

void Instructions::render()
{
	glm::mat4 modelview;
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	simpleTexProgram.setUniform2f("texCoordDispl", 0, 0);
	backgroundQuad->render(backgroundTexture);
	backButton->render();
}

void Instructions::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	backButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
}

int Instructions::getStatus() {
	if (backButton->isClicked())
		return 1;

	return 0;
}

void Instructions::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if (!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



