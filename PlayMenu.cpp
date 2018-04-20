#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "PlayMenu.h"


PlayMenu::PlayMenu()
{
}

PlayMenu::~PlayMenu()
{
}


void PlayMenu::init()
{
	initShaders();

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	backgroundQuad = TexturedQuad::createTexturedQuad(geom, texCoords, simpleTexProgram);

	backgroundTexture.loadFromFile("images/MainMenuBackground.jpg", TEXTURE_PIXEL_FORMAT_RGB);
	backgroundTexture.setMagFilter(GL_NEAREST);
	backgroundTexture.setMinFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	level1ButtonTexture.loadFromFile("images/level1Button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	level2ButtonTexture.loadFromFile("images/level2Button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	level3ButtonTexture.loadFromFile("images/level3Button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	backButtonTexture.loadFromFile("images/backButton.png", TEXTURE_PIXEL_FORMAT_RGBA);

	level1Button = InteractiveQuad::createInteractiveQuad(glm::vec2(CAMERA_HEIGHT / 3.f, 5.f * CAMERA_HEIGHT / 10), glm::vec2(CAMERA_WIDTH / 5.f, CAMERA_HEIGHT / 10.f), glm::vec2(1.f/3.f, 1.f), &level1ButtonTexture, &simpleTexProgram);
	level1Button->setOffsetIdle(glm::vec2(0.f, 0.f));
	level1Button->setOffsetHover(glm::vec2(1.f/3.f, 0.f));
	level1Button->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));
	level2Button = InteractiveQuad::createInteractiveQuad(glm::vec2(CAMERA_HEIGHT / 3.f, 5.f * CAMERA_HEIGHT / 10 + 20), glm::vec2(CAMERA_WIDTH / 5.f, CAMERA_HEIGHT / 10.f), glm::vec2(1.f/3.f, 1.f), &level2ButtonTexture, &simpleTexProgram);
	level2Button->setOffsetIdle(glm::vec2(0.f, 0.f));
	level2Button->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	level2Button->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));
	level3Button = InteractiveQuad::createInteractiveQuad(glm::vec2(CAMERA_HEIGHT / 3.f, 5.f * CAMERA_HEIGHT / 10 + 40), glm::vec2(CAMERA_WIDTH / 5.f, CAMERA_HEIGHT / 10.f), glm::vec2(1.f/3.f, 1.f), &level3ButtonTexture, &simpleTexProgram);
	level3Button->setOffsetIdle(glm::vec2(0.f, 0.f));
	level3Button->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	level3Button->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));
	backButton = InteractiveQuad::createInteractiveQuad(glm::vec2(CAMERA_HEIGHT / 3.f, 5.f * CAMERA_HEIGHT / 10 + 60), glm::vec2(CAMERA_WIDTH / 5.f, CAMERA_HEIGHT / 10.f), glm::vec2(1.f/3.f, 1.f), &backButtonTexture, &simpleTexProgram);
	backButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	backButton->setOffsetHover(glm::vec2(1.f/3.f, 0.f));
	backButton->setOffsetClick(glm::vec2(2.f/3.f, 0.f)); 

}

void PlayMenu::render()
{
	glm::mat4 modelview;
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	simpleTexProgram.setUniform2f("texCoordDispl", 0, 0);
	backgroundQuad->render(backgroundTexture);
	level1Button->render();
	level2Button->render();
	level3Button->render();
	backButton->render();
}

void PlayMenu::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	level1Button->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	level2Button->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	level3Button->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	backButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
}

int PlayMenu::getStatus() {
	if (level1Button->isClicked())
		return 1;
	else if (level2Button->isClicked())
		return 2;
	else if (level3Button->isClicked())
		return 3;
	else if (backButton->isClicked())
		return 4;

	return 0;
}

void PlayMenu::initShaders()
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



