#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "Invader.h"
#include "player.h"
// include the map for the maze.
// the width of the screen taking into account the maze and block
#define WIDTH 800
// the height of the screen taking into account the maze and block
#define HEIGHT 600
// an enumeration for direction to move USE more enums!
//enum DIRECTION{UP,DOWN,LEFT,RIGHT,NONE};
//player coor
#define pwidth 400
#define pheight 550


void initializeInvaders(Invader invaders[ROWS][COLS]);

void updateInvaders(Invader invaders[ROWS][COLS]);

void drawInvaders(SDL_Renderer *ren,SDL_Texture *tex,Invader invaders[ROWS][COLS]);

void drawship(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect ship);

void updateShip(SDL_Rect ship);

//void moveShip(SDL_GetKeyboardState(NULL),drawship(shipim.x, shipim.y));

int main()
{
  //used for the keyboard input detection for moving the player


  Invader invaders[ROWS][COLS];
  initializeInvaders(invaders);

  SDL_Rect ship;
  ship.w = 40;
  ship.x = WIDTH/2 - ship.w/2;
  ship.h = 20;
  ship.y = HEIGHT-40;


  // initialise SDL and check that it worked otherwise exit
  // see here for details http://wiki.libsdl.org/CategoryAPI
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
  {
    printf("%s\n",SDL_GetError());
    return EXIT_FAILURE;
  }
  // we are now going to create an SDL window



  SDL_Window *win = 0;
  win = SDL_CreateWindow("Invaders", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (win == 0)
  {
    printf("%s\n",SDL_GetError());
    return EXIT_FAILURE;
  }
  // the renderer is the core element we need to draw, each call to SDL for drawing will need the
  // renderer pointer
  SDL_Renderer *ren = 0;
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  // check to see if this worked
  if (ren == 0)
  {
    printf("%s\n",SDL_GetError() );
    return EXIT_FAILURE;
  }
  // this will set the background colour to white.
  // however we will overdraw all of this so only for reference
  SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

//  // SDL image is an abstraction for all images
  SDL_Surface *image /*ship*/;
//  // we are going to use the extension SDL_image library to load a png, documentation can be found here
//  // http://www.libsdl.org/projects/SDL_image/
  image=IMG_Load("invaders.gif");
  if(!image)
  {
    printf("IMG_Load: %s\n", IMG_GetError());
    return EXIT_FAILURE;
  }
  /*ship=IMG_Load("ship.png");
  if (!ship)
  {
      printf ("IMG_LOAD: %s\n", IMG_GetError());
      return EXIT_FAILURE;
  }*/

//  // SDL texture converts the image to a texture suitable for SDL rendering  / blitting
//  // once we have the texture it will be store in hardware and we don't need the image data anymore

  SDL_Texture *tex = 0;
  tex = SDL_CreateTextureFromSurface(ren, image);
  // free the image
  SDL_FreeSurface(image);

 /* SDL_Texture *tex2 = 0;
  tex2 = SDL_CreateTextureFromSurface(ren, ship);
  SDL_FreeSurface(ship);
*/

  int quit=0;
  // now we are going to loop forever, process the keys then draw

  while (quit !=1)
  {
    SDL_Event event;
    // grab the SDL even (this will be keys etc)
    while (SDL_PollEvent(&event))
    {
      // look for the x of the window being clicked and exit
      if (event.type == SDL_QUIT)
        quit = 1;
      // check for a key down
      if (event.type == SDL_KEYDOWN)
      {
        switch (event.key.keysym.sym)
        {
            // if we have an escape quit
            case SDLK_ESCAPE : quit=1; break;

       }
    }
  }

  // now we clear the screen (will use the clear colour set previously)
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);
  updateInvaders(invaders);
  drawInvaders(ren,tex,invaders);
  drawship(ren,tex,ship);
  updateShip(ship);
  // Up until now everything was drawn behind the scenes.
  // This will show the new, red contents of the window.
  SDL_RenderPresent(ren);

  }

  SDL_Quit();
  return 0;


}


void initializeInvaders(Invader invaders[ROWS][COLS])
{
  SDL_Rect pos;
  pos.w=SPRITEWIDTH;
  pos.h=SPRITEHEIGHT;
  int ypos=GAP;

  for(int r=0; r<ROWS; ++r)
  {
    int xpos=GAP;
    for(int c=0; c<COLS; ++c)
    {
      pos.x=xpos+SPRITEWIDTH;
      pos.y=ypos+SPRITEHEIGHT;
      xpos+=(GAP+SPRITEWIDTH);
      invaders[r][c].pos=pos;
      invaders[r][c].active=1;
      invaders[r][c].frame=0;
      if(r==0)
        invaders[r][c].type=TYPE1;
      else if(r==1 || r==2)
        invaders[r][c].type=TYPE2;
      else
        invaders[r][c].type=TYPE3;

    }
    ypos+=(GAP+SPRITEHEIGHT);
  }
}

/*
 *This function is used for the renderind of the invaders on the screen
 *The code commented out shows the different colours for the different types of invaders
 */
void drawInvaders(SDL_Renderer *ren, SDL_Texture *tex, Invader invaders[ROWS][COLS])
{
  SDL_Rect SrcR;
  SrcR.x=18;
  SrcR.y=13;
  SrcR.w=112;
  SrcR.h=83;
  for(int r=0; r<ROWS; ++r)
  {
    for(int c=0; c<COLS; ++c)
    {
      /*switch(invaders[r][c].type)
      {
      case TYPE1 : SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); break;
      case TYPE2 : SDL_SetRenderDrawColor(ren, 0, 255, 0, 255); break;
      case TYPE3 : SDL_SetRenderDrawColor(ren, 0, 0, 255, 255); break;
      }
      SDL_RenderFillRect(ren,&invaders[r][c].pos);*/
      SDL_RenderCopy(ren, tex,&SrcR,&invaders[r][c].pos);


    }
  }
}
/*
 * This Function is used to render the ship onto the screen using the coordinates given.
 */
void drawship(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect ship)
{
    SDL_Rect shipim;
    shipim.x=150;
    shipim.y=635;
    shipim.w=74;
    shipim.h=56;




/*   int (ch);

    if(kbhit()) //checking to see if keyboard input pressed
    {
        ch=getch();
        if(ch==75)
        {
            shipim.x=-1;
            shipim.y=0;
        }

        if(ch==77)
        {
            shipim.x=1;
            shipim.y=0;
        }
    }


*/




    SDL_RenderFillRect(ren,&ship);
    SDL_RenderCopy(ren, tex, &shipim, &ship);
}




 /* This is used to move the player left and right accross the screen
 */

void updateShip (SDL_Rect ship)
    {
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        //for (int i = )

        if (keystate[SDL_SCANCODE_LEFT])
        {
            printf("Left Pressed. \n");
            ship.x +=-6;
        }
        if (keystate[SDL_SCANCODE_RIGHT])
        {
            printf("Right Pressed. \n");
            ship.x += 6;
        }
    }



void updateInvaders(Invader invaders[ROWS][COLS])
{
  enum DIR{FWD,BWD};
  static int DIRECTION=FWD;
  int yinc=0;
  if(invaders[0][COLS].pos.x>=(WIDTH-SPRITEWIDTH)-(COLS*(SPRITEWIDTH+GAP)))
  {
    DIRECTION=BWD;
    yinc=GAP;

  }
  else if(invaders[0][0].pos.x<=SPRITEWIDTH)
  {
    DIRECTION=FWD;
    yinc=GAP;

  }

  for(int r=0; r<ROWS; ++r)
  {
    for(int c=0; c<COLS; ++c)
    {
      if(DIRECTION==FWD)
        invaders[r][c].pos.x+=1;
      else
        invaders[r][c].pos.x-=1;
      invaders[r][c].pos.y+=yinc;

    }
  }
}
