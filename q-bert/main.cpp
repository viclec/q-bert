#include "sprites.h"
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>
#include <vector>
#include <cmath>

const int WIDTH = 800;
const int HEIGHT = 640;

unsigned long points = 0;
unsigned remainingDisks = 2;

ALLEGRO_BITMAP* bitmap = NULL;

int qbertfall = 0;
int rightdisklives = 1;
int leftdisklives = 1;
int gototorightdisk = 0;
double qbertX = 36;
double qbertY = 48;

double tileSizeX = 38;
double tileSizeY = 48;

double cubeX=6;
double cubeY=180;

double cubeSizeX = 32;
double cubeSizeY = 38;

double diskX = 5;
double diskY = 150;

struct pyramid{
	unsigned x;
	unsigned y;
	unsigned multiplier;
};

bool keys[] = {false, false, false, false};
enum KEYS{UP, DOWN, LEFT, RIGHT};
enum CHARACTERS{QBERT, SNAKE, EGG, DISK, ISO, BALL ,QBERTBACK ,QBERTFLIP ,QBERTBACKFLIP};

int h1 = 7;
int h3 = 26;
bool is_first_time = true;

double iso_cube_y (double row){
	return WIDTH/4 + (row-1)*h3;
}

bool pyramid_colision(Sprites &qbert , Sprites &diskLeft, Sprites &diskRight, unsigned &blocksLeft, bool &done, std::vector<pyramid> &p)
{	
	//########################
	// bound width, height
	//########################
	unsigned counter = 0;
	unsigned q_x = qbert.getPositionX();
	unsigned q_y = qbert.getPositionY() + 36;
	
	bool flag = true;
	
	while(counter < p.size())
	{	
		if(q_x == p[counter].x && q_y == p[counter].y)
		{
			if(p[counter].multiplier == 1){
				blocksLeft--;
				points += 25;	//color change
			}
			flag = false;
			p[counter].multiplier = 7;
			break;
		}
		counter++;
	}

	if(blocksLeft == 0){
		points += 50 * remainingDisks;	//remaining disks bonus
		points += 1000;	//completing screen
		//std::cout << "kerdises wow\n";
		done = true;
	}
	if( qbert.getAnimationStatus() == false && flag == true)
	{	
		if(q_y != diskLeft.getPositionY())
		{
			qbertfall=1;
		}
		else if(q_y == diskLeft.getPositionY() && q_x == diskLeft.getPositionX())
		{	
				
				if(diskLeft.getlives()!=0){
					remainingDisks--;
					qbert.setpositionX(304) ;
					qbert.setpositionY(145) ;
					diskLeft.zerolives();
					diskLeft.setdraw(0);
					qbertfall=0; 
				}else{
					qbertfall=1; 
				}
				

			gototorightdisk=1; 
			
		}
		else if(q_y == diskRight.getPositionY() && q_x == diskRight.getPositionX())
		{	

			if(diskRight.getlives()!=0){
					remainingDisks--;
					qbert.setpositionX(304) ;
					qbert.setpositionY(145) ;
					diskRight.zerolives();
					diskRight.setdraw(0);
					qbertfall=0; 
				}else{
					qbertfall=1; 
				}



			gototorightdisk=1; 
			
		}
		
		
	}
	 return false;	
}

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
			al_draw_bitmap_region(bitmap, pyramid[index].multiplier * cubeX , cubeY, cubeSizeX, cubeSizeY, xc, yc , 0);	
		//	if(i==2){std::cout<<yc;}


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

	unsigned key_pushed = 666;
	const unsigned spriteHeightInBitmap[] = {0, 50, 115, 150, 178, 213, 432, 482 ,532};
	
	unsigned number_of_rows= 7;
	unsigned blocksLeft = (number_of_rows * (number_of_rows + 1) ) / 2;

	const unsigned qbertY = 50;
	const unsigned qbertX = 37;
	const unsigned qbertFrames = 4;
	
	const unsigned snakeY = 65;
	const unsigned snakeX = 40;
	const unsigned snakeFrames = 4;
	
	const unsigned ballY = 35;
	const unsigned ballX = 38;
	const unsigned ballFrames = 3;

	//const unsigned diskY = 28;
	//const unsigned diskX = 37;
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
	al_install_audio();
	al_init_image_addon();
	al_init_font_addon();	
	al_init_ttf_addon();
	al_init_acodec_addon();
	al_init_primitives_addon();

	ALLEGRO_FONT *font = al_load_ttf_font("pirulen.ttf",20,0 );
	ALLEGRO_SAMPLE *hop = al_load_sample("Hop.wav");
	ALLEGRO_SAMPLE *coilyHop = al_load_sample("Ahop.wav");
	ALLEGRO_SAMPLE *fall = al_load_sample("fall.wav");
	al_reserve_samples(1);

	//==============================================
	// PROJECT INIT
	//==============================================
	
	bitmap = al_load_bitmap("sprites.png");

		al_convert_mask_to_alpha(bitmap,al_map_rgb(0,255,255));


	Sprites qbert(bitmap, 
		spriteHeightInBitmap[QBERT], qbertX, qbertY,
		8, 8,
		qbertFrames, 5,
		1, 10, 1, 0, 
		1, 304, 145);
	
	Sprites snake(bitmap,
		spriteHeightInBitmap[SNAKE], snakeX, snakeY,
		8, 8,
		snakeFrames, 20,
		1, 10, 1, 0,
		1, 304, 145);

	Sprites ball(bitmap, 
		spriteHeightInBitmap[BALL], ballX, ballY,
		8, 8,
		ballFrames, 5, 
		1, 10, 1, 0,
		1, 304, 165);

	Sprites egg(bitmap, 
		spriteHeightInBitmap[EGG], ballX, ballY,
		8, 8,
		ballFrames, 5, 
		1, 10, 1, 0,
		1, 304, 165);

	Sprites diskright(bitmap
		,spriteHeightInBitmap[DISK],37,30
		,0,0
		,4,20
		,38,0,1,0
		,1,384,259);

	Sprites diskleft(bitmap
		,spriteHeightInBitmap[DISK],37,30
		,0,0
		,4,20
		,38,0,1,0
		,1,224,259);

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
			al_play_sample(hop, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			if(qbert.getAnimationStatus())
				continue;
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				qbert.setIndex(532);
				qbert.moveUpLeft();
				qbert.moveCurve();
				key_pushed = LEFT;
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				qbert.setIndex(0);
				qbert.moveDownRight();
				qbert.moveCurve();
				keys[RIGHT] = true;
				key_pushed = RIGHT;
				break;
			case ALLEGRO_KEY_UP:
				qbert.setIndex(432);
				qbert.moveUpRight();
				qbert.moveCurve();
				keys[UP] = true;
				key_pushed = UP;
				break;
			case ALLEGRO_KEY_DOWN:
				qbert.setIndex(482);
				qbert.moveDownLeft();
				qbert.moveCurve();
				keys[DOWN] = true;
				key_pushed = DOWN;
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
				pyramid_colision(qbert,diskleft,diskright,blocksLeft,done,pyramid_boxes);
				if(qbertfall==1){
					al_play_sample(fall, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					qbert.set_Fall(true);
					qbertfall=0;
				}
				
				
			}
			if(blocksLeft < ((number_of_rows * (number_of_rows + 1) ) / 2) -1)
			{   
				ball.animationMove();
			}
			
			if(blocksLeft < ((number_of_rows * (number_of_rows + 1) ) / 2) -3)
			{  
				if(egg.getPositionY() < 311)
				{
					egg.animationMove();
					qbert.qbertcollision(egg);
					snake.setpositionX(egg.getPositionX());
					snake.setpositionY(egg.getPositionY());
				}
				else
				{
					snake.animationSnake(qbert);
					if(qbert.qbertcollision(snake)){
						std::cout << "SNAKE";	
					}
				}
			}

			qbert.qbertcollision(ball);
			
			
		    diskright.animationUpdate();
			diskleft.animationUpdate();
			
			
		}

		//==============================================
		// RENDER
		//==============================================
		if(render && al_is_event_queue_empty(event_queue))
		{
			render = false;

			qbert.Draw();
			Compute_iso_cube_placement(number_of_rows,3, pyramid_boxes, first_run);
			
			if(blocksLeft < ((number_of_rows * (number_of_rows + 1) ) / 2) -1)
			{  
				 ball.Draw();
				
			}			
			if(blocksLeft < ((number_of_rows * (number_of_rows + 1) ) / 2) -3)
			{  
				if(egg.getPositionY() < 311)
				{
					egg.Draw();
				}
				else
				{
					snake.Draw();
				}
				 
			}
			if(diskright.getdraw()==1){
				diskright.Draw();
			}
			if(diskleft.getdraw()==1){
				diskleft.Draw();
			}

			if(qbert.getFallingStatus()!=1){
				qbert.Draw();
			
			}

			if(key_pushed != 666){
				qbert.playerAnimationUpdate(key_pushed);
			}
			
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
			char buffer [33];
			itoa (points,buffer,10);
			al_draw_text(font, al_map_rgb(255,255,255), 150, 100,ALLEGRO_ALIGN_CENTRE, buffer);
		}
	}
	//==============================================
	// DESTROY ALLEGRO OBJECTS
	//==============================================
	al_destroy_bitmap(bitmap);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						
	al_destroy_sample(hop);						
	al_destroy_sample(coilyHop);						
	al_destroy_sample(fall);						

	return 0;
}