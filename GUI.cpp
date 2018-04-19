#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "GUI.h"


GUI::GUI()
{
}

GUI::~GUI()
{
}


void GUI::init()
{
	initShaders();

	float coordToPixel = 480.f / float(CAMERA_HEIGHT);

	glm::vec2 geom[2] = { glm::vec2(0.f, (-70.f / coordToPixel) + float(CAMERA_HEIGHT)) , glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	barQuad = TexturedQuad::createTexturedQuad(geom, texCoords, simpleTexProgram);

	interfaceBar.loadFromFile("images/BarTest.png", TEXTURE_PIXEL_FORMAT_RGBA);
	interfaceBar.setMinFilter(GL_NEAREST);
	interfaceBar.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	stopperButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	stopperButton = InteractiveQuad::createInteractiveQuad(glm::vec2(8.f/coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f/coordToPixel, 42.f/coordToPixel), glm::vec2(1.f/3.f, 1.f), &stopperButtonTexture, &simpleTexProgram);
	stopperButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	stopperButton->setOffsetHover(glm::vec2(1.f/3.f, 0.f));
	stopperButton->setOffsetClick(glm::vec2(2.f/3.f, 0.f));

	diggerButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	diggerButton = InteractiveQuad::createInteractiveQuad(glm::vec2(58.f / coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f / coordToPixel, 42.f / coordToPixel), glm::vec2(1.f / 3.f, 1.f), &diggerButtonTexture, &simpleTexProgram);
	diggerButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	diggerButton->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	diggerButton->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));

	basherButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	basherButton = InteractiveQuad::createInteractiveQuad(glm::vec2(108.f / coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f / coordToPixel, 42.f / coordToPixel), glm::vec2(1.f / 3.f, 1.f), &basherButtonTexture, &simpleTexProgram);
	basherButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	basherButton->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	basherButton->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));

	climberButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	climberButton = InteractiveQuad::createInteractiveQuad(glm::vec2(158.f / coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f / coordToPixel, 42.f / coordToPixel), glm::vec2(1.f / 3.f, 1.f), &climberButtonTexture, &simpleTexProgram);
	climberButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	climberButton->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	climberButton->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));

	builderButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	builderButton = InteractiveQuad::createInteractiveQuad(glm::vec2(208.f / coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f / coordToPixel, 42.f / coordToPixel), glm::vec2(1.f / 3.f, 1.f), &builderButtonTexture, &simpleTexProgram);
	builderButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	builderButton->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	builderButton->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));
}


void GUI::render()
{
	glm::mat4 modelview;

	modelview = glm::mat4(1.0f);
	simpleTexProgram.use();
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform2f("texCoordDispl", 0, 0);
	barQuad->render(interfaceBar);
	stopperButton->render();
	diggerButton->render(); 
	basherButton->render();
	climberButton->render();
	builderButton->render();

}

void GUI::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	stopperButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	diggerButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	basherButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	climberButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	builderButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
}

void GUI::initShaders()
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



