#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>
#include "TileLabel.h"
#include "box.h"
#include <array.h>

#define PLAYER_SPRITE_START 1
#define OAM_PLAYER 0

uint8_t spriteX,spriteY;
uint8_t boxX = 20, boxY = 20;
int8_t velocityX,velocityY;

typedef struct {
    uint8_t x, y, width, height, id;
} Sprite;



bool collides(Sprite sprite1, Sprite sprite2) {
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

void UpdateInput(Sprite *Player)
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

void main(void)
{
	DISPLAY_ON;
    SHOW_SPRITES;

	auto Sprite Player = {0 , 0 , 4, 4,0};
	auto Sprite Sp1 = {20,20,4,4,1};
	auto Sprite Sp2 = {60,120,4,4,2};
	auto Sprite Sp3 = {40,100,4,4,3};
	auto Sprite Sp4 = {100,20,4,4,4};
	auto Sprite Sp5 = {130,90,4,4,5};


	
    set_sprite_data(1,1,box);
	set_sprite_data(2,1,box);
	set_sprite_data(3,1,box);
	set_sprite_data(4,1,box);
	set_sprite_data(5,1,box);


	set_sprite_tile(1,1);
	set_sprite_tile(2,2);
	set_sprite_tile(3,3);
	set_sprite_tile(4,4);
	set_sprite_tile(5,5);
    
	
	
	set_sprite_data(0,1,TileLabel);
    set_sprite_tile(0,0);
	
	Player.x = 84;
	Player.y = 88;
	
    move_sprite(0,Player.x,Player.y);

	move_sprite(1,Sp1.x+4,Sp1.y+12);
	move_sprite(2,Sp2.x+4,Sp2.y+12);
	move_sprite(3,Sp3.x+4,Sp3.y+12);
	move_sprite(4,Sp4.x+4,Sp4.y+12);
	move_sprite(5,Sp5.x+4,Sp5.y+12);

    // Set our default position

	Player.x = 80;

	Player.y = 72;

	

    // Set our default velocity to be moving down and to the right
    velocityX=1;
    velocityY=1;
    // Loop forever
    while(1) {


		// Game main loop processing goes here
		uint8_t prevSpriteY = Player.y;
		uint8_t prevSpriteX = Player.x;

		UpdateInput(&Player);


		for(uint8_t i = 1; i<5;i++)
			shadow_OAM[i]
		if(collides(Player,Sp1))
		{
				Player.x = prevSpriteX;
				Player.y = prevSpriteY;
		}
		
		if(collides(Player,Sp2))
		{
				Player.x = prevSpriteX;
				Player.y = prevSpriteY;
		}
		
		if(collides(Player,Sp3))
		{
				Player.x = prevSpriteX;
				Player.y = prevSpriteY;
		}
		
		if(collides(Player,Sp4))
		{
				Player.x = prevSpriteX;
				Player.y = prevSpriteY;
		}
		
		if(collides(Player,Sp5))
		{
				Player.x = prevSpriteX;
				Player.y = prevSpriteY;
		}
/*
		if(boxX<spriteX+2 && boxX + 2> spriteX && boxY<spriteY-2 && boxY>spriteY+2)
				{
				spriteX = prevSpriteX;
				spriteY = prevSpriteY;
				}
				*/
		// When we get too far to the right while moving to the right
        if((Player.x>156)){

            // Switch directions for our x velocity
            //velocityX=-velocityX;

            // Clamp our x position back down to 156 so we don't go past the edge
            Player.x=156;
        }

        // When we get too far down on the screen while moving downards
        if((Player.y>140)){

            // Switch directions for our y velocity
            //velocityY=-velocityY;

            // Clamp our y position back down to 140 so we don't go past the edge
            Player.y=140;
        }

        // When we get too far to the left while moving left
        if((Player.x<4)){

            // Switch directions for our x velocity
            //velocityX=-velocityX;

            // Clamp our x position back up to 4 so we don't go past the edge
            Player.x=4;
        }

        // When we get too far towards the top of the screen while moving upwards
        if((Player.y<4)){

            // Switch directions for our y velocity
            //velocityY=-velocityY;

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
