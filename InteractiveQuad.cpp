#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "InteractiveQuad.h"


InteractiveQuad *InteractiveQuad::createInteractiveQuad(glm::vec2 &initialPos, glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	InteractiveQuad *quad = new InteractiveQuad(initialPos, quadSize, sizeInSpritesheet, spritesheet, program);

	return quad;
}


InteractiveQuad::InteractiveQuad() 
{
}


InteractiveQuad::InteractiveQuad(glm::vec2 &initialPosition, glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	float vertices[24] = { 0.f, 0.f, 0.f, 0.f,
		quadSize.x, 0.f, sizeInSpritesheet.x, 0.f,
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f, 0.f, 0.f, 0.f,
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f, quadSize.y, 0.f, sizeInSpritesheet.y };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	pos = initialPosition;
	size = quadSize;
	frame = 0;
	clicked = false;
	frameOrigins.resize(3);
	validFrames.resize(3, false);
}

void InteractiveQuad::render() const
{
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	//cout << "Displacement: " << texCoordDispl.x << " " << texCoordDispl.y << endl;
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	shaderProgram->setTextureUnit("tex", 0);
	glActiveTexture(GL_TEXTURE0);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void InteractiveQuad::free()
{
	glDeleteBuffers(1, &vbo);
}

void InteractiveQuad::setOffsetIdle(const glm::vec2 &frame) {
	validFrames[0] = true;
	frameOrigins[0] = frame;
}

void InteractiveQuad::setOffsetHover(const glm::vec2 &frame) {
	validFrames[1] = true;
	frameOrigins[1] = frame;
}

void InteractiveQuad::setOffsetClick(const glm::vec2 &frame) {
	validFrames[2] = true;
	frameOrigins[2] = frame;
}

void InteractiveQuad::mouseEvent(int x, int y, bool bLeftMouse, bool bRightMouse) {
	if (isInsideQuad(x, y)) {
		//cout << "Inside the quad" << endl;
		if (bLeftMouse == true) {
			//cout << "The button is clicked" << endl;
			//Button clicked
			clicked = true;
			if (validFrames[2] == true) {
				//cout << "Valid frame, changing to clicked texture" << endl;
				texCoordDispl = frameOrigins[2];
			}
		}
		else {
			//cout << "Button not clicked" << endl;
			//Button not clicked
			//Button may be released
			clicked = false;
			if (validFrames[1] == true) {
				//cout << "Valid frame, changing to hover texture" << endl;
				texCoordDispl = frameOrigins[1];
			}
		}
	}
	else {
		//Mouse outside
		//cout << "Outside the quad" << endl;
		if (bLeftMouse == false) {
			//cout << "Button not clicked" << endl;
			clicked = false;
			if (validFrames[0] == true) {
				//cout << "Valid frame, changing to idle texture" << endl;
				texCoordDispl = frameOrigins[0];
			}
		}
		

	}
}

void InteractiveQuad::setPosition(const glm::vec2 &newPos)
{
	pos = newPos;
}

glm::vec2 InteractiveQuad::position() const
{
	return pos;
}

glm::vec2 &InteractiveQuad::position()
{
	return pos;
}

bool InteractiveQuad::isInsideQuad(int x, int y) {
	x = x / (960 / CAMERA_WIDTH)/* / 3*/;
	y = y / (480 / CAMERA_WIDTH) / 3;
	//cout << x << " " << y << " " << pos.x << " " << pos.y << endl;
	if (x >= pos.x && x <= pos.x + size.x && y >= pos.y && y <= pos.y + size.y) {
		return true;
	}
	
	return false;
}




