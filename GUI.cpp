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

	paused = false; 
	fastForward = false; 
	notext = true; 

	float coordToPixel = 480.f / float(CAMERA_HEIGHT);

	glm::vec2 geom[2] = { glm::vec2(0.f, (-90.f / coordToPixel) + float(CAMERA_HEIGHT)) , glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	barQuad = TexturedQuad::createTexturedQuad(geom, texCoords, simpleTexProgram);

	interfaceBar.loadFromFile("images/barTest3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	interfaceBar.setMinFilter(GL_NEAREST);
	interfaceBar.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	playPauseButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	playPauseButton = InteractiveQuad::createInteractiveQuad(glm::vec2(685.f / coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f / coordToPixel, 42.f / coordToPixel), glm::vec2(1.f / 3.f, 1.f), &playPauseButtonTexture, &simpleTexProgram);
	playPauseButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	playPauseButton->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	playPauseButton->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));

	playPauseSheet.loadFromFile("images/playPause.png", TEXTURE_PIXEL_FORMAT_RGBA);
	playPauseSheet.setMinFilter(GL_NEAREST);
	playPauseSheet.setMagFilter(GL_NEAREST);

	playPauseSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.5, 1.0), &playPauseSheet, &simpleTexProgram);
	playPauseSprite->setNumberAnimations(10);

	playPauseSprite->setAnimationSpeed(PAUSE, 1);
	playPauseSprite->addKeyframe(PAUSE, glm::vec2(0.5f, 0.0f));

	playPauseSprite->setAnimationSpeed(PLAY, 1);
	playPauseSprite->addKeyframe(PLAY, glm::vec2(0.0f, 0.0f));

	playPauseSprite->changeAnimation(PAUSE);
	playPauseSprite->setPosition(glm::vec2(683.f / coordToPixel, (-54.f / coordToPixel) + float(CAMERA_HEIGHT)));

	stopperButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	stopperButton = InteractiveQuad::createInteractiveQuad(glm::vec2(8.f/coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f/coordToPixel, 42.f/coordToPixel), glm::vec2(1.f/3.f, 1.f), &stopperButtonTexture, &simpleTexProgram);
	stopperButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	stopperButton->setOffsetHover(glm::vec2(1.f/3.f, 0.f));
	stopperButton->setOffsetClick(glm::vec2(2.f/3.f, 0.f));

	stopperSheet.loadFromFile("images/stopper.png", TEXTURE_PIXEL_FORMAT_RGBA);
	stopperSheet.setMinFilter(GL_NEAREST);
	stopperSheet.setMagFilter(GL_NEAREST);

	stopperSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 1.0), &stopperSheet, &simpleTexProgram);
	stopperSprite->setNumberAnimations(16);

	stopperSprite->setAnimationSpeed(STOPPER, 12);
	for (int i = 0; i<16; i++)
		stopperSprite->addKeyframe(STOPPER, glm::vec2(float(i) / 16, 0.0f));

	stopperSprite->changeAnimation(STOPPER);
	stopperSprite->setPosition(glm::vec2(6.f / coordToPixel, (-55.f / coordToPixel) + float(CAMERA_HEIGHT)));

	diggerButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	diggerButton = InteractiveQuad::createInteractiveQuad(glm::vec2(58.f / coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f / coordToPixel, 42.f / coordToPixel), glm::vec2(1.f / 3.f, 1.f), &diggerButtonTexture, &simpleTexProgram);
	diggerButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	diggerButton->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	diggerButton->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));

	diggerSheet.loadFromFile("images/digger.png", TEXTURE_PIXEL_FORMAT_RGBA);
	diggerSheet.setMinFilter(GL_NEAREST);
	diggerSheet.setMagFilter(GL_NEAREST);

	diggerSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 1.0), &diggerSheet, &simpleTexProgram);
	diggerSprite->setNumberAnimations(8);

	diggerSprite->setAnimationSpeed(DIGGER, 12);
	for (int i = 0; i<8; i++)
		diggerSprite->addKeyframe(DIGGER, glm::vec2(float(i) / 8, 0.0f));

	diggerSprite->changeAnimation(DIGGER);
	diggerSprite->setPosition(glm::vec2(56.f / coordToPixel, (-55.f / coordToPixel) + float(CAMERA_HEIGHT)));

	basherButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	basherButton = InteractiveQuad::createInteractiveQuad(glm::vec2(108.f / coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f / coordToPixel, 42.f / coordToPixel), glm::vec2(1.f / 3.f, 1.f), &basherButtonTexture, &simpleTexProgram);
	basherButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	basherButton->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	basherButton->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));

	basherSheet.loadFromFile("images/basher.png", TEXTURE_PIXEL_FORMAT_RGBA);
	basherSheet.setMinFilter(GL_NEAREST);
	basherSheet.setMagFilter(GL_NEAREST);

	basherSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.5), &basherSheet, &simpleTexProgram);
	basherSprite->setNumberAnimations(16);

	basherSprite->setAnimationSpeed(BASHER, 12);
	for (int i = 0; i<16; i++)
		basherSprite->addKeyframe(BASHER, glm::vec2(float(i) / 16, 0.0f));

	basherSprite->changeAnimation(BASHER);
	basherSprite->setPosition(glm::vec2(106.f / coordToPixel, (-55.f / coordToPixel) + float(CAMERA_HEIGHT)));

	climberButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	climberButton = InteractiveQuad::createInteractiveQuad(glm::vec2(158.f / coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f / coordToPixel, 42.f / coordToPixel), glm::vec2(1.f / 3.f, 1.f), &climberButtonTexture, &simpleTexProgram);
	climberButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	climberButton->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	climberButton->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));

	climberSheet.loadFromFile("images/climber2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	climberSheet.setMinFilter(GL_NEAREST);
	climberSheet.setMagFilter(GL_NEAREST);

	climberSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.5), &climberSheet, &simpleTexProgram);
	climberSprite->setNumberAnimations(16);

	climberSprite->setAnimationSpeed(CLIMBER, 12);
	for (int i = 0; i<16; i++)
		climberSprite->addKeyframe(CLIMBER, glm::vec2(float(i) / 16, 0.0f));

	climberSprite->changeAnimation(CLIMBER);
	climberSprite->setPosition(glm::vec2(156.f / coordToPixel, (-55.f / coordToPixel) + float(CAMERA_HEIGHT)));

	builderButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	builderButton = InteractiveQuad::createInteractiveQuad(glm::vec2(208.f / coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f / coordToPixel, 42.f / coordToPixel), glm::vec2(1.f / 3.f, 1.f), &builderButtonTexture, &simpleTexProgram);
	builderButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	builderButton->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	builderButton->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));

	builderSheet.loadFromFile("images/builder2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	builderSheet.setMinFilter(GL_NEAREST);
	builderSheet.setMagFilter(GL_NEAREST);

	builderSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.5), &builderSheet, &simpleTexProgram);
	builderSprite->setNumberAnimations(16);

	builderSprite->setAnimationSpeed(BUILDER, 12);
	for (int i = 0; i<16; i++)
		builderSprite->addKeyframe(BUILDER, glm::vec2(float(i) / 16, 0.0f));

	builderSprite->changeAnimation(BUILDER);
	builderSprite->setPosition(glm::vec2(206.f / coordToPixel, (-55.f / coordToPixel) + float(CAMERA_HEIGHT)));

	surrenderButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	surrenderButton = InteractiveQuad::createInteractiveQuad(glm::vec2(630.f / coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f / coordToPixel, 42.f / coordToPixel), glm::vec2(1.f / 3.f, 1.f), &surrenderButtonTexture, &simpleTexProgram);
	surrenderButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	surrenderButton->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	surrenderButton->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));

	explosionSheet.loadFromFile("images/explodingLemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	explosionSheet.setMinFilter(GL_NEAREST);
	explosionSheet.setMagFilter(GL_NEAREST);

	explosionSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 1.0), &explosionSheet, &simpleTexProgram);
	explosionSprite->setNumberAnimations(14);

	explosionSprite->setAnimationSpeed(EXPLOSION, 10);
	for (int i = 0; i<14; i++)
		explosionSprite->addKeyframe(EXPLOSION, glm::vec2(float(i) / 14, 0.0f));

	explosionSprite->changeAnimation(EXPLOSION);
	explosionSprite->setPosition(glm::vec2(624.f / coordToPixel, (-55.f / coordToPixel) + float(CAMERA_HEIGHT)));

	fastButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	fastButton = InteractiveQuad::createInteractiveQuad(glm::vec2(740.f / coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f / coordToPixel, 42.f / coordToPixel), glm::vec2(1.f / 3.f, 1.f), &fastButtonTexture, &simpleTexProgram);
	fastButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	fastButton->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	fastButton->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));

	fastSheet.loadFromFile("images/fastForward.png", TEXTURE_PIXEL_FORMAT_RGBA);
	fastSheet.setMinFilter(GL_NEAREST);
	fastSheet.setMagFilter(GL_NEAREST);

	fastSprite = Sprite::createSprite(glm::ivec2(13, 16), glm::vec2(0.25, 1.0), &fastSheet, &simpleTexProgram);
	fastSprite->setNumberAnimations(16);

	fastSprite->setAnimationSpeed(FAST, 8);
	for (int i = 0; i<4; i++)
		fastSprite->addKeyframe(FAST, glm::vec2(float(i) / 4, 0.0f));

	fastSprite->changeAnimation(FAST);
	fastSprite->setPosition(glm::vec2(740.f / coordToPixel, (-53.f / coordToPixel) + float(CAMERA_HEIGHT)));

	helpSheet.loadFromFile("images/helptext.png", TEXTURE_PIXEL_FORMAT_RGBA);
	helpSheet.setMinFilter(GL_NEAREST);
	helpSheet.setMagFilter(GL_NEAREST);

	helpSprite = Sprite::createSprite(glm::ivec2(40, 4), glm::vec2(1.0, 0.125), &helpSheet, &simpleTexProgram);
	helpSprite->setNumberAnimations(16); 

	helpSprite->setAnimationSpeed(STOPPERT, 1);
	helpSprite->addKeyframe(STOPPERT, glm::vec2(0.0f, 0.0f));
	helpSprite->setAnimationSpeed(DIGGERT, 1);
	helpSprite->addKeyframe(DIGGERT, glm::vec2(0.0f ,0.125f));
	helpSprite->setAnimationSpeed(BASHERT, 1);
	helpSprite->addKeyframe(BASHERT, glm::vec2(0.0f, 0.250f));
	helpSprite->setAnimationSpeed(CLIMBERT, 1);
	helpSprite->addKeyframe(CLIMBERT, glm::vec2(0.0f, 0.375f));
	helpSprite->setAnimationSpeed(BUILDERT, 1);
	helpSprite->addKeyframe(BUILDERT, glm::vec2(0.0f, 0.500f));
	helpSprite->setAnimationSpeed(SURRENDERT, 1);
	helpSprite->addKeyframe(SURRENDERT, glm::vec2(0.0f, 0.625f));
	helpSprite->setAnimationSpeed(PLAYPAUSET, 1);
	helpSprite->addKeyframe(PLAYPAUSET, glm::vec2(0.0f, 0.750f));
	helpSprite->setAnimationSpeed(FASTT, 1);
	helpSprite->addKeyframe(FASTT, glm::vec2(0.0f, 0.875f));
	
	helpSprite->changeAnimation(STOPPERT);
	helpSprite->setPosition(glm::vec2(8.f / coordToPixel, (-82.f / coordToPixel) + float(CAMERA_HEIGHT)));

}

void GUI::update(int deltaTime) {
	if (stopperButton->isHovering()) {
		stopperSprite->update(deltaTime);
		notext = false; 
		helpSprite->changeAnimation(STOPPERT); 
	}
	else if (diggerButton->isHovering()) {
		diggerSprite->update(deltaTime); 
		notext = false; 
		helpSprite->changeAnimation(DIGGERT);
	}
	else if (basherButton->isHovering()) {
		basherSprite->update(deltaTime);
		notext = false;
		helpSprite->changeAnimation(BASHERT);
	}
	else if (climberButton->isHovering()) {
		climberSprite->update(deltaTime);
		notext = false;
		helpSprite->changeAnimation(CLIMBERT);
	}
	else if (builderButton->isHovering()) {
		builderSprite->update(deltaTime);
		notext = false;
		helpSprite->changeAnimation(BUILDERT);
	}
	else if (surrenderButton->isHovering()) {
		explosionSprite->update(deltaTime);
		notext = false;
		helpSprite->changeAnimation(SURRENDERT);
	}
	else if (playPauseButton->isHovering()){
		notext = false;
		helpSprite->changeAnimation(PLAYPAUSET);
	}
	else if (fastButton->isHovering()){
		notext = false;
		helpSprite->changeAnimation(FASTT);
	}
	else notext = true; 

	if (fastForward) {
		fastSprite->update(deltaTime); 
	}
}

int GUI::getSpeedState() {
	if (paused) {
		if (fastForward) {
			cout << "error" << endl;
		}
		return 1; 
	}
	else {
		if (fastForward)
			return 2; 
	}
	return 0; 
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

	if (!notext) helpSprite->render(); 

	playPauseButton->render();	
	playPauseSprite->render(); 

	stopperButton->render();
	stopperSprite->render();  

	diggerButton->render(); 
	diggerSprite->render(); 

	basherButton->render();
	basherSprite->render(); 

	climberButton->render();
	climberSprite->render();

	builderButton->render();
	builderSprite->render();

	surrenderButton->render(); 
	explosionSprite->render(); 

	fastButton->render();
	fastSprite->render(); 
}

void GUI::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	playPauseButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton); 
	stopperButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	diggerButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	basherButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	climberButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	builderButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	surrenderButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	fastButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);

	if (playPauseButton->isClicked()) {
		paused = !paused; 
		if (!paused) { 
			playPauseSprite->changeAnimation(PAUSE);
		}
		else {
			playPauseSprite->changeAnimation(PLAY);
			if (fastForward) {
				fastSprite->changeAnimation(FAST);
				fastForward = false; 
			}
		}
	}

	if (fastButton->isClicked()) {
		fastForward = !fastForward; 
		if (!fastForward) {
			fastSprite->changeAnimation(FAST);
		}
		else {
			if (paused) {
				playPauseSprite->changeAnimation(PAUSE);
				paused = false; 
			}
		}

	}
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



