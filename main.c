#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "Invader.h"
#include "player.h"
#include <stdbool.h>
#include <time.h>
#include <SDL_ttf.h>


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
#define false 0
#define true 1
#define ANIMATIONTIMERLIMIT  50





void initializeInvaders(Invader invaders[ROWS][COLS]);

void updateInvaders(Invader invaders[ROWS][COLS]);

void drawInvaders(SDL_Renderer *ren,SDL_Texture *tex,Invader invaders[ROWS][COLS]);

void drawship(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect ship);

void bulletTrigger(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect *bullet, SDL_Rect *ship);

void bulletCollision(Invader invaders[ROWS][COLS],SDL_Rect *bullet);

void bulletFire(SDL_Rect *bullet);

void updateShip (SDL_Rect *ship);

void explosion(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect explosions, Invader invaders[ROWS][COLS]);

void Levels();


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

  SDL_Rect bullet;
  bullet.w = 10;
  bullet.x = WIDTH/2 - bullet.w/2;
  bullet.h = 20;
  bullet.y = HEIGHT-40;

  SDL_Rect explosions;
  explosions.w = 40;
  explosions.x = WIDTH/2 - ship.w/2;
  explosions.h = 20;
  explosions.y = HEIGHT-240;


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
//    SDL_Texture *renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Render *renderer)
//    {

//    }
  }

  // now we clear the screen (will use the clear colour set previously)
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);
  bulletTrigger(ren,tex,&bullet,&ship);
  updateInvaders(invaders);
  bulletCollision(invaders,&bullet);
  drawInvaders(ren,tex,invaders);
  drawship(ren,tex,ship);
  bulletFire(&bullet);
  updateShip(&ship);
  Levels();
  explosion(ren,tex,explosions,invaders);
  // Up until now everything was drawn behind the scenes.
  // This will show the new, red contents of the window.
  SDL_RenderPresent(ren);

  }

  SDL_Quit();
  return 0;


}
//invader animation
//const int INVADER_FRAMES = 2;
//bool loadMedia()
//{

//}


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
      invaders[r][c].frame=1;
      invaders[r][c].boom=false;
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

  static int animationframe = 0;
  static int rowCount = 0;

  //frame 1
  SDL_Rect SrcR1;
  SrcR1.x=300;
  SrcR1.y=13;
  SrcR1.w=112;
  SrcR1.h=83;

  SDL_Rect SrcR2;
  SrcR2.x=18;
  SrcR2.y=13;
  SrcR2.w=112;
  SrcR2.h=83;

  SDL_Rect SrcR3;
  SrcR3.x=18;
  SrcR3.y=130;
  SrcR3.w=125;
  SrcR3.h=83;

//  if (animationframe == ANIMATIONTIMERLIMIT )
//  {
//      invaders[r][c].frame = 1
//  }
  animationframe++;


  for(int r=0; r<ROWS; ++r)
  {
    for(int c=0; c<COLS; ++c)
    {
        if(animationframe == ANIMATIONTIMERLIMIT)
        {
          invaders[r][c].frame = (invaders[r][c].frame == 1 ? 2 : 1);
          rowCount++;
          printf("SDAAAGE %d\n", invaders[r][c].frame);
          if (rowCount == 55)
          {
              animationframe = 0;
              rowCount       = 0;
          }
        }
        if (invaders[r][c].frame == 1)
        {
            SrcR1.x = 420;
            SrcR2.x = 158;
            SrcR3.x = 158;
        }
        else
        {
            SrcR1.x = 300;
            SrcR2.x = 18;
            SrcR3.x = 18;
        }
      if (invaders[r][c].active == 1)
      {

          switch(invaders[r][c].type)
          {
              case TYPE1 : SDL_RenderCopy(ren, tex,&SrcR1,&invaders[r][c].pos);break;
              case TYPE2 : SDL_RenderCopy(ren, tex,&SrcR2,&invaders[r][c].pos);break;
              case TYPE3 : SDL_RenderCopy(ren, tex,&SrcR3,&invaders[r][c].pos);break;
      }

      }

      //SDL_RenderFillRect(ren,&invaders[r][c].pos);
      //SDL_RenderCopy(ren, tex,&SrcR1,&invaders[r][c].pos);
      //SDL_RenderCopy(ren, tex,&SrcR2,&invaders[r][c].pos);
      //SDL_RenderCopy(ren, tex,&SrcR3,&invaders[r][c].pos);



    }
  }
}
/*
 * This Function is used to render the ship onto the screen using the coordinates given.
 */

//this is the function that will be used to print text onto the screen. it will contain the cordinates of were the text will be shown
void textLoader()
{
//    TTF_Font *text_font =  TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMonoBold.ttf", 36);

//      if (text_font == NULL) {
//        printf("Could not load font\n");
//        exit(1);
//      }
}

void explosion(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect explosions, Invader invaders[ROWS][COLS])
{
    static int frameTimer = 10;

    SDL_Rect Explim1;
    Explim1.x=345;
    Explim1.y=610;
    Explim1.w=125;
    Explim1.h=83;


    for(int r=0; r<ROWS; ++r)
    {
      for(int c=0; c<COLS; ++c)
      {
        if (invaders[r][c].boom == true)
        {
          Explim1.x=invaders[r][c].pos.x;
          Explim1.y=invaders[r][c].pos.y;
          SDL_RenderCopy(ren, tex, &Explim1, &explosions);
          frameTimer++;
          if (frameTimer == 10)
          {
              frameTimer = 0;
              invaders[r][c].boom = false;
          }
        }
      }
    }

    //SDL_RenderCopy(ren, tex, &Explim1, &explosions);
}

void drawship(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect ship)
{
    SDL_Rect shipim;
    shipim.x=150;
    shipim.y=635;
    shipim.w=74;
    shipim.h=56;


    SDL_RenderFillRect(ren,&ship);
    SDL_RenderCopy(ren, tex, &shipim, &ship);
}


static char bulletFired = false;
static char flag = false;




void bulletTrigger(SDL_Renderer *ren, SDL_Texture *tex,SDL_Rect *bullet, SDL_Rect *ship)
{
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    SDL_Rect bulletim;
    bulletim.x = 485;
    bulletim.y = 395;
    bulletim.w = 30;
    bulletim.h = 60;

    if (keystate[SDL_SCANCODE_SPACE] && bulletFired == false)
    {
        bulletFired = true;
        bullet->x = ship->x + 16;
    }
    if (bulletFired == true)
    {
        if (flag == false)
        {


            flag = true;
            printf("ship pos set");
        }

        //SDL_RenderFillRect(ren,bullet);
        SDL_RenderCopy(ren,tex,&bulletim,bullet);


    }

}



void bulletFire(SDL_Rect *bullet)
{
    if (bulletFired == true)
        bullet->y -=20;
    if (bullet->y <= 0)
    {
        bulletFired = false;
        bullet->y = HEIGHT-40;
        flag = false;

    }
//    if (bulletFired == false)
//        bullet->y = HEIGHT-40;

}





 /* This is used to move the player left and right accross the screen
 */

void updateShip (SDL_Rect *ship)
{
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    //for (int i = )
    char moveRight = false;
    char moveLeft = false;

    if (keystate[SDL_SCANCODE_LEFT])
    {
        moveLeft = true;
        if (ship->x <=10)
        {
            moveLeft = false;
        }

    }
    if (keystate[SDL_SCANCODE_RIGHT])
    {
        moveRight = true;

        if (ship->x >= 750)
        {
            moveRight=false;
        }

    }
    if (moveRight == true)
    {
        //printf("Right Pressed. \n");
        ship->x += 6;
    }
    else if (moveLeft == true)
    {
        //printf("Left Pressed. \n");
        ship->x -= 6;
    }
}

void bulletCollision(Invader invaders[ROWS][COLS],SDL_Rect *bullet)
{
//    static int collision = false;
//    if(invaders[0][COLS].pos.x>=bullet->x)
//    {
//      printf("collide");
//      invaders[r][c].active=0;

//    }
    for(int r=0; r<5+2; r++)
    {
      for(int c=0; c<11+2; c++)
      {
//          float bw = 10, bh = 20;
//          float bx = bullet->x, by = bullet->y;
//          float ix = invaders[r][c].pos.x, iy = invaders[r][c].pos.y, iw = invaders[r][c].pos.w, ih = invaders[r][c].pos.h;
//          int ileft = invaders[r][c].pos.x;
//          int iright = invaders[r][c].pos.x + invaders[r][c].pos.w;
//          int ibottom = invaders[r][c].pos.y + invaders[r][c].pos.h;

//          int bleft = bullet->x;
//          int bright = bullet->x + bullet->w;
//          int btop = bullet->y;
//           Check edges
//          if (ileft > bright )// Left 1 is right of right 2
//              return false; // No collision

//          if ( iright < bleft ) // Right 1 is left of left 2
//              return false; // No collision

//          if ( ibottom < btop ) // Bottom 1 is above top 2
//              return false; // No collision

//          else
//          {
//              printf("collision");
//                            invaders[r][c].active=0;
//                            bulletFired = false;
//                            bullet->y = HEIGHT-40;
//                            flag = false;
//          }

// best so far
//            if (bulletFired == true)
//            {
                //if (shipPos > invaders[r][c].pos.x && shipPos < (invaders[r][c].pos.x+invaders[r][c].pos.w) && bullet->y > invaders[r][c].pos.y && bullet->y < (invaders[r][c].pos.y+invaders[r][c].pos.h))
                if(SDL_HasIntersection(&invaders[r][c].pos, bullet))
                {
                    if (invaders[r][c].active == 1)
                    {
                        printf("collision\n");
                        invaders[r][c].active=0;
                        invaders[r][c].boom = true;
                        bulletFired = false;
                        bullet->y = HEIGHT-40;
                        flag = false;
                    }
                }
//            }
//            if(invaders[r][c].pos.y>=bullet->y && invaders[r][c].pos.x>=bullet->x && invaders[r][c].active==1)
//            {


//                    printf("collision\n");
//                    invaders[r][c].active=0;
//                    bulletFired = false;
//                    bullet->y = HEIGHT-40;
//                    flag = false;



//            }
//            else
//            {
//                c++;
//            }






//          if (ix > bx && ix < bx+bw && iy > by && iy < by+bh)
//          {
//              printf("collision");
//              invaders[r][c].active=0;
//              bulletFired = false;
//              bullet->y = HEIGHT-40;
//              flag = false;
//          }

      }
//      r++;
    }


}

static bool Winner = false;

void updateInvaders(Invader invaders[ROWS][COLS])
{
  enum DIR{FWD,BWD};
  static int DIRECTION=FWD;
//  int row [6]= {0,1,2,3,4,5};
  int yinc=0;
  static int s = 0;
  static int b = 10;
  static int i = 0;
  static int z = 0;

  if (!invaders[i][s].active && Winner == false)
  {
      i++;
      if (i == 5)
      {
          invaders[0][0].pos.x+=SPRITEWIDTH+GAP;
          s += 1;
          printf("gap trigger\n");
          i = 0;
      }
      if (s >= COLS && Winner == false)
          {
              Winner = true;
              s = 0;
          }
  }
  if (!invaders[z][b ].active && Winner == false)
   {
      z++;
      if (z == 5)
          {
              invaders[0][10].pos.x-=SPRITEWIDTH+GAP;
              b--;
              printf("gap trigger\n");
              z = 0;
          }
      if (b <= 0 && Winner == false)
      {
          Winner = true;
          b = 10;

      }

  }
  if(invaders[0][COLS-1].pos.x>=(WIDTH-2*SPRITEWIDTH))
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
      {

        invaders[r][c].pos.x+=2;
      }
      else
      invaders[r][c].pos.x-=2;
      invaders[r][c].pos.y+=yinc;

    }

  }
}

void Levels()
{
 static int flag2 = 0;
 if (Winner == true && flag2 ==0)
 {
     printf("Winner");
     flag2 = 1;
 }

}


