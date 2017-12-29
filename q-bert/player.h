#include "sprites.h"

class Player : protected Sprites
{
private:
	int lifes;

public:
	void moveDown(ALLEGRO_BITMAP* im, 
		unsigned ind, unsigned sWidth, unsigned sHeight,
		unsigned bWidth, unsigned bHeight, 
		unsigned mFrame, unsigned fDelay,
		unsigned velX, unsigned velY, int dirX, int dirY,
		int animationDir, unsigned posX, unsigned posY, int zwes):
			Sprites(im, ind, sWidth, sHeight, bWidth, bHeight, mFrame, 
			fDelay, velX, velY, dirX, dirY, animationDir, posX, posy)
	{
		lifes = zwes;	
	};

	
};