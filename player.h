#ifndef SHIP_H__
#define SHIP_H__

enum ShipType{SHIP1,SHIP2};

#define SPRITEWIDTH 30
#define SPRITEHEIGHT 30

typedef struct
{
  SDL_Rect pos;
  int active;
  int frame;
  enum ShipType type;
}Ship;


#endif
