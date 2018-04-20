#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	bLeftMouse = bRightMouse = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	//scene.init();
	renderingElement = MAIN_MENU;
	mainMenu.init();
}

bool Game::update(int deltaTime)
{
	if (fast) deltaTime = 5 * deltaTime;
	if (paused) deltaTime = 0;
	if (renderingElement == MAIN_MENU) {
		int status = mainMenu.getStatus(); //0 -> Executing //1 -> Level1 //2 -> Level2 //3 -> Level3 //4 -> Exit
		if (status == 1) {
			scene.init();
			renderingElement = LEVEL_1;
		}
		else if (status == 4) {
			bPlay = false;
		}
	}
	else if (renderingElement == LEVEL_1) {
		scene.update(deltaTime);
		int status = scene.getStatus(); //0-> Executing //1 -> Repeat level //2 -> Level2 //3 -> Level3 //4 -> MainMenu //5 -> Exit
		if (status == 1) {
			scene.init();
		}
		else if (status == 2) {
			//Level2 selected
		}
		else if (status == 3) {
			//Level3 selected
		}
		else if (status == 4) {
			mainMenu.init();
			renderingElement = MAIN_MENU;
		}
		else if (status == 5) {
			bPlay = false;
		}
	}
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (renderingElement == MAIN_MENU) {
		mainMenu.render();
	}
	else if (renderingElement == LEVEL_1) {
		scene.render();
	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	if (key == 'f') {
		fast = !fast;
	}
	if (key == 'p') {
		paused = !paused;
	}
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;
	if (renderingElement == MAIN_MENU) {
		mainMenu.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
	else if (renderingElement == LEVEL_1) {
		scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
}

void Game::mousePress(int button)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		bLeftMouse = true;
		if (renderingElement == MAIN_MENU) {
			mainMenu.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		}
		else if (renderingElement == LEVEL_1) {
			scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		}
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		bRightMouse = true;
		if (renderingElement == MAIN_MENU) {
			mainMenu.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		}
		else if (renderingElement == LEVEL_1) {
			scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		}
	}
}

void Game::mouseRelease(int button)
{
	if(button == GLUT_LEFT_BUTTON)
		bLeftMouse = false;
	else if(button == GLUT_RIGHT_BUTTON)
		bRightMouse = false;
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





