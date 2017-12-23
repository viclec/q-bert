#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

class Sprites{
protected:
	ALLEGRO_BITMAP* image;
	int spriteWidth;
	int spriteHeight;
	int boundWidth;
	int boundHeight;

public:
	Sprites(ALLEGRO_BITMAP* im, int sWidth, int sHeight, int bWidth, int bHeight){
		assert(image != NULL);
		assert(sWidth != 0);
		assert(sHeight != 0);

		image = im;
		spriteWidth = sWidth;
		spriteHeight = sHeight;
		boundWidth = bWidth;
		boundHeight = bHeight;
	}	
}