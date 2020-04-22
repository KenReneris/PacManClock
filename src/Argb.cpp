/*
 * Argb.cpp
 *  Some color helpers
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"



uint16 
Color( CRGB rgb )
{
    return g_display.color565( rgb.r, rgb.g, rgb.b );
}


uint16
Color( uint8 r, uint8 g, uint8 b )
{
    return g_display.color565( r, g, b );
}


uint16
Color( uint8 a, uint8 r, uint8 g, uint8 b )
{
    uint8 r1 = scale8( r, a );
    uint8 g1 = scale8( g, a );
    uint8 b1 = scale8( b, a );

    return g_display.color565( r1, g1, b1 );
}

