#ifndef _INTERACTIVEQUAD_INCLUDE
#define _INTERACTIVEQUAD_INCLUDE


#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 


class InteractiveQuad
{

public:
	InteractiveQuad();
	// Textured quads can only be created inside an OpenGL context
	static InteractiveQuad *createInteractiveQuad(glm::vec2 &initialPosition, glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);

	InteractiveQuad(glm::vec2 &initialPosition, glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);

	void render() const;
	void free();

	//frameNum: 0 -> Idle 1 -> Hover 2 -> Clicked Extensible
	void setOffsetIdle(const glm::vec2 &frame);
	void setOffsetHover(const glm::vec2 &frame);
	void setOffsetClick(const glm::vec2 &frame);

	void mouseEvent(int x, int y, bool bLeftMouse, bool bRightMouse);

	void setPosition(const glm::vec2 &newPos);
	glm::vec2 position() const;
	glm::vec2 &position();

private:
	int frame; //0, 1, 2
	vector<glm::vec2> frameOrigins;
	vector<bool> validFrames;
	Texture *texture;
	ShaderProgram *shaderProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::vec2 pos;
	glm::vec2 size;
	glm::vec2 texCoordDispl;
	bool clicked = false;

	bool isInsideQuad(int x, int y);

};


#endif // _SPRITE_INCLUDE
