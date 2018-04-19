#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "MainMenu.h"


MainMenu::MainMenu()
{
	playButton = NULL;
}

MainMenu::~MainMenu()
{
}


void MainMenu::init()
{
	initShaders();

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	backgroundQuad = TexturedQuad::createTexturedQuad(geom, texCoords, simpleTexProgram);

	backgroundTexture.loadFromFile("images/MainMenuBackground.jpg", TEXTURE_PIXEL_FORMAT_RGB);
	backgroundTexture.setMagFilter(GL_NEAREST);
	backgroundTexture.setMinFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	playButtonTexture.loadFromFile("images/TestPlayTexture.jpg", TEXTURE_PIXEL_FORMAT_RGB);

	playButton = InteractiveQuad::createInteractiveQuad(glm::vec2(CAMERA_HEIGHT / 3.f, 5.f * CAMERA_HEIGHT / 10), glm::vec2(CAMERA_WIDTH / 3.f, CAMERA_HEIGHT / 10.f), glm::vec2(0.5f, 1.f), &playButtonTexture, &simpleTexProgram);
	playButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	playButton->setOffsetHover(glm::vec2(0.5f, 0.f));
	instructionsButton = InteractiveQuad::createInteractiveQuad(glm::vec2(CAMERA_HEIGHT / 3.f, 5.f * CAMERA_HEIGHT / 10 + 20), glm::vec2(CAMERA_WIDTH / 3.f, CAMERA_HEIGHT / 10.f), glm::vec2(0.5f, 1.f), &playButtonTexture, &simpleTexProgram);
	instructionsButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	instructionsButton->setOffsetHover(glm::vec2(0.5f, 0.f));
	creditsButton = InteractiveQuad::createInteractiveQuad(glm::vec2(CAMERA_HEIGHT / 3.f, 5.f * CAMERA_HEIGHT / 10 + 40), glm::vec2(CAMERA_WIDTH / 3.f, CAMERA_HEIGHT / 10.f), glm::vec2(0.5f, 1.f), &playButtonTexture, &simpleTexProgram);
	creditsButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	creditsButton->setOffsetHover(glm::vec2(0.5f, 0.f));

	renderingElement = MAIN_MENU;
	playMenu.init();
	instructions.init("Instructions");
	credits.init("Credits");

}

void MainMenu::render()
{
	if (renderingElement == MAIN_MENU) {
		selfRender();
	}
	else if (renderingElement == PLAY_MENU) {
		playMenu.render();
	}
	else if (renderingElement == INSTRUCTIONS) {
		instructions.render();
	}
	else if (renderingElement == CREDITS) {
		credits.render();
	}
}

void MainMenu::selfRender() {
	glm::mat4 modelview;
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	simpleTexProgram.setUniform2f("texCoordDispl", 0, 0);
	backgroundQuad->render(backgroundTexture);
	playButton->render();
	instructionsButton->render();
	creditsButton->render();
}

void MainMenu::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if (renderingElement == MAIN_MENU) {
		playButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
		instructionsButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
		creditsButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	}
	else if (renderingElement == PLAY_MENU) {
		playMenu.mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	}
	else if (renderingElement == INSTRUCTIONS) {
		instructions.mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	}
	else if (renderingElement == CREDITS) {
		credits.mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	}
}

int MainMenu::getStatus() {
	if (renderingElement == MAIN_MENU) {
		if (playButton->isClicked()) {
			renderingElement = PLAY_MENU;
		}
		else if (instructionsButton->isClicked()) {
			renderingElement = INSTRUCTIONS;
		}
		else if (creditsButton->isClicked()) {
			renderingElement = CREDITS;
		}
		return 0;
	}
	else if (renderingElement == PLAY_MENU) {
		int status = playMenu.getStatus();
		if (status == 4) {
			renderingElement = MAIN_MENU;
			return 0;
		}
		return status;
	}
	else if (renderingElement == INSTRUCTIONS) {
		int status = instructions.getStatus();
		if (status == 1) {
			renderingElement = MAIN_MENU;
		}
		return 0;
	}
	else if (renderingElement == CREDITS) {
		int status = credits.getStatus();
		if (status == 1) {
			renderingElement = MAIN_MENU;
		}
		return 0;
	}
}

void MainMenu::initShaders()
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



