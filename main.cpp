#include "sprites.h"
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>
#include <vector>
#include <cmath>
#include <Windows.h>
#include <time.h>
const int WIDTH = 800;
const int HEIGHT = 640;

unsigned long points = 0;
unsigned long rounds = 0;
unsigned remainingDisks = 2;

ALLEGRO_BITMAP* bitmap = NULL;
int y=0;
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

bool done = false;

struct pyramid{
	unsigned x;
	unsigned y;
	unsigned multiplier;
};

bool keys[] = {false, false, false, false};
enum KEYS{UP, DOWN, LEFT, RIGHT};
enum CHARACTERS{QBERT, SNAKE, EGG, DISK, ISO, BALL ,QBERTBACK ,QBERTFLIP ,QBERTBACKFLIP ,QBERTCURSES};

int h1 = 7;
int h3 = 26;
bool is_first_time = true;

double iso_cube_y (double row){
	return WIDTH/4 + (row-1)*h3;
}


void wait ( int seconds )
{
  clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}

void gameOver(ALLEGRO_FONT* font){
	done = true;
	std::stringstream ss;
	char buffer [33];
	itoa (points,buffer,10);
	ss << "Total Points: " << buffer;
	const std::string tmp = ss.str();
	
	al_draw_text(font, al_map_rgb(255,255,255), 400, 200,ALLEGRO_ALIGN_CENTRE, "Game over!");
	al_draw_text(font, al_map_rgb(255,0,0), 400, 300,ALLEGRO_ALIGN_CENTRE, tmp.c_str());
	al_draw_text(font, al_map_rgb(255,0,0), 400, 400,ALLEGRO_ALIGN_CENTRE, "Press ESCAPE to exit.");
	/*itoa (rounds,buffer,10);
	ss << "Total Rounds: " << buffer;
	tmp = ss.str();
	al_draw_text(font, al_map_rgb(255,0,0), 400, 330,ALLEGRO_ALIGN_CENTRE, tmp.c_str());*/
	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));
}

void clearMonsters(){

}

bool pyramid_colision(Sprites &qbert , Sprites &diskLeft, Sprites &diskRight, unsigned &blocksLeft, bool &done, std::vector<pyramid> &p)
{	
	//########################
	// bound width, height
	//########################
	unsigned counter = 0;
	unsigned q_x = qbert.getPositionX();
	unsigned q_y = qbert.getPositionY() + 36;
	
	bool onAir = true;
	
    int a=100;	
	while(counter < p.size())
	{	
		if(q_x == p[counter].x && q_y == p[counter].y)
		{
			if(p[counter].multiplier == 1){
				blocksLeft--;
				points += 25;	//color change
			}
			onAir = false;
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
	if( qbert.getAnimationStatus() == false && onAir == true )
	{	
		if(q_y != diskLeft.getPositionY() && q_y != diskRight.getPositionY())
		{    
			qbertfall=1;
		}
		else if(q_y == diskLeft.getPositionY() && q_x == diskLeft.getPositionX())
		{	
				
				if(diskLeft.getlives()!=0){
					remainingDisks--;
					diskLeft.zerolives();
				    diskLeft.setdraw(0);
					qbertfall=0; 
				}else{
					qbertfall=1; 
				}
				

			//gototorightdisk=1; 
			
		}
		else if(q_y == diskRight.getPositionY() && q_x == diskRight.getPositionX())
		{	

			if(diskRight.getlives()!=0){
					remainingDisks--;
					diskRight.zerolives();
					diskRight.setdraw(0);
					qbertfall=0; 
				}else{
					qbertfall=1; 
				}



			//gototorightdisk=1; 
			
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
	bool render = false;
	const float FPS = 60.0;

	unsigned key_pushed = 666;
	const unsigned spriteHeightInBitmap[] = {0, 50, 115, 150, 178, 213, 432, 482 ,532 ,302};
	
	unsigned number_of_rows= 7;
	unsigned blocksLeft = (number_of_rows * (number_of_rows + 1) ) / 2;

	const unsigned qbertY = 50;
	const unsigned qbertX = 37;
	const unsigned qbertFrames = 4;
	
	const unsigned snakeY = 50;
	const unsigned snakeX = 30;
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
	ALLEGRO_FONT *title = al_load_ttf_font("pirulen.ttf",80,0 );
	ALLEGRO_FONT *credits = al_load_ttf_font("pirulen.ttf",14,0 );

	ALLEGRO_SAMPLE *hop = al_load_sample("Hop.wav");
	ALLEGRO_SAMPLE *coilyHop = al_load_sample("Ahop.wav");
	ALLEGRO_SAMPLE *fall = al_load_sample("fall.wav");
	ALLEGRO_SAMPLE *coilyfall = al_load_sample("coilyfall.wav");
	ALLEGRO_SAMPLE *coilybites = al_load_sample("coilybites.wav");
	ALLEGRO_SAMPLE *curse = al_load_sample("curse.wav");
	ALLEGRO_SAMPLE *start = al_load_sample("start.wav");
	ALLEGRO_SAMPLE *disk = al_load_sample("disk.wav");
	al_reserve_samples(1);

	//==============================================
	// PROJECT INIT
	//==============================================
	
	bitmap = al_load_bitmap("sprites.png");

		al_convert_mask_to_alpha(bitmap,al_map_rgb(0,255,255));


	Sprites qbert(bitmap, 
		spriteHeightInBitmap[QBERT], qbertX, qbertY,
		8, 8,
		qbertFrames,5,
		1, 10, 1, 0, 
		1, 304, 145);
	
	Sprites PLAYERLOGO(bitmap, 
		605, 75, 25,
		8, 8,
		qbertFrames,5,
		1, 10, 1, 0, 
		1, 150, 155);

	Sprites CURSE_CLOUD(bitmap, 
		380, 130, 50,
		8, 8,
		qbertFrames,5,
		1, 10, 1, 0, 
		1, 150, 155);


	Sprites qbertlive1(bitmap, 
		270, 17, 25,
		8, 8,
		qbertFrames,5,
		1, 10, 1, 0, 
		1, 150, 175);

	Sprites qbertlive2(bitmap, 
		270, 17, 25,
		8, 8,
		qbertFrames,5,
		1, 10, 1, 0, 
		1, 170, 175);

	Sprites qbertlive3(bitmap, 
		270, 17, 25,
		8, 8,
		qbertFrames,5,
		1, 10, 1, 0, 
		1, 190, 175);

	Sprites snake(bitmap,
		spriteHeightInBitmap[SNAKE], 30, 50,
		8, 8,
		snakeFrames, 15,
		1, 10, 1, 0,
		1, 304, 145);

	Sprites ball(bitmap, 
		spriteHeightInBitmap[BALL], ballX, ballY,
		8, 8,
		ballFrames, 10, 
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
	int a=0;
	//==============================================
	// TIMER INIT AND STARTUP
	//==============================================
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);
	ALLEGRO_EVENT ev;

	al_draw_text(credits, al_map_rgb(0,255,255), 400, 100,ALLEGRO_ALIGN_CENTRE, "Victor Lecomte 3449");
	al_draw_text(credits, al_map_rgb(0,255,255), 400, 135,ALLEGRO_ALIGN_CENTRE, "Alexis Pavlakis 34xx");
	al_draw_text(credits, al_map_rgb(0,255,255), 400, 170,ALLEGRO_ALIGN_CENTRE, "George Zervas 3456");
	al_draw_text(font, al_map_rgb(255,0,0), 400, 320,ALLEGRO_ALIGN_CENTRE, "Press ENTER to start!");
	al_draw_text(title, al_map_rgb(255,255,255), 400, 220,ALLEGRO_ALIGN_CENTRE, "Q*bert");
	al_draw_text(credits, al_map_rgb(0,255,255), 400, 400,ALLEGRO_ALIGN_CENTRE, "University of Crete");
	al_draw_text(credits, al_map_rgb(0,255,255), 400, 450,ALLEGRO_ALIGN_CENTRE, "Department of Computer Science");
	al_draw_text(credits, al_map_rgb(0,255,255), 400, 500,ALLEGRO_ALIGN_CENTRE, "University of Crete Department of Computer Science");
	al_draw_text(credits, al_map_rgb(0,255,255), 400, 550,ALLEGRO_ALIGN_CENTRE, "CS-454. Development of Intelligent Interfaces and Games");
	al_draw_text(credits, al_map_rgb(0,255,255), 400, 600,ALLEGRO_ALIGN_CENTRE, "Term Project, Fall Semester 2017");

	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_play_sample(start, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
	while(true){    
		
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
			if(ev.keyboard.keycode == ALLEGRO_KEY_ENTER){
				break;
			}else if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
				al_destroy_bitmap(bitmap);
				al_destroy_timer(timer);
				al_destroy_event_queue(event_queue);
				al_destroy_display(display);						
				al_destroy_sample(hop);						
				al_destroy_sample(coilyHop);						
				al_destroy_sample(fall);						

				return 0;
			}
		}
	}

	while(!done)
	{    
		
		al_wait_for_event(event_queue, &ev);
		
		//==============================================
		// INPUT
		//==============================================
		
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if(qbert.getAnimationStatus())
				continue;
			al_play_sample(hop, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
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
					if (qbert.getPositionY() > 350){
						qbert.lose_live();
						al_play_sample(fall, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);	
						
					 if(qbert.getlives()==20){
						 qbertlive3.setIndex(10000);
					 }else if(qbert.getlives()==10){
						 qbertlive2.setIndex(10000);
					 }else if(qbert.getlives()<=0){
						 qbertlive1.setIndex(10000);
						 gameOver(font);
						 break;
					 }
						std::cout<<"lives"<<qbert.getlives()<<"\n";
			        }
					qbert.set_Fall(true);
					qbertfall=0;
				}
				
				
			}
			
			if(blocksLeft < ((number_of_rows * (number_of_rows + 1) ) / 2) -1)
			{   
				if(qbert.qbert_get_collision() == 0){
					ball.animationMove();
				}else if(play == 0 &&  qbert.get_losing_sprite()>50){
					 
					 
			//		std::cout<<"qbert lives:"<<a<<"\n";
				//	a--;
					 if(qbert.getlives()==20){
						 qbertlive3.setIndex(10000);
					 }
					 if(qbert.getlives()==10){
						 qbertlive2.setIndex(10000);
					 }
					 if(qbert.getlives()==0){
						 qbertlive1.setIndex(10000);
						 gameOver(font);
						 break;
					 }
					 ball.setIndex(10000);
					 ball.setpositionX(500);
					 ball.setpositionY(260);
					 qbert.reducelives();
					
				}

			}
		//	std::cout<<"qbert: "<<qbert.getPositionY()<<"   "<<qbert.getPositionX()<<"\n";
			if(blocksLeft < ((number_of_rows * (number_of_rows + 1) ) / 2) -3)
			{   
				if(egg.getPositionY() < 311)
				{
					egg.animationMove();
					qbert.qbertcollision(egg);
					snake.setpositionX(egg.getPositionX());
					snake.setpositionY(egg.getPositionY()-20);
			//		std::cout<<"snake: "<<snake.getPositionY()<<" "<<snake.getPositionX() <<"\n";

				}
				else
				{   
					if(qbert.qbert_get_collision() == 0){
			    		snake.animationSnake(qbert);
						if(qbert.qbertcollision(snake)){
							al_play_sample(coilybites, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						}
					}
					else if(play == 0 && qbert.get_losing_sprite()>50){
					  egg.setpositionX(304);
					  egg.setpositionY(165);
					  
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
			qbertlive1.Draw();
			qbertlive2.Draw();
			qbertlive3.Draw();
			PLAYERLOGO.Draw();
			qbert.Draw();
			Compute_iso_cube_placement(number_of_rows,3, pyramid_boxes, first_run);
			
			if(blocksLeft < ((number_of_rows * (number_of_rows + 1) ) / 2) -1)
			{		
				 if(qbert.qbert_get_collision() == 0){
					ball.Draw();
				 }
			}			
			if(blocksLeft < ((number_of_rows * (number_of_rows + 1) ) / 2) -3)
			{  
				if(egg.getPositionY() < 311)
				{
					  egg.Draw();
				}
				else
				{   
					if(qbert.qbert_get_collision() == 0){
					  snake.Draw();
					}

				}
				 
			}
			if(diskright.getdraw()==1){
				diskright.Draw();
			}

			if(diskleft.getdraw()==0){
				if(qbert.getPositionX()!=304 && qbert.getPositionY()!=144){
					qbert.set_Fall(false);
				}else{
					diskleft.setdraw(2);
					qbert.setpositionX(304);
					qbert.setpositionY(145);

					qbert.Draw();
					qbert.set_Fall(false);
				}
				
				if(diskleft.getPositionY()>175){
					al_play_sample(disk, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);	
					diskleft.diskmoveUp();
					qbert.diskmoveUp();
				    diskleft.Draw();
					
					qbert.Draw();
				}else{
					if(diskleft.getPositionX()<304){
					 diskleft.diskmoveRight();
					 qbert.diskmoveRight();
					 diskleft.Draw();
					 qbert.Draw();
					}
				}

				//diskleft.Draw();
			}
			if(diskleft.getdraw()==1){
				diskleft.Draw();
			}

			if(diskright.getdraw()==0){
				if(qbert.getPositionX()!=304 && qbert.getPositionY()!=144){
					qbert.set_Fall(false);
				}else{
					diskright.setdraw(2);
					qbert.setpositionX(304);
					qbert.setpositionY(145);

					qbert.Draw();
					qbert.set_Fall(false);
				}
				
				if(diskright.getPositionY()>175){	
					al_play_sample(disk, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);	
					diskright.diskmoveUp();
					qbert.diskmoveUp();
				    diskright.Draw();
					
					qbert.Draw();
				}else{
					if(diskright.getPositionX()>304){
					 diskright.diskmoveLeft();
					 qbert.diskmoveLeft();
					 diskright.Draw();
					 qbert.Draw();
					}
				}
			}

			if(qbert.getFallingStatus()!=1){
				
						
				qbert.Draw();
				if(qbert.qbert_get_collision()==1){
					qbert.setAnimationStatus(true);
					clearMonsters();
					CURSE_CLOUD.setpositionX(qbert.getPositionX()-15); 
					CURSE_CLOUD.setpositionY(qbert.getPositionY()-35); 
					CURSE_CLOUD.Draw();
					al_play_sample(curse, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
				}
			}
			
			if(key_pushed != 666){
			 
				qbert.playerAnimationUpdate(key_pushed);
			  
			}
			
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
			char buffer [33];
			itoa (points,buffer,10);
			al_draw_text(font, al_map_rgb(255,255,255), 400, 100,ALLEGRO_ALIGN_CENTRE, buffer);
			itoa (rounds,buffer,10);
			al_draw_text(font, al_map_rgb(255,255,255), 400, 120,ALLEGRO_ALIGN_CENTRE, buffer);
		}
	}
	//==============================================
	// WAIT TO EXIT AND DESTROY ALLEGRO OBJECTS
	//==============================================
	while(true){    
		
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
					al_destroy_bitmap(bitmap);
					al_destroy_timer(timer);
					al_destroy_event_queue(event_queue);
					al_destroy_display(display);						
					al_destroy_sample(hop);						
					al_destroy_sample(coilyHop);						
					al_destroy_sample(fall);						

					return 0;
				}
			}
	}
}