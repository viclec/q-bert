#include <assert.h>
#include <iostream>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <string>
#include <sstream>

int play = 1 ;

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
	unsigned draw;
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
	unsigned losingsprite;
	int animationDirection;
	bool animate;
	bool move_to_right_disk;
	bool falling_out_of_bounds;
	bool enemyMove;
	int lives;
	bool disable;
	unsigned collision;
	
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

		
	    losingsprite = 0;
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
		enemyMove = true;
		lives = 29;
		draw=1;
		disable = 0;
		collision = 0;
		
	}	
	
	void setpositionX(unsigned i){
		positionX=i;
	}

	void setIndex(unsigned i){ index = i; }
	void setpositionY(unsigned i){
		positionY=i;
	}

	void set_Fall(bool arg ){
		falling_out_of_bounds=arg;
	}

	void set_Curse(unsigned init ,unsigned x ,unsigned y){
			index = init;
			spriteWidth = x;
			spriteHeight = y;
			collision=1;
			play=0;

	}

	void reset_Curse(){
			index = 0;
			spriteWidth = 36;
			spriteHeight = 48;
			
	}



	void gotodisk(){
		move_to_right_disk=true;
	}
	void zerolives(){
		lives=0;
	}
	void reducelives(){
	 lives--;
	}

	void qbert_set_index(unsigned i){
		index = i;
	}

	void setdraw(int i){
		draw = i;
	}

	void setdelay(int i){
		frameDelay = i;
	}
	void lose_live(){
	 lives=lives-9;
	}
	void Draw()
	{   
		al_draw_bitmap_region(image, currFrame*spriteWidth, index, spriteWidth, spriteHeight ,positionX, positionY, 0);
		
	}

	void qbert_set_collision(unsigned i){
		 collision = i;
	}
	unsigned get_index(){
		return index;
	}

	unsigned qbert_get_collision(){
		return collision ;
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
	void diskmoveUp(){
		positionY=positionY-2;
		Draw();	
	}

	void diskmoveRight(){
		positionX=positionX+2;
		Draw();	
	}

	void diskmoveLeft(){
		positionX=positionX-2;
		Draw();	
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
		if(positionY > 355){
			positionY = 145;
			positionX = 304;
			lives--;
			falling_out_of_bounds = false;
		}
	}

	void moveDownBall(int interval)
	{
		positionY += (interval * velocityX);
		if(positionY > 355){
			positionY = 165;
			positionX = 304;
			setIndex(213);
			lives=0;
		}
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
	void animationSnake(Sprites qbert)
	{
		if(++frameCount >= frameDelay)
		{
			currFrame += animationDirection;
						
			if(currFrame == 1)
			{	
				//moveUp(20);
			}else if(currFrame == 2)
			{   
				 chaseQbert(qbert);
			
			}
					
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
	void animationMove()
	{
		if(++frameCount >= frameDelay)
			{	
				currFrame += animationDirection;
					if(enemyMove)
					{	
						if(currFrame == 1)
						{	
							moveUp(20);
						}else if(currFrame == 2)
						{   
							moveDownBall(20);
					
					   	    moveDownBall(26);
							if(lives!=0){
							if(rand()%2 == 0){
									moveRight(16);
								}else{
									moveLeft(16);
								}
							}else{
								lives=1;
							}
						}
					}
					enemyMove = !enemyMove;
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

	void chaseQbert(Sprites qbert)
	{
		if(qbert.getPositionX() < positionX && qbert.getPositionY() < positionY)
		{
			moveUp(26);
		    moveLeft(16);  
		}
		else if(qbert.getPositionX() < positionX && qbert.getPositionY() > positionY)
		{
			moveDown(26);
		    moveLeft(16);  
		}
		else if(qbert.getPositionX() > positionX && qbert.getPositionY() < positionY)
		{
			moveUp(26);
		    moveRight(16);  
		}
		else if(qbert.getPositionX() > positionX && qbert.getPositionY() > positionY)
		{
			moveDown(26);
		    moveRight(16);  
		}
		else if(qbert.getPositionX() == positionX && qbert.getPositionY() < positionY)
		{
			if(rand() % 2 == 0)
			{
				moveUp(26);
		        moveLeft(16);  
			}
			else
			{
				moveUp(26);
		        moveRight(16); 	
			}
		}
		else if(qbert.getPositionX() == positionX && qbert.getPositionY() > positionY)
		{
			if(rand() % 2 == 0)
			{
				moveDown(26);
		        moveLeft(16);  
			}
			else
			{
				moveDown(26);
		        moveRight(16);  		
			}
		}
		else if(qbert.getPositionY() == positionY && qbert.getPositionX() < positionX)
		{
			moveUp(26);
		    moveLeft(16);  
		}
		else if(qbert.getPositionY() == positionY && qbert.getPositionX() > positionX)
		{
			moveUp(26);
		    moveRight(16);  
		}
		else
		{
			//moveDown(40/2);
		}
	}
	void playerAnimationUpdate(unsigned i)
	{	
		if(collision == 1 ){
		  losingsprite ++;
		}
		
		if(falling_out_of_bounds==true){
			
			moveDown(5);
		}
		
		else if(animate){
			
			if(++frameCount >= frameDelay)
			{
				currFrame += animationDirection;
				
				if(currFrame >= maxFrame)
				{  
				    if(i == 0){
						moveUpRight();
					}
					else if(i == 1){
						moveDownLeft();
					}
					else if(i == 2){
						moveUpLeft();
					}
					else if(i == 3){
						moveDownRight();
					}
					else {
						assert(0);
					}
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
		
		if(collision == 1 && losingsprite > 60){
			
		    reset_Curse();
		    collision=0;
		    play = 1;
			losingsprite = 0;
		}


	}
	bool qbertcollision(Sprites enemy)
	{
		if( positionX + frameWidth > enemy.positionX - enemy.frameWidth &&
			positionX - frameWidth < enemy.positionX + enemy.frameWidth &&
			positionY + frameHeight > enemy.positionY - enemy.frameHeight &&
			positionY - frameHeight < enemy.positionY + enemy.frameHeight)
		{	

			set_Curse(631 , 36 ,48 );
	    
	
//std::cout << "collision\n";
			
			return true;
		}
		return false;
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
	void setAnimationStatus(bool state){ animate = state; }
	int getlives(){return lives;}
	unsigned getdraw(){return draw;}
	unsigned get_losing_sprite(){
		return losingsprite;
	}
	void toString()
	{
		std::cout << "\n\n index:" << index << "\n width: " << spriteWidth << "\n height: " << spriteHeight <<
			"\n frameWidth: "<< frameWidth << "\n frameHeight: "<< frameHeight << 
			"\n maxFrame: " << maxFrame << "\n currFrame: " << currFrame << "\n frameCount: " 
			<< frameCount << "\n frameDelay: "<<frameDelay << "\n velX: " << velocityX << "\n velY: " << velocityY <<
			"\n dirX: " << directionX << "\n dirY" << directionY << "\n";
	}
	
};
	


