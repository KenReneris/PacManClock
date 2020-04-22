/*
 * Particles.cpp
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

#include "platform.h"


Particles::Particles( uint8 speed, uint8 maxCount, CRGB color, uint8 distance )
    : _maxDist      ( distance )
    , _speed        ( speed )
    , _maxParticles ( maxCount )
    , _inUse        ( 0 )
    , _colors       ( salloc<uint16>( distance ) )
    , _particles    ( salloc<Particle>( maxCount ) )
{
    uint8 alpha = 255;
    uint8 decay = alpha / distance;
    for( uint index=0; index < distance; ++index )
    {
        _colors[ index ] = Color( alpha, color.r, color.g, color.b );
        alpha -= (256 / decay);
    }
}


void 
Particles::Add( Point pt )
{
    Particle    * p = nullptr;

    if ( _inUse < _maxParticles )
    {
        p = _particles + _inUse;
        _inUse += 1;

        p->_x = pt.x;
        p->_y = pt.y;
        p->_dist = uint8(-1);
        p->_last = g_ms8;
    }
    else
    {
        Out( F("Particles full\n") );
    }
}


void
Particles::NextFrame()
{
    for ( uint8 index=0; index < _inUse; )
    {
        Particle    * p = _particles + index;

        if ( uint8(g_ms8 - p->_last) > _speed )
        {
            p->_dist += 1;
            p->_last = g_ms8;

            // remove it ?
            if ( p->_dist > _maxDist )
            {
                *p = _particles[ _inUse - 1 ];
                _inUse -= 1;
            }
            else
            {
                index += 1;
            }
        }
        else
        {
            index += 1;
        }
    }
}


void 
Particles::Draw()
{
    for ( uint8 index = 0; index < _inUse; ++index )
    {
        Particle      * p           = _particles + index;
        Point const     center      = Point( p->_x, p->_y );
        uint8 const     dist        = p->_dist;
        uint16          color       = _colors[ dist ];

        g_display.drawPixel( center.x - dist, center.y - dist, color );
        g_display.drawPixel( center.x + dist, center.y - dist, color );
        g_display.drawPixel( center.x + dist, center.y + dist, color );
        g_display.drawPixel( center.x - dist, center.y + dist, color );
    }
}



