#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


Scene::Scene()
{
	map = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}


void Scene::init()
{
	lemmingsToSpawn = 10;
	lemmingsToArrive = 5;
	lemmingsSpawned = lemmingsArrived = nLemmings = 0;

	interface1.init(); 

	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT))};
	glm::vec2 texCoords[2] = {glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, 160.f / 256.0f)};

	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile("images/fun1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile("images/fun1_mask.png", TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	entrance = glm::vec2(60, 30);
	entranceSpritesheet.loadFromFile("images/spawndoor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	entranceSpritesheet.setMinFilter(GL_NEAREST);
	entranceSpritesheet.setMagFilter(GL_NEAREST);
	entranceSprite = Sprite::createSprite(glm::vec2(45.f, 27.f), glm::vec2(1.f, 1.f / 10.f), &entranceSpritesheet, &simpleTexProgram);
	entranceSprite->setNumberAnimations(10);
	entranceSprite->setAnimationSpeed(ENTRANCE_OPENING, 12);
	for (int i = 0; i < 10; ++i) {
		entranceSprite->addKeyframe(ENTRANCE_OPENING, glm::vec2(0.f, float(i) / 10));
	}
	entranceSprite->changeAnimation(ENTRANCE_OPENING);
	entranceSprite->position() = glm::vec2(entrance.x - 15.f, entrance.y);
	exitSpritesheet.loadFromFile("images/exitdoor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	exitSpritesheet.setMinFilter(GL_NEAREST);
	exitSpritesheet.setMagFilter(GL_NEAREST);
	exitSprite = Sprite::createSprite(glm::vec2(37.f, 27.f), glm::vec2(1.f / 6.f, 1.f), &exitSpritesheet, &simpleTexProgram);
	exitSprite->setNumberAnimations(6);
	exitSprite->setAnimationSpeed(EXIT_FIRE, 12);
	for (int i = 0; i < 6; ++i) {
		exitSprite->addKeyframe(EXIT_FIRE, glm::vec2(float(i) / 6.f, 0.f));
	}
	exitSprite->changeAnimation(EXIT_FIRE);
	exitSprite->position() = glm::vec2(220, 105);
	lastTimeLemmingSpawned = 0;
	renderingElement = SCENE;
	selectedLemming = -1;
	buttonClicked = -1;
	powersLeft = { 10, 10, 10, 10, 10, 0 };
	interface1.updatePowers(powersLeft);
	interface1.lemmingsLeft(lemmingsToArrive);
	interface1.maxTime(2 * 60 * 1000);
	interface1.setLevel(1);
	score = 0;
	clearLemmings();
}

unsigned int x = 0;

void Scene::update(int deltaTime)
{
	interface1.update(deltaTime);
	if (interface1.getSpeedState() == 1) {
		deltaTime = 0;
	}
	else if (interface1.getSpeedState() == 2) {
		deltaTime *= 5;
	}
	if (renderingElement == SCENE) {
		currentTime += deltaTime;
		if (lemmingsSpawned < lemmingsToSpawn && currentTime >(24.f / 30.f) * 1000 && currentTime - lastTimeLemmingSpawned > 2000) {
			Lemming* lemming = new Lemming();
			lemming->init(entrance, simpleTexProgram);
			lemming->setMapMask(&maskTexture);
			lemmings.push_back(lemming);
			lastTimeLemmingSpawned = currentTime;
			++lemmingsSpawned; ++nLemmings;
		}
		for (int i = 0; i < lemmings.size(); ++i) {
			lemmings[i]->update(deltaTime);
			int st = lemmings[i]->getStatus();
			if (st == 1 || st == 2) {
				removeElement(lemmings, i);
				--nLemmings;
				if (st == 1) {
					++lemmingsArrived;
					score += 100;
					interface1.setScore(score);
					interface1.lemmingsLeft(lemmingsToArrive - lemmingsArrived);
					if (lemmingsArrived == lemmingsToArrive) {
						won = true;
					}
				}
				if (nLemmings == 0 && lemmingsArrived < lemmingsToArrive) {
					lost = true;
					renderingElement = LOSE;
					endScreen.init("Lose");
				}
				if ((nLemmings == 0 || currentTime > 2*60*1000) && lemmingsArrived > lemmingsToArrive) {
					renderingElement = WIN;
					endScreen.init("Win");
				}
			}
		}
		if (currentTime < (24.f / 30.f) * 1000) {
			entranceSprite->update(deltaTime);
		}
		exitSprite->update(deltaTime);
		if (2 * 60 * 1000 - currentTime < 0) {
			lost = true;
			renderingElement = LOSE;
			endScreen.init("Lose");
		}
		else {
			interface1.setTime(2 * 60 * 1000 - currentTime);
		}
	}
}

void Scene::removeElement(vector<Lemming*> &v, int index) {
	vector<Lemming*> vec;
	for (int i = 0; i < v.size(); ++i) {
		if (i != index) {
			vec.push_back(v[i]);
		}
	}
	v = vec;
}

void Scene::render()
{
	if (renderingElement == SCENE) {
		selfRender();
	}
	else if (renderingElement == WIN || renderingElement == LOSE) {
		endScreen.render();
	}
}

void Scene::selfRender() {
	glm::mat4 modelview;

	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	map->render(maskedTexProgram, colorTexture, maskTexture);

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	entranceSprite->render();
	exitSprite->render();
	for (int i = 0; i < lemmings.size(); ++i) {
		lemmings[i]->render();
	}

	interface1.render();
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if (renderingElement == SCENE) {
		for (int i = 0; i < lemmings.size(); ++i) {
			lemmings[i]->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
			if (lemmings[i]->isClicked()) {
				selectedLemming = i;
			}
		}
		interface1.mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
		int bt = interface1.getButtonClicked();
		if (bt != -1) {
			buttonClicked = bt;
		}
		if (buttonClicked > -1 && buttonClicked < 5 && selectedLemming >= 0 && selectedLemming < lemmings.size() && powersLeft[buttonClicked] > 0) {
			bool success = lemmings[selectedLemming]->setPower(buttonClicked);
			if (success) {
				--powersLeft[buttonClicked];
				interface1.updatePowers(powersLeft);
			}
		}
		selectedLemming = -1;
		if (bLeftButton)
			eraseMask(mouseX, mouseY);
		else if (bRightButton)
			applyMask(mouseX, mouseY);
	}
	else if (renderingElement == WIN || renderingElement == LOSE) {
		endScreen.mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	}
}

int Scene::getStatus() {
	if (renderingElement == WIN || renderingElement == LOSE) {
		int st = endScreen.getStatus();
		if (st == 2) return 2;
		return st;
	}
	return 0;
}

void Scene::clearLemmings() {
	for (int i = 0; i < lemmings.size(); ++i) {
		delete lemmings[i];
	}
	lemmings.clear();
}

void Scene::eraseMask(int mouseX, int mouseY)
{
	int posX, posY;
	
	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX/3 + 120;
	posY = mouseY/3;

	for(int y=max(0, posY-3); y<=min(maskTexture.height()-1, posY+3); y++)
		for(int x=max(0, posX-3); x<=min(maskTexture.width()-1, posX+3); x++)
			maskTexture.setPixel(x, y, 0);
}

void Scene::applyMask(int mouseX, int mouseY)
{
	int posX, posY;
	
	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX/3 + 120;
	posY = mouseY/3;

	for(int y=max(0, posY-3); y<=min(maskTexture.height()-1, posY+3); y++)
		for(int x=max(0, posX-3); x<=min(maskTexture.width()-1, posX+3); x++)
			maskTexture.setPixel(x, y, 255);
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if(!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if(!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



