#ifndef _DIGIT_INCLUDE
#define _DIGIT_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "TexturedQuad.h"
#include "InteractiveQuad.h"
#include "Sprite.h"

class Digit
{

public:
	Digit();
	~Digit();

	void init(glm::vec2 initialPos, glm::ivec2 quadSize);
	void render();
	void setPosition(glm::vec2 position); 
	void changeDigit(int desiredNum);

private:
	void initShaders();

private:
	enum NUM {
		ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE
	};

	vector<int> numbers = { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE };

	glm::vec2 position; 

	Texture digitSheet;
	Sprite *digitSprite;

	ShaderProgram simpleTexProgram;
	glm::mat4 projection;
};


#endif // _DIGIT_INCLUDE