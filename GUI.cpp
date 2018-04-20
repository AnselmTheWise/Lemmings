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

	stopperNumber.init(glm::vec2(22.f / coordToPixel, (-62.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(4, 4));
	diggerNumber.init(glm::vec2(72.f / coordToPixel, (-62.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(4, 4));
	basherNumber.init(glm::vec2(122.f / coordToPixel, (-62.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(4, 4));
	climberNumber.init(glm::vec2(172.f / coordToPixel, (-62.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(4, 4));
	builderNumber.init(glm::vec2(222.f / coordToPixel, (-62.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(4, 4));
	portalNumber.init(glm::vec2(272.f / coordToPixel, (-62.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(4, 4));

	float offset = 8.f; 

	time3.init(glm::vec2((840.f+offset) / coordToPixel, (-35.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(8, 8));
	time2.init(glm::vec2((862.f+offset) / coordToPixel, (-35.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(8, 8));
	time1.init(glm::vec2((894.f+offset) / coordToPixel, (-35.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(8, 8));
	time0.init(glm::vec2((916.f+offset) / coordToPixel, (-35.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(8, 8));

	float offset1 = -8.f; 
	lemmingsleft1.init(glm::vec2((800.f + offset1) / coordToPixel, (-35.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(8, 8));
	lemmingsleft1.changeDigit(3); 
	lemmingsleft0.init(glm::vec2((822.f + offset1) / coordToPixel, (-35.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(8, 8));
	lemmingsleft0.changeDigit(3); 

	float offset2 = 14.f; 
	score3.init(glm::vec2((886.f+offset2) / coordToPixel, (-82.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(4, 4));
	score2.init(glm::vec2((898.f+offset2) / coordToPixel, (-82.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(4, 4));
	score1.init(glm::vec2((910.f+offset2) / coordToPixel, (-82.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(4, 4));
	score0.init(glm::vec2((922.f+offset2) / coordToPixel, (-82.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(4, 4));

	level.init(glm::vec2(810.f / coordToPixel, (-82.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::ivec2(4, 4));

	glm::vec2 geom[2] = { glm::vec2(0.f, (-90.f / coordToPixel) + float(CAMERA_HEIGHT)) , glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	barQuad = TexturedQuad::createTexturedQuad(geom, texCoords, simpleTexProgram);

	interfaceBar.loadFromFile("images/barTest6.png", TEXTURE_PIXEL_FORMAT_RGBA);
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

	basherSheet.loadFromFile("images/basher2a.png", TEXTURE_PIXEL_FORMAT_RGBA);
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

	portalButtonTexture.loadFromFile("images/ButtonTest.png", TEXTURE_PIXEL_FORMAT_RGBA);

	portalButton = InteractiveQuad::createInteractiveQuad(glm::vec2(258.f / coordToPixel, (-50.f / coordToPixel) + float(CAMERA_HEIGHT)), glm::vec2(42.f / coordToPixel, 42.f / coordToPixel), glm::vec2(1.f / 3.f, 1.f), &portalButtonTexture, &simpleTexProgram);
	portalButton->setOffsetIdle(glm::vec2(0.f, 0.f));
	portalButton->setOffsetHover(glm::vec2(1.f / 3.f, 0.f));
	portalButton->setOffsetClick(glm::vec2(2.f / 3.f, 0.f));

	portalSheet.loadFromFile("images/bluePortal.png", TEXTURE_PIXEL_FORMAT_RGBA);
	portalSheet.setMinFilter(GL_NEAREST);
	portalSheet.setMagFilter(GL_NEAREST);

	portalSprite = Sprite::createSprite(glm::ivec2(8, 13), glm::vec2(0.25, 1.0), &portalSheet, &simpleTexProgram);
	portalSprite->setNumberAnimations(16);

	portalSprite->setAnimationSpeed(PORTAL, 4);
	for (int i = 0; i<4; i++)
		portalSprite->addKeyframe(PORTAL, glm::vec2(float(i) / 4, 0.0f));

	portalSprite->changeAnimation(PORTAL);
	portalSprite->setPosition(glm::vec2(267.f / coordToPixel, (-48.f / coordToPixel) + float(CAMERA_HEIGHT)));

	helpSheet.loadFromFile("images/helptext.png", TEXTURE_PIXEL_FORMAT_RGBA);
	helpSheet.setMinFilter(GL_NEAREST);
	helpSheet.setMagFilter(GL_NEAREST);

	helpSprite = Sprite::createSprite(glm::ivec2(40, 4), glm::vec2(1.0, 0.1), &helpSheet, &simpleTexProgram);
	helpSprite->setNumberAnimations(16); 

	helpSprite->setAnimationSpeed(STOPPERT, 1);
	helpSprite->addKeyframe(STOPPERT, glm::vec2(0.0f, 0.0f));
	helpSprite->setAnimationSpeed(DIGGERT, 1);
	helpSprite->addKeyframe(DIGGERT, glm::vec2(0.0f ,0.1f));
	helpSprite->setAnimationSpeed(BASHERT, 1);
	helpSprite->addKeyframe(BASHERT, glm::vec2(0.0f, 0.2f));
	helpSprite->setAnimationSpeed(CLIMBERT, 1);
	helpSprite->addKeyframe(CLIMBERT, glm::vec2(0.0f, 0.3f));
	helpSprite->setAnimationSpeed(BUILDERT, 1);
	helpSprite->addKeyframe(BUILDERT, glm::vec2(0.0f, 0.4f));
	helpSprite->setAnimationSpeed(SURRENDERT, 1);
	helpSprite->addKeyframe(SURRENDERT, glm::vec2(0.0f, 0.5f));
	helpSprite->setAnimationSpeed(PLAYPAUSET, 1);
	helpSprite->addKeyframe(PLAYPAUSET, glm::vec2(0.0f, 0.6f));
	helpSprite->setAnimationSpeed(FASTT, 1);
	helpSprite->addKeyframe(FASTT, glm::vec2(0.0f, 0.7f));
	helpSprite->setAnimationSpeed(PORTALT, 1);
	helpSprite->addKeyframe(PORTALT, glm::vec2(0.0f, 0.8f));
	
	helpSprite->changeAnimation(STOPPERT);
	helpSprite->setPosition(glm::vec2(8.f / coordToPixel, (-82.f / coordToPixel) + float(CAMERA_HEIGHT)));

	scoreSheet.loadFromFile("images/score.png", TEXTURE_PIXEL_FORMAT_RGBA);
	scoreSheet.setMinFilter(GL_NEAREST);
	scoreSheet.setMagFilter(GL_NEAREST);

	scoreSprite = Sprite::createSprite(glm::ivec2(20, 4), glm::vec2(1.0, 1.0), &scoreSheet, &simpleTexProgram);
	scoreSprite->setNumberAnimations(16);

	scoreSprite->setAnimationSpeed(SCORE, 1);
	scoreSprite->addKeyframe(SCORE, glm::vec2(0.0f, 0.0f));

	scoreSprite->changeAnimation(SCORE);
	scoreSprite->setPosition(glm::vec2(838.f / coordToPixel, (-82.f / coordToPixel) + float(CAMERA_HEIGHT)));

	timeSheet.loadFromFile("images/time.png", TEXTURE_PIXEL_FORMAT_RGBA);
	timeSheet.setMinFilter(GL_NEAREST);
	timeSheet.setMagFilter(GL_NEAREST);

	timeSprite = Sprite::createSprite(glm::ivec2(20, 4), glm::vec2(1.0, 1.0), &timeSheet, &simpleTexProgram);
	timeSprite->setNumberAnimations(16);

	timeSprite->setAnimationSpeed(TIME, 1);
	timeSprite->addKeyframe(TIME, glm::vec2(0.0f, 0.0f));

	timeSprite->changeAnimation(TIME);
	timeSprite->setPosition(glm::vec2(870.f / coordToPixel, (-55.f / coordToPixel) + float(CAMERA_HEIGHT)));

	levelSheet.loadFromFile("images/level.png", TEXTURE_PIXEL_FORMAT_RGBA);
	levelSheet.setMinFilter(GL_NEAREST);
	levelSheet.setMagFilter(GL_NEAREST);

	levelSprite = Sprite::createSprite(glm::ivec2(20, 4), glm::vec2(1.0, 1.0), &levelSheet, &simpleTexProgram);
	levelSprite->setNumberAnimations(16);

	levelSprite->setAnimationSpeed(LEVEL, 1);
	levelSprite->addKeyframe(LEVEL, glm::vec2(0.0f, 0.0f));

	levelSprite->changeAnimation(LEVEL);
	levelSprite->setPosition(glm::vec2(755.f / coordToPixel, (-82.f / coordToPixel) + float(CAMERA_HEIGHT)));

	inSheet.loadFromFile("images/in.png", TEXTURE_PIXEL_FORMAT_RGBA);
	inSheet.setMinFilter(GL_NEAREST);
	inSheet.setMagFilter(GL_NEAREST);

	inSprite = Sprite::createSprite(glm::ivec2(10, 4), glm::vec2(1.0, 1.0), &inSheet, &simpleTexProgram);
	inSprite->setNumberAnimations(16);

	inSprite->setAnimationSpeed(IN, 1);
	inSprite->addKeyframe(IN, glm::vec2(0.0f, 0.0f));

	inSprite->changeAnimation(IN); 
	inSprite->setPosition(glm::vec2(801.f / coordToPixel, (-55.f / coordToPixel) + float(CAMERA_HEIGHT)));
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
	else if (portalButton->isHovering()) {
		portalSprite->update(deltaTime);
		notext = false; 
		helpSprite->changeAnimation(PORTALT); 
	}
	else notext = true; 

	if (fastForward) {
		fastSprite->update(deltaTime); 
	}
}

void GUI::setTime(float currentTime) {
	int minutes = glm::floor(currentTime / 60000.f);
	int seconds = glm::floor(currentTime / 1000.f) - (float(minutes)*60.f);
	
	int unim = minutes % 10;
	int decm = glm::floor(minutes/10); 
	time3.changeDigit(decm); 
	time2.changeDigit(unim); 
	
	int unis = seconds % 10;
	int decs = glm::floor(seconds/10); 
	time1.changeDigit(decs);
	time0.changeDigit(unis); 
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

	portalButton->render(); 
	portalSprite->render(); 

	surrenderButton->render();
	explosionSprite->render();

	fastButton->render();
	fastSprite->render();

	stopperNumber.render(); 
	diggerNumber.render(); 
	basherNumber.render();
	climberNumber.render();
	builderNumber.render();
	portalNumber.render(); 

	time3.render(); 
	time2.render(); 
	time1.render(); 
	time0.render(); 

	lemmingsleft1.render(); 
	lemmingsleft0.render();

	score3.render(); 
	score2.render(); 
	score1.render(); 
	score0.render(); 

	scoreSprite->render(); 

	levelSprite->render(); 

	timeSprite->render(); 
	inSprite->render(); 

	level.render(); 

}

void GUI::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	playPauseButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton); 
	stopperButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	diggerButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	basherButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	climberButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	builderButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
	portalButton->mouseEvent(mouseX, mouseY, bLeftButton, bRightButton);
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

int GUI::getButtonClicked() {
	if (stopperButton->isClicked()) {
		return 0;
	}
	else if (diggerButton->isClicked()) {
		return 1;
	}
	else if (basherButton->isClicked()) {
		return 2;
	}
	else if (climberButton->isClicked()) {
		return 3;
	}
	else if (builderButton->isClicked()) {
		return 4;
	}
	else if (surrenderButton->isClicked()) {
		return 5;
	}
	else if (playPauseButton->isClicked()) {
		return 6;
	}
	else if (fastButton->isClicked()) {
		return 7;
	}
	else if (portalButton->isClicked()) {
		return 8; 
	}
	return -1;
}

void GUI::maxTime(int time) {
	setTime(float(time)); 
}

void GUI::lemmingsLeft(int lemmingsL) {
	if (lemmingsL >= 0) {
		int unil = lemmingsL % 10;
		int decl = glm::floor(lemmingsL / 10);
		lemmingsleft0.changeDigit(unil);
		lemmingsleft1.changeDigit(decl);
	}
}

void GUI::updatePowers(vector<int> powersLeft) {
	stopperNumber.changeDigit(powersLeft[0]);
	diggerNumber.changeDigit(powersLeft[1]);
	basherNumber.changeDigit(powersLeft[2]);
	climberNumber.changeDigit(powersLeft[3]);
	builderNumber.changeDigit(powersLeft[4]);
	portalNumber.changeDigit(powersLeft[5]); 
}

void GUI::setScore(int score) {
	cout << score << endl;
	int unis = score % 10; 
	int decs = int(glm::floor(score/10)) %10; 
	int cens = int(glm::floor(score/100)) %10; 
	int mils = int(glm::floor(score/1000)) %10; 

	cout << "Unis: " << unis << endl; 
	cout << "Decs: " << decs << endl;
	cout << "Cens: " << cens << endl;
	cout << "Mils: " << mils << endl;

	score0.changeDigit(unis);
	score1.changeDigit(decs); 
	score2.changeDigit(cens);
	cout << "antea" << endl; 
	score3.changeDigit(mils); 
	cout << "despues" << endl; 
}

void GUI::setLevel(int level) {
	this->level.changeDigit(level); 
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



