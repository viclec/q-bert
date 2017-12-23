#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 640;

bool keys[] = {false, false, false, false};
enum KEYS{UP, DOWN, LEFT, RIGHT};

int main()
{	//==============================================
	// PROJECT VARIABLES
	//==============================================
	bool done = false;
	bool render = false;
	const float FPS = 60.0;

	
	int qbertY = 50;
	int qbertX = 37;
	
	int snakeY = 65;
	int snakeX = 41;
	
	int ballY = 35;
	int ballX = 38;

	int diskY = 28;
	int diskX = 37;

	int iso_cY = 41;
	int iso_cX = 38;

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
	ALLEGRO_BITMAP* bitmap = NULL;
	
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
	
	bitmap = al_load_bitmap("sprites.jpg");
	

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
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
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
			int height = 0 + qbertY + snakeY;
			int i = 0;

			al_draw_bitmap_region(bitmap,i*ballX,height,ballX,ballY,0,0,0);
			
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

