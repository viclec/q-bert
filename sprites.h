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
	unsigned maxFrame;					// frames του animation
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
	bool animate;
	bool move_to_right_disk;
	bool falling_out_of_bounds;
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
		falling_out_of_bounds=false;
		move_to_right_disk=false;
		animationDirection = animationDir;
		animate = false;
	}	
	
	void setpositionX(unsigned i){
		positionX=i;
	}

	void setpositionY(unsigned i){
		positionY=i;
	}

	void set_Fall(bool arg ){
		falling_out_of_bounds=arg;
	}

	void gotodisk(){
		move_to_right_disk=true;
	}
	void Draw()
	{
		al_draw_bitmap_region(image, currFrame*spriteWidth, index, spriteWidth, spriteHeight ,positionX, positionY, 0);
	}

	void moveUpRight()
	{
		moveDown(40/2);
		moveUp(26/2);
		moveRight(16/2);
		animate = true;
	}

	void moveUpLeft()
	{
		moveDown(40/2);
		moveUp(26/2);
		moveLeft(16/2);
		animate = true;
	}

	void moveDownRight()
	{
		moveDown(40/2);
		moveDown(26/2);
		moveRight(16/2);
		animate = true;
	}

	void moveDownLeft()
	{
		moveDown(40/2);
		moveDown(26/2);
		moveLeft(16/2);
		animate = true;
	}
	
	void moveRight(int interval)
	{
		positionX += (interval * velocityX);
		if(positionX > 600){
			positionX = 600;
		}

	}
	void moveLeft(int interval)
	{
		positionX -= (interval * velocityX);
		if(positionX < 0)
			positionX = 0;
	}
	void moveUp(int interval)
	{
		positionY -= (interval * velocityX);
		if(positionY < 0)
			positionY = 0;
	}
	void moveDown(int interval)
	{
		positionY += (interval * velocityX);
		if(positionY > 600)
			positionY = 600;
	}
	void moveCurve()
	{
		positionY -= 40;
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
	void playerAnimationUpdate(unsigned i)
	{	
		
		if(falling_out_of_bounds==true){
			moveDown(15);
		}
		
		else if(animate){
			
			if(++frameCount >= frameDelay)
			{
				currFrame += animationDirection;

				 
					
					
				if(currFrame >= maxFrame)
				{   
					  
				    if(i == 0)
						moveUpRight();
					else if(i == 1)
						moveDownLeft();
					else if(i == 2)
						moveUpLeft();
					else if(i == 3)
						moveDownRight();
					else 
						assert(0);
					 
					currFrame = 0;

					
				}
				else if(currFrame <= 0)
				{
					currFrame = maxFrame - 1;
				}
				frameCount = 0;
			}
			if(frameCount == 0 && currFrame == 0){
				animate = false;
			}
		}
	}
	unsigned getCurrFrame(){ return currFrame; }
	unsigned getFrameCount(){ return frameCount; }
	unsigned getMaxFrame(){ return maxFrame; }
	unsigned getFrameDelay(){ return frameDelay; }
	unsigned getPositionX(){ return positionX; }
	unsigned getPositionY(){ return positionY; }
	unsigned getFrameWidth(){ return frameWidth; }
	unsigned getFrameHeight(){ return frameHeight; }
	bool getFallingStatus(){return falling_out_of_bounds;}
	bool getAnimationStatus(){ return animate; }
	void toString()
	{
		std::cout << "\n\n index:" << index << "\n width: " << spriteWidth << "\n height: " << spriteHeight <<
			"\n frameWidth: "<< frameWidth << "\n frameHeight: "<< frameHeight << 
			"\n maxFrame: " << maxFrame << "\n currFrame: " << currFrame << "\n frameCount: " 
			<< frameCount << "\n frameDelay: "<<frameDelay << "\n velX: " << velocityX << "\n velY: " << velocityY <<
			"\n dirX: " << directionX << "\n dirY" << directionY << "\n";
	}
	
};
	


