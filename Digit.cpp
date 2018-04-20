#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Digit.h"


Digit::Digit()
{
}

Digit::~Digit()
{
}


void Digit::init(glm::vec2 initialPos, glm::ivec2 quadSize)
{
	initShaders();

	float coordToPixel = 480.f / float(CAMERA_HEIGHT);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	digitSheet.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	digitSheet.setMinFilter(GL_NEAREST);
	digitSheet.setMagFilter(GL_NEAREST);

	digitSprite = Sprite::createSprite(quadSize, glm::vec2(14.f/140.f, 1.0), &digitSheet, &simpleTexProgram);
	digitSprite->setNumberAnimations(16);

	digitSprite->setAnimationSpeed(ZERO, 1);
	digitSprite->addKeyframe(ZERO, glm::vec2((0.f*14.f) / 140.f, 0.0f));
	digitSprite->setAnimationSpeed(ONE, 1);
	digitSprite->addKeyframe(ONE, glm::vec2((1.f*14.f) / 140.f, 0.0f));
	digitSprite->setAnimationSpeed(TWO, 1);
	digitSprite->addKeyframe(TWO, glm::vec2((2.f*14.f) / 140.f, 0.0f));
	digitSprite->setAnimationSpeed(THREE, 1);
	digitSprite->addKeyframe(THREE, glm::vec2((3.f*14.f) / 140.f, 0.0f));
	digitSprite->setAnimationSpeed(FOUR, 1);
	digitSprite->addKeyframe(FOUR, glm::vec2((4.f*14.f) / 140.f, 0.0f));
	digitSprite->setAnimationSpeed(FIVE, 1);
	digitSprite->addKeyframe(FIVE, glm::vec2((5.f*14.f) / 140.f, 0.0f));
	digitSprite->setAnimationSpeed(SIX, 1);
	digitSprite->addKeyframe(SIX, glm::vec2((6.f*14.f) / 140.f, 0.0f));
	digitSprite->setAnimationSpeed(SEVEN, 1);
	digitSprite->addKeyframe(SEVEN, glm::vec2((7.f*14.f) / 140.f, 0.0f));
	digitSprite->setAnimationSpeed(EIGHT, 1);
	digitSprite->addKeyframe(EIGHT, glm::vec2((8.f*14.f) / 140.f, 0.0f));
	digitSprite->setAnimationSpeed(NINE, 1);
	digitSprite->addKeyframe(NINE, glm::vec2((9.f*14.f) / 140.f, 0.0f));

	digitSprite->changeAnimation(ZERO);
	digitSprite->setPosition(initialPos);
}

void Digit::setPosition(glm::vec2 position) {
	digitSprite->setPosition(position); 
}

void Digit::changeDigit(int desiredNum) {
	if (desiredNum > -1 && desiredNum < 10) {
		digitSprite->changeAnimation(numbers[desiredNum]); 
	}
	else cout << "Invalid num" << endl; 
}


void Digit::render()
{
	glm::mat4 modelview;

	modelview = glm::mat4(1.0f);
	simpleTexProgram.use();
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform2f("texCoordDispl", 0, 0);
	
	digitSprite->render();
}

void Digit::initShaders()
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



