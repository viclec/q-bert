#include <assert.h>
#include <iostream>
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

class Sprites{
public:
	ALLEGRO_BITMAP* image;
	unsigned index;						// poso katw prepei na pas sto array wste na pareis to katalhlo sprite	
	unsigned spriteWidth;
	unsigned spriteHeight;
	unsigned boundWidth;
	unsigned boundHeight;

	unsigned maxFrame;					// frames του animation
	unsigned currFrame;

	unsigned frameCount;
	unsigned frameDelay;


public:
	Sprites(
		ALLEGRO_BITMAP* im, 
		unsigned ind, unsigned sWidth, unsigned sHeight,
		unsigned bWidth, unsigned bHeight, 
		unsigned mFrame, unsigned fDelay)
	{
		
		assert(image != NULL);
		assert(sWidth != 0);
		assert(sHeight != 0);

		image = im;
		index = ind;
		spriteWidth = sWidth;
		spriteHeight = sHeight;
		boundWidth = bWidth;
		boundHeight = bHeight;
	
		currFrame = 0;
		frameCount = 0;
		maxFrame = mFrame;
		frameDelay = fDelay;
	}	
	
	void toString()
	{
		std::cout << "\n\n index:" << index << "\n width: " << spriteWidth << "\n height: " << spriteHeight <<
			"\n boundWidth: "<< boundWidth << "\n boundHeight: "<< boundHeight << 
			"\n maxFrame: " << maxFrame << "\n currFrame: " << currFrame << "\n frameCount: " 
			<< frameCount << "\n frameDelay: "<<frameDelay;
	}

	void Draw()
	{
		al_draw_bitmap_region(image, currFrame*spriteWidth, index, spriteWidth, spriteHeight ,0, 0, 0);
	}
};
	


