#include <gb/gb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "Sprites/Box.h"
#include "Sprites/Player.h"
#include "Sprites/Plataform.h"
#include "Sprites/Wall.h"
#include "Sprites/DoorOpen.h"
#include "Sprites/DoorClose.h"



#define PLAYER_SPRITE_START 1
#define BOX_SPRITE_START 1
#define BOX_VRAM_INDEX 1
#define HOLE_VRAM_INDEX 2
#define WALL_VRAM_INDEX 3
#define OAM_PLAYER 0

uint8_t spriteX,spriteY;
uint8_t boxX = 20, boxY = 20;
int8_t velocityX,velocityY;

typedef struct Sprite{
    uint8_t x, y, width, height, id, vram;
} Sprite;

typedef struct Door{
	Sprite sprite;
    Sprite spriteOpen;
	Sprite spriteClose;
	bool open;
} Door;

typedef struct box{
    struct Sprite sprite;
} box;

typedef struct wall{
    Sprite sprite;
} wall;

typedef struct plataform{
    Sprite sprite;
} plataform;

typedef struct SWTR{
    Sprite sprite;
	bool complete;
	Door *door;
} SWTR;

typedef struct KYTR{
    Sprite sprite;
	bool activate;
} KYTR;


bool collides(struct Sprite sprite1, struct Sprite sprite2) {
    return sprite1.x < sprite2.x + sprite2.width &&
           sprite1.x + sprite1.width > sprite2.x &&
           sprite1.y < sprite2.y + sprite2.height &&
           sprite1.y + sprite1.height > sprite2.y;   

}
/*
void SetupPlayer(){

    set_sprite_tile(0,PLAYER_SPRITE_START);


    // Put thep layer at the bottom middle ofthescreen
    Player.x=80;
    Player.y=138;


}
*/

void UpdateInput(struct Sprite *Player)
{
	// Apply our velocity
		if(joypad() & J_UP)
		Player->y-=velocityY;
		if(joypad() & J_DOWN)
		Player->y+=velocityY;
		if(joypad() & J_LEFT)
		Player->x-=velocityX;
		if(joypad() & J_RIGHT)
		Player->x+=velocityX;
}

void MoveColliderPlayer(struct Sprite *sprite1, struct Sprite *sprite2, int *x, int *y)
{
				if(sprite1->x - *x !=0)
				{
					sprite2->x += sprite1->x - *x; // Modify sprite2.x using pointer
					move_sprite(sprite2->id, sprite2->x + 4, sprite2->y + 12);
				}
				if(sprite1->y - *y !=0)
				{
					sprite2->y += sprite1->y - *y; // Modify sprite2.y using pointer
					move_sprite(sprite2->id, sprite2->x + 4, sprite2->y + 12);
				}
				sprite1->x = *x;
				sprite1->y = *y;

}

void SetupLevel(struct box *to)
{
	struct box Box1 = {{20,20,8,8,1,BOX_VRAM_INDEX}};
	struct box Box2 = {{60,120,8,8,2,BOX_VRAM_INDEX}};
	struct box Box3 = {{40,100,8,8,3,BOX_VRAM_INDEX}};
	struct box Box4 = {{100,20,8,8,4,BOX_VRAM_INDEX}};
	struct box Box5 = {{130,90,8,8,5,BOX_VRAM_INDEX}};
	struct box Box6 = {{35,35,8,8,6,BOX_VRAM_INDEX}};
	

	*to++ = Box1;
	*to++ = Box2;
	*to++ = Box3;
	*to++ = Box4;
	*to++ = Box5;
	*to = Box6;
	
		
}

void main(void)
{
	DISPLAY_ON;
    SHOW_SPRITES;

	//Config Sprites
	set_sprite_data(1,BOX_SPRITE_START,Box);
	set_sprite_data(2,1,Plataform);
	set_sprite_data(3,1,Wall);
	set_sprite_data(4,1,DoorOpen);
	set_sprite_data(5,1,DoorClose);
	set_sprite_tile(7,2);
	//set_sprite_tile(8,3);
	//set_sprite_tile(2,4);
	for(uint8_t i=1;i<7;i++)
	set_sprite_tile(i,BOX_VRAM_INDEX);
	for(uint8_t i=8;i<17;i++)
	set_sprite_tile(i,WALL_VRAM_INDEX);
	struct Sprite wa1 = {100,100,8,8,8,WALL_VRAM_INDEX};
	struct Sprite wa2 = {90,140,8,8,9,WALL_VRAM_INDEX};
	struct Sprite wa3 = {90,60,8,8,10,WALL_VRAM_INDEX};
	struct Sprite wa4 = {150,40,8,8,11,WALL_VRAM_INDEX};
	move_sprite(wa1.id,wa1.x,wa1.y);
	move_sprite(wa2.id,wa2.x,wa2.y);
	move_sprite(wa3.id,wa3.x,wa3.y);
	move_sprite(wa4.id,wa4.x,wa4.y);

	//config ObjectsInitial
	struct Sprite Player = {0 , 0 , 8, 8,0,0};
	struct plataform plat = {{50,50,8,8,7,HOLE_VRAM_INDEX}};
	struct box Box[6];
	//struct Sprite Sp[6];
	SetupLevel(Box);
	for(uint8_t i=0;i<6;i++)
		move_sprite(Box[i].sprite.id,Box[i].sprite.x+4,Box[i].sprite.y+12);
    // Set our default velocity to be moving down and to the right
    velocityX=1;
    velocityY=1;

	set_sprite_data(0,PLAYER_SPRITE_START,PlayerSprite);
    set_sprite_tile(0,0);
	Player.x = 80; Player.y = 72;
    velocityX=1;
    velocityY=1;
	move_sprite(Player.id,Player.x,Player.y);
	
	
	move_sprite(plat.sprite.id,plat.sprite.x+4,plat.sprite.y+12);

    // Loop forever
    while(1) {


		// Game main loop processing goes here
		int prevSpriteY = Player.y;

		int prevSpriteX = Player.x;


		UpdateInput(&Player);


		for(uint8_t i=0;i<6;i++)
		if(collides(Player,Box[i].sprite))
		{
				MoveColliderPlayer(&Player,&Box[i].sprite,&prevSpriteX,&prevSpriteY);
		}
		
		if(collides(Player,plat.sprite))
		{
				Player.x = prevSpriteX;
				Player.y = prevSpriteY;
		}
		
		
		for(uint8_t i=0;i<6;i++)
		if(collides(plat.sprite,Box[i].sprite))
		{
			Box[i].sprite.x = -10;
			Box[i].sprite.y = -10;
			move_sprite(Box[i].sprite.id,Box[i].sprite.x+4,Box[i].sprite.y+12);
		}


		// When we get too far to the right while moving to the right
        if((Player.x>156)){

            
            // Clamp our x position back down to 156 so we don't go past the edge
            Player.x=156;
        }

        // When we get too far down on the screen while moving downards
        if((Player.y>140)){

           
            // Clamp our y position back down to 140 so we don't go past the edge
            Player.y=140;
        }

        // When we get too far to the left while moving left
        if((Player.x<4)){

            
            // Clamp our x position back up to 4 so we don't go past the edge
            Player.x=4;
        }

        // When we get too far towards the top of the screen while moving upwards
        if((Player.y<4)){

            // Clamp our y position back up to 4 so we don't go past the edge
            Player.y=4;
        }

        // Position the first sprite at our spriteX and spriteY
        // All sprites are render 8 pixels to the left of their x position and 16 pixels ABOVE their actual y position
        // This means an object rendered at 0,0 will not be visible
        // x+5 and y+12 will center the 8x8 tile at our x and y position
        move_sprite(OAM_PLAYER,Player.x+4,Player.y+12);
		
		// Done processing, yield CPU and wait for start of next frame
        
		wait_vbl_done();
		//vsync();
    }
}
