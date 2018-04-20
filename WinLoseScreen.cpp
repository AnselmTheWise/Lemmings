#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "WinLoseScreen.h"


WinLoseScreen::WinLoseScreen()
{
}

WinLoseScreen::~WinLoseScreen()
{
}


void WinLoseScreen::init(string kind)
{

	this->kind = kind;

	initShaders();

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	backgroundQuad = TexturedQuad::createTexturedQuad(geom, texCoords, simpleTexProgram);

	if (kind == "Win") {
		backgroundTexture.loadFromFile("images/Win.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}
	else if (kind == "Lose") {
		backgroundTexture.loadFromFile("images/Lose.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}
	backgroundTexture.setMagFilter(GL_NEAREST);
	backgroundTexture.setMinFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	mainMenuButtonTexture.loadFromFile("images/backButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	if (kind == "Win") nextReplayButtonTexture.loadFromFile("images/nextLevelButton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else nextReplayButtonTexture.loadFromFile("images/retryButton.png", TEXTURE_PIXEL_FORMAT_RGBA); 

	mainMenuButton = InteractiveQuad::createInteractiveQuad(glm::vec2(400.f*CAMERA_WIDTH / 960.f, 400.f*CAMERA_HEIGHT / 480.f), glm::vec2((CAMERA_WIDTH / 3.f)-40.f, CAMERA_HEIGHT / 10.f), glm::vec2(1.f/3.f, 1.f), &mainMenuButtonTexture, &simpleTexProgram);
	mainMenuButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	mainMenuButton->setOffsetHover(glm::vec2(1.f/3.f, 0.f));
	mainMenuButton->setOffsetClick(glm::vec2(2.f/3.f, 0.f));
	nextReplayButton = InteractiveQuad::createInteractiveQuad(glm::vec2(650.f*CAMERA_WIDTH / 960.f, 400.f*CAMERA_HEIGHT / 480.f), glm::vec2((CAMERA_WIDTH / 3.f)-40.f, CAMERA_HEIGHT / 10.f), glm::vec2(1.f/3.f, 1.f), &nextReplayButtonTexture, &simpleTexProgram);
	nextReplayButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	nextReplayButton->setOffsetHover(glm::vec2(1.f/3.f, 0.f));
	nextReplayButton->setOffsetClick(glm::vec2(2.f/3.f, 0.f)); 

}

void WinLoseScreen::render()
{
	glm::mat4 modelview;
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	simpleTexProgram.setUniform2f("texCoordDispl", 0, 0);
	backgroundQuad->render(backgroundTexture);
	mainMenuButton->render();
	nextReplayButton->render();
}

void WinLoseScreen::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	mainMenuButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	nextReplayButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
}

int WinLoseScreen::getStatus() {
	if (mainMenuButton->isClicked()) {
		return 4;
	}
	if (nextReplayButton->isClicked()) {
		if (kind == "Win") {
			return 2;
		}
		else if (kind == "Lose") {
			return 1;
		}
	}

	return 0;
}

void WinLoseScreen::initShaders()
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



