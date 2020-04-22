/*
 * Draw.h
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


enum DrawMode : uint8
{
    Draw_                   = 0x00,         // draw
    Draw_Z                  = 0x01,
    Draw_EarseCollisions    = 0x02,
    Draw_OnTop              = 0x04,         // draw even if hits Z
};


bool    CheckZBuffer( Point pt, bool outOfBounds = false  );
bool    CheckZBuffer( int16 x, int16 y, bool outOfBounds = false );
void    ClearZBuffer();

extern PxMATRIX         g_display;
extern uint8            g_zbuffer[];

