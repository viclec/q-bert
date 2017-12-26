#include "sprites.h"
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>
#include <vector>
#include <cmath>

const int WIDTH = 800;
const int HEIGHT = 640;

ALLEGRO_BITMAP* bitmap = NULL;

double qbertX = 36;
double qbertY = 48;

double tileSizeX = 38;
double tileSizeY = 48;

double cubeX=6;
double cubeY=180;

double cubeSizeX = 32;
double cubeSizeY = 38;

struct pyramid{
	unsigned x;
	unsigned y;
	unsigned multiplier;
};

bool keys[] = {false, false, false, false};
enum KEYS{UP, DOWN, LEFT, RIGHT};
enum CHARACTERS{QBERT, SNAKE, BALL, DISK, ISO};

int h1 = 7;
int h3 = 26;
bool is_first_time = true;

double iso_cube_y (double row){
	return WIDTH/4 + (row-1)*h3;
}

void pyramid_colision(Sprites qbert, std::vector<pyramid> &p)
{	
	//########################
	// bound width, height
	//########################
	unsigned counter = 0;
	unsigned q_x = qbert.getPositionX();
	unsigned q_y = qbert.getPositionY();
	unsigned q_bx = qbert.getFrameWidth();			
	unsigned q_by = qbert.getFrameHeight();
	int diff = HEIGHT;
	int position = -1;
	
	while(counter < p.size())
	{
		if(p[counter].x == q_x)
		{
			if(p[counter].y - q_y  <= 0){
				counter++;
				continue;
			}
			if(p[counter].y - q_y <= diff)
			{
				diff = (int)std::abs((int)(q_y - p[counter].y));
				position = counter;
			}
		}
		counter++;
	}
	if(position >= 0)
	{
		
		if(diff > 40)
			std::cout <<"\nexases";
		p[position].multiplier = 7;
	}
//	assert(position >= 0);
}

//otan pathsw sto cube pollaplasiazw to cubeX me 6.6 gia na alla3w xrwma
void Compute_iso_cube_placement(double row,double col, std::vector<pyramid> &pyramid, bool &first_run){
	double i;
	double j;
	int index = 0;
	struct pyramid p;
	for(i=0; i<row; i++){

		double y = iso_cube_y(i);
		double ys = y;
		double xs = HEIGHT/2 - i/2*cubeSizeX;

		for(j=0; j<=i; j++){
			double x = xs + (j-1)*cubeSizeX;
			double xc = x + cubeSizeX/2;
			double yc = y + h1;

			if(first_run){
				p.x = xc;
				p.y = yc;
				p.multiplier = 1;
				pyramid.push_back(p);
			}
			al_draw_bitmap_region(bitmap, pyramid[index].multiplier * cubeX , cubeY, cubeSizeX, cubeSizeY,xc, yc , 0);	
			index++;
		}
	}	
	
	first_run = false;
}

int main()
{	//==============================================
	// PROJECT VARIABLES
	//==============================================
	std::vector<pyramid> pyramid_boxes;
	bool first_run = true;
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
		1, 10, 1, 0, 
		1, 304, 145);

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
				qbert.moveUpLeft();
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				qbert.moveDownRight();
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				qbert.moveUpRight();
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				qbert.moveDownLeft();
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
			unsigned qx = qbert.getPositionX();
			unsigned qy = qbert.getPositionY();
			if(!first_run)
			{
				pyramid_colision(qbert,pyramid_boxes);
			}
			

		}

		//==============================================
		// RENDER
		//==============================================
		if(render && al_is_event_queue_empty(event_queue))
		{
			render = false;
			
			Compute_iso_cube_placement(7,3, pyramid_boxes, first_run);
			
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