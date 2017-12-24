#include <assert.h>
#include <iostream>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

class Sprites{
protected:
	ALLEGRO_BITMAP* image;

	//########################################
	// index se poio upsos sto bitmap vrisketai to sprite
	//########################################
	unsigned index;						// poso katw prepei na pas sto array wste na pareis to katalhlo sprite	
	unsigned spriteWidth;
	unsigned spriteHeight;
	unsigned frameWidth;
	unsigned frameHeight;
	
	
	//########################################
	// maxFrame ola ta frames tou animation, currFrame curr animation frame
	//########################################
	unsigned maxFrame;					// frames ��� animation
	unsigned currFrame;

	//########################################
	// frameDelay animation delay, frameCount posa animation exoun perasei
	//########################################
	unsigned frameCount;
	unsigned frameDelay;

	//########################################
	// vel taxuthta, dir kateu8unsh
	//########################################
	unsigned velocityX;
	unsigned velocityY;
	int directionX;
	int directionY;
	unsigned positionX;
	unsigned positionY;

	int animationDirection;
public:
	Sprites(
		ALLEGRO_BITMAP* im, 
		unsigned ind, unsigned sWidth, unsigned sHeight,
		unsigned bWidth, unsigned bHeight, 
		unsigned mFrame, unsigned fDelay,
		unsigned velX, unsigned velY, int dirX, int dirY,
		int animationDir, unsigned posX, unsigned posY)
	{
		
		assert(image != NULL);
		assert(sWidth != 0);
		assert(sHeight != 0);

		image = im;
		index = ind;
		spriteWidth = sWidth;
		spriteHeight = sHeight;
		frameWidth = bWidth;
		frameHeight = bHeight;
	
		currFrame = 0;
		frameCount = 0;
		maxFrame = mFrame;
		frameDelay = fDelay;

		velocityX = velX;
		velocityY = velY;
		directionX = dirX;
		directionY = dirY;
		positionX = posX;
		positionY = posY;

		animationDirection = animationDir;
	}	
	
	

	void Draw()
	{
		al_draw_bitmap_region(image, currFrame*spriteWidth, index, spriteWidth, spriteHeight ,positionX, positionY, 0);
	}
	
	void moveRight()
	{
		positionX += (5 * velocityX);
		if(positionX > 600){
			positionX = 600;
		}

	}
	void moveLeft()
	{
		positionX -= (5 * velocityX);
		if(positionX < 0)
			positionX = 0;
	}
	void moveUp()
	{
		positionY -= (5 * velocityX);
		if(positionY < 0)
			positionY = 0;
	}
	void moveDown()
	{
		positionY += (5 * velocityX);
		if(positionY > 600)
			positionY = 600;
	}
	void animationUpdate()
	{
			if(++frameCount >= frameDelay)
			{
				currFrame += animationDirection;
				if(currFrame >= maxFrame)
				{
					currFrame = 0;
				}
				else if(currFrame <= 0)
				{
					currFrame = maxFrame - 1;
				}
				frameCount = 0;
			}
		
	}
	unsigned getCurrFrame(){ return currFrame; }
	unsigned getFrameCount(){ return frameCount; }
	unsigned getMaxFrame(){ return maxFrame; }
	unsigned getFrameDelay(){ return frameDelay; }

	void toString()
	{
		std::cout << "\n\n index:" << index << "\n width: " << spriteWidth << "\n height: " << spriteHeight <<
			"\n frameWidth: "<< frameWidth << "\n frameHeight: "<< frameHeight << 
			"\n maxFrame: " << maxFrame << "\n currFrame: " << currFrame << "\n frameCount: " 
			<< frameCount << "\n frameDelay: "<<frameDelay << "\n velX: " << velocityX << "\n velY: " << velocityY <<
			"\n dirX: " << directionX << "\n dirY" << directionY << "\n";
	}
	
};
	


