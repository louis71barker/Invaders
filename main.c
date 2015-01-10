/*
 * CopyRight© 2015 Louis Barker & Jon Macey.
 * This program was written by Louis Barker and Jon Macey of the NCCA.
 * This programs is intended to be a remake of the Space Invaders game.
 * This is an assignment for my Principles and Pratice of Programming unit at Bournemouth University.
 */



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



//this is a global variable that is used throughout several function to define if the bullet has been fired
static char bulletFired = false;
//this is a globall variable that i have used as a trigger in serveral function so that a certain peice of code is only run once or when i need to to run
static char flag = false;



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

///
/// \brief explosion - This function is used for the explosion of the invader when they collide with the bullet
/// \param ren  - This is what is used to store all of the data that needs to be rendered on the screen
/// \param tex - This is what is used to bring through the image that is imported so it can be accessed, used and rendered withing the game
/// \param explosions - This is what is used to bring in all the locational data and information regarding the explosion sprite
/// \param invaders - This is what is used to bring in all the locational data, information and stucture for the invaders.
///
void explosion(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect explosions, Invader invaders[ROWS][COLS])
{
    //this is a timer which is used to hold to explosion image on the screen for the set period of time
    static int frameTimer = 0;

    //this is the co-ordinates of the explosion image on the sprite sheet
    SDL_Rect Explim1;
    Explim1.x=345;
    Explim1.y=610;
    Explim1.w=125;
    Explim1.h=83;

    //these for loop are used to cycle through all the invaders so they all are able to explode
    for(int r=0; r<ROWS; ++r)
    {
      for(int c=0; c<COLS; ++c)
      {
        //this tests to see if any of the invaders have been collided with by the bullet which turn the bool 'boom' to true.
        if (invaders[r][c].boom == true)
        {
          //this sets the x positon of the explosion to the x of the invader that has been hit by the bullet
          explosions.x=invaders[r][c].pos.x;
          //this sets the y positon of the explosion to the y of the invader that has been hit by the bullet
          explosions.y=invaders[r][c].pos.y;
          //this is what renders the explosion onto the screen
          SDL_RenderCopy(ren, tex, &Explim1, &explosions);
          //this starts the frame timer for how long the explosion will be rendered onto the screen
          frameTimer++;
          //this is what tests to see how long the invader has been on the screen for
          if (frameTimer == 10)
          {
              //this resets the frame timer to 0 so it can be re-used next time there is a collison
              frameTimer = 0;
              //this resets the boom variable back to false so that the next explosion timer is able to be detected
              invaders[r][c].boom = false;
          }
        }
      }
    }
}

/*
 * This Function is used to render the ship onto the screen using the coordinates given.
 */
///
/// \brief drawship
/// \param ren - This is what is used to store all of the data that needs to be rendered on the screen
/// \param tex - This is what is used to bring through the image that is imported so it can be accessed, used and rendered withing the game
/// \param ship - This is what passes in all the information to do with the ship so that it can be used and then displayed
///
void drawship(SDL_Renderer *ren, SDL_Texture *tex, SDL_Rect ship)
{
    //this is what is used to define the co-ordinated of the ship image on the sprite sheet
    SDL_Rect shipim;
    shipim.x=150;
    shipim.y=635;
    shipim.w=74;
    shipim.h=56;

    //this renders the ship onto the screen using the co-ordinates passed in and defined in this function
    SDL_RenderCopy(ren, tex, &shipim, &ship);
}
/*
 * This function is used to test the space bar to see if it has been pressed
 * once the space bar is pressed, it then triggers the bullet to render on the screen at the cordintes of the ship.
 */
///
/// \brief bulletTrigger
/// \param ren - This is what is used to store all of the data that needs to be rendered on the screen
/// \param tex - This is what is used to bring through the image that is imported so it can be accessed, used and rendered withing the game
/// \param bullet - This is what passes in all the information to do with the bullet so that it is able to be used to render onto the screen when needed
/// \param ship - This is what passes in all the information to do with the ship so that it can be used and then displayed
///
void bulletTrigger(SDL_Renderer *ren, SDL_Texture *tex,SDL_Rect *bullet, SDL_Rect *ship)
{
    //this is used to allow for key detection by checking for inputs from the keyboard
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    //this is the co-ordinates of the bullet image on the sprite sheet so it is able to be used to render onto the screen
    SDL_Rect bulletim;
    bulletim.x = 485;
    bulletim.y = 395;
    bulletim.w = 30;
    bulletim.h = 60;

    //this is were the keyboard is checked to see if the space bar has been pressed
    //it then also checks to see if the bool bulletfired is set to false so that it only allows for one bullet to be on the screen at any one time
    if (keystate[SDL_SCANCODE_SPACE] && bulletFired == false)
    {
        //this sets the bool to true so that only one bullet can be used at once
        bulletFired = true;
        //this gives the bullet the x position of the ship plus 16 so it is able to spawn in the middle of the ship
        bullet->x = ship->x + 16;
    }
    //this is were it is checked to see if a bullet has been fired, if it has then it renders it onto the screen
    //it then also set the y value to decrease so that the bullet travels up the screen when fired
    if (bulletFired == true)
    {
        //this renders the bullet onto the screen
        SDL_RenderCopy(ren,tex,&bulletim,bullet);
        //this set the bullet to move up the screen
        bullet->y -=20;
    }
    //this is used to check if the bullet has gone off the top of the screen
    //if this is true then the bulletfired is turned off so another bullet can be fired and the y positon is set back to its starting value
    if (bullet->y <= 0)
    {
        //turns bullet fire to false so another bullet is able to be fired
        bulletFired = false;
        //resets the bullet y back to its start location so when it is next fired it is back at the ship
        bullet->y = HEIGHT-40;
    }
}

 /* This is used to move the player left and right accross the screen
 */
///
/// \param ship - This is used to pass in all the data and information regarding the ship so it is able to be moved around the screen
///
void updateShip (SDL_Rect *ship)
{
    //this is used to allow for key detection by checking for inputs from the keyboard
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    //this is a variable which is used determine if the right key has been pressed
    char moveRight = false;
    //this is a variable which is used determine if the left key has been pressed
    char moveLeft = false;

    //this is were the keyboard is checked to see if the left key has been pressed
    if (keystate[SDL_SCANCODE_LEFT])
    {
        //this turns the bool to true so that only when the key is pressed does the ship move
        moveLeft = true;
        //this checks so see if the invader is at the left edge of the screen
        if (ship->x <=10)
        {
            //this turns of the move left so that the invader is unable to move of the side of the screen
            moveLeft = false;
        }

    }
    //this is were the keyboard is checked to see if the right key has been pressed
    if (keystate[SDL_SCANCODE_RIGHT])
    {
        //this turns the bool to true so that only when the key is pressed does the ship move
        moveRight = true;

        //this checks so see if the invader is at the left edge of the screen
        if (ship->x >= 750)
        {
            //this turns of the move left so that the invader is unable to move of the side of the screen
            moveRight=false;
        }

    }
    //this tests to see if the ship is still able to move right
    if (moveRight == true)
    {
        //this moves the player to the left at a rate of 6 pixels
        ship->x += 6;
    }
    //this tests to see if the ship is still able to move left
    else if (moveLeft == true)
    {
        //this moves the player to the left at a rate of 6 pixels
        ship->x -= 6;
    }
}

/*
 * This function is used for the detection of the bullet with the invaders
 */
///
/// \brief bulletCollision
/// \param invaders - This is what is used to bring in all the information to do with the invaders so they are able to be detected
/// \param bullet - This is what is used to bring in all the information to do with the bullet so that it is able to collide with the invaders
///
void bulletCollision(Invader invaders[ROWS][COLS],SDL_Rect *bullet)
{
    //these for loop are used to cycle through all the invaders so they all are able to detected
    for(int r=0; r<5+2; r++)
    {
      for(int c=0; c<11+2; c++)
      {
        //this is what is used to detect if the bullet sprite collides with the invaders
        if(SDL_HasIntersection(&invaders[r][c].pos, bullet))
        {
            //this is used to test weither the invader has been hit already
            if (invaders[r][c].active == 1)
            {
                //this turns the invader that is hit off so that it does not render on the screen and so that it can not be hit again
                invaders[r][c].active=0;
                //this is what is used to say when to show the explosion aniamtion
                invaders[r][c].boom = true;
                //this turns the bulledfired to false so it is no longer rendered and so that it can be refired
                bulletFired = false;
                //this sets if so that the bullet is set back to the ship
                bullet->y = HEIGHT-40;
            }
        }
      }
    }
}
/*
 * This function is used to update the invaders
 * it is used to move the invaders left and right on the screen
 * it is used to detect if the invaders have hit either side of the screen and then make then go the other direction
 * it is used to detect if a coloum on the outside has been destroyed, if it has then that coloum is moved so that the active invaders are able to hit either side of the screen
 */
///
/// \brief updateInvaders
/// \param invaders - This is what is used to bring in all the information needed to do with the invaders
///
void updateInvaders(Invader invaders[ROWS][COLS])
{
  //this is used to help determine which direction the invader is travelling
  enum DIR{FWD,BWD};
  //this sets the invaders to start by moving forward(to the right)
  static int DIRECTION=FWD;
  int yinc=0;
  //this variables are used for the detection of when a coloumn has become empty to allow for it to move along and allow the coloumn to collapse
  static int s = 0;
  static int b = 10;
  static int i = 0;
  static int z = 0;

  //this first if statement is to detect if the left hand row of invaders is active
  if (!invaders[i][s].active)
  {
      //this sets it to count every time an invader is inactive in the left coloumn
      i++;
      //this checks to see if all the invaders in the left coloumn have been turned inactive
      if (i == 5)
      {
          //this moves the left coloum up so that the next active row of invaders is now on the side
          invaders[4][0].pos.x+=SPRITEWIDTH+GAP;
          //this is used to move the coloumn which is being check up to the next one so that it can be checked to see if it is active
          s ++;
          //this resets the row count to 0 so all the invaders in the next row have to be turned off as well
          i = 0;
      }
      //this is used to check if all the invaders have been destroyed
      if (s >= COLS)
          {
              //this set the coloumn count back to 0 so the game is able to restart
              s = 0;
              b = 10;
              //this restarts the game over again by re initializing the invader
              initializeInvaders(invaders);

          }
  }
  //this ssecond if statement is to detect if the right hand row of invaders is active
  if (!invaders[z][b].active)
   {
      //this sets it to count every time an invader is inactive in the right coloumn
      z++;
      //this checks to see if all the invaders in the right coloumn have been turned inactive
      if (z == 5)
          {
              //this moves the right coloum up so that the next active row of invaders is now on the side
              invaders[4][10].pos.x-=SPRITEWIDTH+GAP;
              //this is used to move the coloumn which is being check up to the next one so that it can be checked to see if it is active
              b--;
              //this resets the row count to 0 so all the invaders in the next row have to be turned off as well
              z = 0;
          }
      //this is used to check if all the invaders have been destroyed
      if (b <= -1)
      {
          //this set the coloumn count back to 0 so the game is able to restart
          b = 10;
          s = 0;
          //this restarts the game over again by re initializing the invader
          initializeInvaders(invaders);
      }

  }
  //this next set of if statements are used to detected weither the invaders have hit either side of the screen
  //this checks to see if the invaders have reached the right hand side of the screen
  if(invaders[4][COLS-1].pos.x>=(WIDTH-2*SPRITEWIDTH))
  {
    //this sets the direction of the invaders to go backwards once they have hit the right hand side of the screen
    DIRECTION=BWD;
    yinc=GAP;

  }
  //this checks to see if the invaders have hit the left hand side of the screen
  else if(invaders[4][0].pos.x<=SPRITEWIDTH)
  {
    //this sets the direction to go forwards once the invaders have hit the left hand side of the screen
    DIRECTION=FWD;
    yinc=GAP;

  }
  //these for loop are used so that all the invaders are able to be tested
  for(int r=0; r<ROWS; ++r)
  {
    for(int c=0; c<COLS; ++c)
    {
      //this is used so that when the invaders are going forward, they move to the right at 2 pixel
      if(DIRECTION==FWD)
      {
        invaders[r][c].pos.x+=2;
      }
      else
          //this is used because of the invaders are not going forward then they are going backwards and this makes them move backwards at 2 pixels
          invaders[r][c].pos.x-=2;
          invaders[r][c].pos.y+=yinc;
    }
  }
}

int main()
{
  Invader invaders[ROWS][COLS];
  initializeInvaders(invaders);

  //this is what is used to define the size and the location of the ship when it is rendered
  SDL_Rect ship;
  ship.w = 40;
  ship.x = WIDTH/2 - ship.w/2;
  ship.h = 20;
  ship.y = HEIGHT-40;

  //this is what is used to define the size and the location of the bullet when it is rendered
  SDL_Rect bullet;
  bullet.w = 10;
  bullet.x = WIDTH/2 - bullet.w/2;
  bullet.h = 20;
  bullet.y = HEIGHT-40;

  //this is what is used to define the size and the location of the explosion when it is rendered
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
  SDL_Surface *image;
//  // we are going to use the extension SDL_image library to load a png, documentation can be found here
//  // http://www.libsdl.org/projects/SDL_image/
  image=IMG_Load("invaders.gif");
  if(!image)
  {
    printf("IMG_Load: %s\n", IMG_GetError());
    return EXIT_FAILURE;
  }

//  // SDL texture converts the image to a texture suitable for SDL rendering  / blitting
//  // once we have the texture it will be store in hardware and we don't need the image data anymore

  SDL_Texture *tex = 0;
  tex = SDL_CreateTextureFromSurface(ren, image);
  // free the image
  SDL_FreeSurface(image);


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

  //this is were all the above function are called into the main so that they can be used
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);
  bulletTrigger(ren,tex,&bullet,&ship);
  updateInvaders(invaders);
  bulletCollision(invaders,&bullet);
  drawInvaders(ren,tex,invaders);
  drawship(ren,tex,ship);
  updateShip(&ship);
  explosion(ren,tex,explosions,invaders);
  // Up until now everything was drawn behind the scenes.
  // This will show the new, red contents of the window.
  SDL_RenderPresent(ren);
  }
  SDL_Quit();
  return 0;
}

