#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "Invader.h"
#include "player.h"
#include <stdbool.h>
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
//here is the definations for the bools that will be used through out the code
#define false 0
#define true 1
//this is the limit that will be used to change how long each frame of the invader is on the screen for.
#define ANIMATIONTIMERLIMIT  50




/*This function is used to initalize the invaders onto the screen
 * it sets the positions aswell as assigning them all the attributes that are need throughout the game
 */
void initializeInvaders(Invader invaders[ROWS][COLS])
{
  SDL_Rect pos;
  //sets the width of the invaders box in the grid
  pos.w=SPRITEWIDTH;
  //sets the height of the invaders box in the grid
  pos.h=SPRITEHEIGHT;
  //this is the gad between all of the invaders
  int ypos=GAP;
  //Loops through all of the rows in the invader grid
  for(int r=0; r<ROWS; ++r)
  {
    int xpos=GAP;
    //this loops through all of the coloums withing the invaders grid.
    for(int c=0; c<COLS; ++c)
    {
      pos.x=xpos+SPRITEWIDTH;
      pos.y=ypos+SPRITEHEIGHT;
      xpos+=(GAP+SPRITEWIDTH);
      invaders[r][c].pos=pos;
      //this turns the invaders on so they are able to be rendered onto the screen and turned off then they are hit.
      invaders[r][c].active=1;
      //this is used to flicker between the two different frames of the invaders.
      invaders[r][c].frame=1;
      //this is used to show the explosion of the invaders when they are hit but the defenders bullet.
      invaders[r][c].boom=false;
      //this is used to set the different invaders to their different rows show it is possible to show the three different types of invaders.
      if(r==0)
          //this is for the top row
        invaders[r][c].type=TYPE1;
      else if(r==1 || r==2)
          //this is for the second and third row down.
        invaders[r][c].type=TYPE2;
      else
          //this is for the bottom two rows.
        invaders[r][c].type=TYPE3;

    }
    ypos+=(GAP+SPRITEHEIGHT);
  }
}

/*
 *This function is used for the renderind of the invaders on the screen
 *The three different levels of the Invader are defined here defining the co-ordinates on the sprite sheet
 *The width, height, x and y cordinates on the spirte sheet are defined here so only the required area of the sprite sheet is shown
 *In this function, it is were the animation of the sprites is done by changing the x pos of the invader sprite location.
 */
///
/// \brief drawInvaders - This function is used for the renderind of the invaders on the screen
/// \param ren - This is what is used to store all of the data that needs to be rendered on the screen
/// \param tex - This is what is used to bring through the image that is imported so it can be accessed, used and rendered withing the game
/// \param invaders - This is the structure for the invaders which contains all the variables need for the invaders.
///
void drawInvaders(SDL_Renderer *ren, SDL_Texture *tex, Invader invaders[ROWS][COLS])
{
  //this is the variable used to count the amount of frames have run between the transitions of the sprite animation
  static int animationframe = 0;
  //this variable is used so that all of the invaders are scanned through to allow for the animation to work correctly on the invaders
  static int rowCount = 0;
  //here is the location of the first invader image off of the sprite sheet
  SDL_Rect SrcR1;
  SrcR1.x=300;
  SrcR1.y=13;
  SrcR1.w=95;
  SrcR1.h=83;
  //here is the location of the second invader image off of the sprite sheet
  SDL_Rect SrcR2;
  SrcR2.x=18;
  SrcR2.y=13;
  SrcR2.w=120;
  SrcR2.h=83;
  //here is the location of the third invader image off of the sprite sheet
  SDL_Rect SrcR3;
  SrcR3.x=18;
  SrcR3.y=130;
  SrcR3.w=125;
  SrcR3.h=83;

  // this is the repitition to at 1 at every frame to allows for the defenders to have a delay between the different sprite images allowing them to animate
  animationframe++;

//these for loop are used for the animation of the invaders
  for(int r=0; r<ROWS; ++r)
  {
    for(int c=0; c<COLS; ++c)
    {
        //this tests to see if the current frame iteration has reached it limit meaning that the next image frame can be loaded
        if(animationframe == ANIMATIONTIMERLIMIT)
        {
          //here sets the current frame to 1 to allow for the animation to run through two different frames
          //it tests to see if the invader frame is 1 or 2 and if it is, set the invader frame to 1
          invaders[r][c].frame = (invaders[r][c].frame == 1 ? 2 : 1);
          //this is the sequencial addition for the row count so all 55 invaders are able to be animated
          rowCount++;
          //tests to see if all 55 images have been scanned
          if (rowCount == 55)
          {
              //if all images have been animated and tested then the frame count and the row count are set back to the start so that they can re run through the next frame
              animationframe = 0;
              rowCount       = 0;
          }
        }
        //This is used to test if the frame is one or two, depending on which frame is active,
        //the x value for the location on the sprite sheet is changes to show the second or first frame image
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
      //here it tests to see if the invader is active to see what invaders need to be displayed on the screen
      if (invaders[r][c].active == 1)
      {

          //here it tests to see what invader image needs to be rendered depending the on Type of the invader
          switch(invaders[r][c].type)
          {
              //this is the top row
              case TYPE1 : SDL_RenderCopy(ren, tex,&SrcR1,&invaders[r][c].pos);break;
              //this is the second and third row
              case TYPE2 : SDL_RenderCopy(ren, tex,&SrcR2,&invaders[r][c].pos);break;
              //this is the forth and fifth row
              case TYPE3 : SDL_RenderCopy(ren, tex,&SrcR3,&invaders[r][c].pos);break;
      }

      }
    }
  }
}
/*
 *
 */
///
/// \brief explosion - This function is used for the explosion of the invader when they collide with the bullet
/// \param ren  - This is what is used to store all of the data that needs to be rendered on the screen
/// \param tex - This is what is used to bring through the image that is imported so it can be accessed, used and rendered withing the game
/// \param explosions
/// \param invaders
///
void explosion(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect explosions, Invader invaders[ROWS][COLS])
{
    static int frameTimer = 0;

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
          explosions.x=invaders[r][c].pos.x;
          explosions.y=invaders[r][c].pos.y;
          SDL_RenderCopy(ren, tex, &Explim1, &explosions);
          frameTimer++;
          printf("%d \n", frameTimer);
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

/*
 * This Function is used to render the ship onto the screen using the coordinates given.
 */
///
/// \brief drawship
/// \param ren - This is what is used to store all of the data that needs to be rendered on the screen
/// \param tex - This is what is used to bring through the image that is imported so it can be accessed, used and rendered withing the game
/// \param ship
///
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



///
/// \brief bulletTrigger
/// \param ren - This is what is used to store all of the data that needs to be rendered on the screen
/// \param tex - This is what is used to bring through the image that is imported so it can be accessed, used and rendered withing the game
/// \param bullet
/// \param ship
///
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
        SDL_RenderCopy(ren,tex,&bulletim,bullet);
    }
}


///
/// \brief bulletFire
/// \param bullet
///
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
}





 /* This is used to move the player left and right accross the screen
 */
///
/// \brief updateShip
/// \param ship
///
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
        ship->x += 6;
    }
    else if (moveLeft == true)
    {
        ship->x -= 6;
    }
}


///
/// \brief bulletCollision
/// \param invaders
/// \param bullet
///
void bulletCollision(Invader invaders[ROWS][COLS],SDL_Rect *bullet)
{
    for(int r=0; r<5+2; r++)
    {
      for(int c=0; c<11+2; c++)
      {

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
      }
    }
}

///
/// \brief updateInvaders
/// \param invaders
///
void updateInvaders(Invader invaders[ROWS][COLS])
{
  enum DIR{FWD,BWD};
  static int DIRECTION=FWD;
  int yinc=0;
  static int s = 0;
  static int b = 10;
  static int i = 0;
  static int z = 0;

  if (!invaders[i][s].active)
  {
      i++;
      if (i == 5)
      {
          invaders[4][0].pos.x+=SPRITEWIDTH+GAP;
          s += 1;
          printf("gap trigger\n");
          i = 0;
      }
      if (s >= COLS)
          {

              s = 0;
          }
  }
  if (!invaders[z][b].active)
   {
      z++;
      if (z == 5)
          {
              invaders[4][10].pos.x-=SPRITEWIDTH+GAP;
              b--;
              printf("gap trigger\n");
              z = 0;
          }
      if (b <= 0)
      {

          b = 10;

      }

  }
  if(invaders[4][COLS-1].pos.x>=(WIDTH-2*SPRITEWIDTH))
  {

    DIRECTION=BWD;
    yinc=GAP;

  }
  else if(invaders[4][0].pos.x<=SPRITEWIDTH)
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

int main()
{
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


