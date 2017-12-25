#include "sprites.h"
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>


const int WIDTH = 800;
const int HEIGHT = 640;

ALLEGRO_BITMAP* bitmap = NULL;

double qbertX = 36;
double qbertY = 48;

double tileSizeX = 38;
double tileSizeY = 48;

double cubeX=0;
double cubeY=180;

double cubeSizeX = 38;
double cubeSizeY = 41;

bool keys[] = {false, false, false, false};
enum KEYS{UP, DOWN, LEFT, RIGHT};
enum CHARACTERS{QBERT, SNAKE, BALL, DISK, ISO};

int h1 = 7;
int h3 = 26;

double iso_cube_y (double row){
	return WIDTH/4 + (row-1)*h3;
}

void Compute_iso_cube_placement(double row,double col){
	double i;
	double j;

	for(i=0; i<row; i++){

		double y = iso_cube_y(i);
		double ys = y;
		double xs = HEIGHT/2 - i/2*cubeSizeX;

		for(j=0; j<=i; j++){
			double x = xs + (j-1)*cubeSizeX;
			double xc = x + cubeSizeX/2;
			double yc = y + h1;
			al_draw_bitmap_region(bitmap, cubeX , cubeY, cubeSizeX, cubeSizeY,xc, yc , 0);
		}
	}	
}

int main()
{	//==============================================
	// PROJECT VARIABLES
	//==============================================
	bool done = false;
	bool render = false;
	const float FPS = 60.0;
	const unsigned spriteHeightInBitmap[] = {0, 50, 115, 150, 178};

	const unsigned qbertY = 50;
	const unsigned qbertX = 37;
	const unsigned qbertFrames = 4;
	
	const unsigned snakeY = 65;
	const unsigned snakeX = 41;
	const unsigned snakeFrames = 3;
	
	const unsigned ballY = 35;
	const unsigned ballX = 38;
	const unsigned ballFrames = 3;

	const unsigned diskY = 28;
	const unsigned diskX = 37;
	const unsigned diskFrames = 4;

	const unsigned iso_cY = 41;
	const unsigned iso_cX = 38;
	const unsigned isoFrames = 2;

	int mapSize = 80;
	int mapColumns = 10;
	int map[] = 
	{		
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1
	};

	//==============================================
	// ALLEGRO VARIABLES
	//==============================================
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer;
	
	//==============================================
	// ALLEGRO INIT FUNCTIONS
	//==============================================
	if(!al_init())
		return -1;											
	
	display = al_create_display(WIDTH, HEIGHT);				
	
	if(!display)
		return -1;											

	//==============================================
	// ADDON INSTALL
	//==============================================
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();	
	al_init_ttf_addon();
	al_init_primitives_addon();


	//==============================================
	// PROJECT INIT
	//==============================================
	
	bitmap = al_load_bitmap("sprites.png");

		al_convert_mask_to_alpha(bitmap,al_map_rgb(0,255,255));


	Sprites qbert(bitmap, 
		spriteHeightInBitmap[QBERT], qbertX, qbertY,
		10, 10,
		qbertFrames, 5,
		3, 10, 1, 0, 
		1, 200, 100);

	Sprites ball(bitmap, spriteHeightInBitmap[BALL], ballX, ballY,
		10, 10, 
		ballFrames, 7, 
		3, 3, 1, 0,
		1, 250, 150);

	//==============================================
	// TIMER INIT AND STARTUP
	//==============================================
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);

	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//==============================================
		// INPUT
		//==============================================
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				qbert.moveLeft();
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				qbert.moveRight();
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				qbert.moveUp();
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				qbert.moveDown();
				keys[DOWN] = true;
				break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			}
		}
		//==============================================
		// GAME UPDATE
		//==============================================
		else if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			render = true;

		}

		//==============================================
		// RENDER
		//==============================================
		if(render && al_is_event_queue_empty(event_queue))
		{
			render = false;
			Compute_iso_cube_placement(12,3);
			ball.Draw();
			ball.animationUpdate();
			qbert.Draw();
			qbert.animationUpdate();

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}	
	}

	//==============================================
	// DESTROY ALLEGRO OBJECTS
	//==============================================
	al_destroy_bitmap(bitmap);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						

	return 0;
}