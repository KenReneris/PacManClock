/*
 * Draw.cpp
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"


uint8    g_zbuffer[ PANEL_WIDTH * PANEL_HEIGHT / 8 ];


bool
CheckZBuffer( Point pt, bool outOfBounds )
{
    return CheckZBuffer( pt.x, pt.y, outOfBounds );
}


bool
CheckZBuffer( int16 x, int16 y, bool result )
{
    if ( (x >= 0) && (x <= PANEL_WIDTH) && (y >= 0) && (y <= PANEL_HEIGHT) )
    { 
        uint16 const    pixelIndex  = ( y * PANEL_WIDTH + x );
        uint8 const     byteIndex   = pixelIndex / 8;
        uint8 const     bit         = 1 << ( pixelIndex % 8 );

        result = !!( g_zbuffer[ byteIndex ] & bit );
    }

    return result;
}


void
ClearZBuffer()
{
    memset( g_zbuffer, 0, sizeof(g_zbuffer) );
}


