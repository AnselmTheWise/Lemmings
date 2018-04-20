#ifndef _SCENE2_INCLUDE
#define _SCENE2_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "TexturedQuad.h"
#include "Lemming.h"
#include "GUI.h"
#include "WinLoseScreen.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene2
{

public:
	Scene2();
	~Scene2();

	void init();
	void update(int deltaTime);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	int getStatus();

private:
	void initShaders();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);
	void removeElement(vector<Lemming*> &v, int index);
	void selfRender();
	void clearLemmings();

private:
	TexturedQuad* barQuad;
	Texture colorTexture, interfaceBar, entranceSpritesheet, exitSpritesheet;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	glm::mat4 projection;
	vector<Lemming*> lemmings;
	GUI interface1;
	int lastTimeLemmingSpawned;
	glm::vec2 entrance;
	glm::vec2 exit;
	Sprite* entranceSprite;
	Sprite* exitSprite;
	int lemmingsToSpawn, lemmingsToArrive, lemmingsSpawned, lemmingsArrived, nLemmings;
	bool won, lost;
	WinLoseScreen endScreen;
	int selectedLemming;
	int buttonClicked;
	int score;

	vector<int> powersLeft;

	enum ANIMATION_ID { ENTRANCE_OPENING, EXIT_FIRE };
	enum RENDERING_ELEMENT { SCENE, WIN, LOSE };

	RENDERING_ELEMENT renderingElement;

};


#endif // _SCENE_INCLUDE

