/*
 * Defender.cpp
 *  Defender animation
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */



#include "platform.h"
#include "Defender.h"


const uint8 PROGMEM k_speeder[] = {
    0, 0,0,0, 7, 0, 15, 6,
    0x00,0x00,0xff, 0x00,0xff,0x00, 0x80,0x80,0x80, 0xf1,0xbe,0x14, 0xff,0x00,0x00, 0xff,0x00,0xff, 0xff,0xff,0xff,
        0x00, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0,
        0x30, 0x33, 0x03, 0x00, 0x00, 0x00, 0x00, 0xf0,
        0x35, 0x33, 0x33, 0x00, 0x00, 0x00, 0x00, 0xf0,
        0x30, 0x66, 0x33, 0x33, 0x13, 0x41, 0x00, 0xf0,
        0x35, 0x66, 0x66, 0x33, 0x33, 0x33, 0x73, 0xf2,
        0x00, 0x66, 0x36, 0x27, 0x00, 0x00, 0x00, 0x00 };


const uint8 PROGMEM k_defUfo0[] = {
    2, 0,0,0, 4, 0, 11, 4,
    0x00,0x00,0x00, 0x00,0xff,0x00, 0xdc,0x82,0x16, 0xf1,0xdc,0x14,
        0x00, 0x22, 0x22, 0x22, 0x02, 0x00,
        0x20, 0x13, 0x31, 0x11, 0x23, 0x00,
        0x42, 0x14, 0x44, 0x41, 0x14, 0xf2,
        0x20, 0x22, 0x22, 0x22, 0x22, 0x00 };


const uint8 PROGMEM k_defUfo1[] = {
    2, 0,0,0, 4, 0, 11, 4,
    0x00,0x00,0x00, 0x00,0xff,0x00, 0xdc,0x82,0x16, 0xf1,0xdc,0x14,
        0x00, 0x22, 0x22, 0x22, 0x02, 0x00,
        0x20, 0x11, 0x13, 0x31, 0x21, 0x00,
        0x12, 0x44, 0x41, 0x14, 0x44, 0x02,
        0x20, 0x22, 0x22, 0x22, 0x22, 0x00 };


const uint8 PROGMEM k_defUfo2[] = {
    2, 0,0,0, 4, 0, 11, 4,
    0x00,0x00,0x00, 0x00,0xff,0x00, 0xdc,0x82,0x16, 0xf1,0xdc,0x14,
        0x00, 0x22, 0x22, 0x22, 0x02, 0x00,
        0x20, 0x31, 0x11, 0x13, 0x21, 0x00,
        0x42, 0x41, 0x14, 0x44, 0x41, 0x02,
        0x20, 0x22, 0x22, 0x22, 0x22, 0x00 };

const uint8 PROGMEM k_bullet[] = { 0, 4,0,0, 1, 0, 1, 1, 0xff,0xff,0xff,  0xff };

const uint8 PROGMEM k_terrain[] = { 0xff, 0xde, 0xee, 0xff, 0xfe, 0xff, 0xde, 0xcc, 0xcb, 0xed, 0xee, 0xcd, 0xdc, 0xfe, 0xff, 0xef, 0xee, 0xde, 0xcc, 0xbc, 0xba, 0xcb, 0xed, 0xfe, 0xef, 0xdd, 0xee, 0xff, 0xef, 0xcd, 0xed, 0xff, };


//////////////////////////////////////////////////////////////////////////////////////
//
//
//


Defender::Terrain::Terrain()
    : _terrain      ( scopy<uint8>( k_terrain, countof(k_terrain) ) )
    , _color        ( Color(64,0,0) )
    , _posx         ( -(PANEL_WIDTH - 1) )      // negative of _startx
    , _endx         ( uint16(-1) )
    , _speedMs      ( 200 )
    , _startx       ( PANEL_WIDTH-1 )

{
    _lastMove = g_ms16;
    _lastAccel = g_ms16;
}


void            
Defender::Terrain::End()
{
    if( _endx == uint16(-1) )
    {
        _endx = _posx + PANEL_WIDTH;
    }
}



void
Defender::Terrain::Draw()
{
    uint16      startx          = _posx + _startx;
    uint16      iendx           = startx + PANEL_WIDTH - _startx;
    uint16      endx            = MIN( iendx, _endx );
    uint8       startIndex      = startx % (2*countof(k_terrain));
    uint8       endIndex        = endx % (2*countof(k_terrain));
    Point       pos;

    pos.x = _startx;
    if ( endIndex < startIndex )
    { 
        for ( uint8 index=startIndex; index < 2*countof(k_terrain); ++index, ++pos.x )
        {
            pos.y = (_terrain[ index / 2 ] >> (index % 2 ? 4 : 0)) & 0xF;
            g_display.drawPixel( pos.x, pos.y, _color );
        }
        startIndex = 0;
    }

    for ( uint8 index=startIndex; index <= endIndex; ++index, ++pos.x )
    {
        pos.y = (_terrain[ index / 2 ] >> (index % 2 ? 4 : 0)) & 0xF;
        g_display.drawPixel( pos.x, pos.y, _color );
    }

    if ( pos.x < PANEL_WIDTH )
    {
        if ( pos.x < 2 )
        {
            g_animationManager.Done();
        }
        else
        {
            for ( ; pos.x < PANEL_WIDTH; ++pos.x )
            {
                pos.y = (pos.y < PANEL_HEIGHT) ? pos.y+1 : pos.y;
                g_display.drawPixel( pos.x, pos.y, _color );
            }
        }
    }

    if ( uint16(g_ms16 - _lastMove) > _speedMs )
    {
        _lastMove += _speedMs;
        _posx += 1;
        if ( _startx )
        {
            _startx -= 1;
            _lastAccel = g_ms16;
            g_clock.SetOffset( Point( _startx - PANEL_WIDTH, 0 ) );
        }
    }

    if ( uint16(g_ms16 - _lastAccel) > 10 )
    {
        _lastAccel += 10;
        _speedMs = _speedMs - (_speedMs - 20) / 10 - 1;
        _speedMs = MAX( _speedMs, 20 );
    }
}



//////////////////////////////////////////////////////////////////////////////////////
//
//
//

Defender::UfoFlyby::UfoFlyby()
{
    g_flipX = false;

    BitmapBase * ufo[] = 
        {
            snew<Bitmap<4>>( 25, k_defUfo0 ),
            snew<Bitmap<4>>( 25, k_defUfo1 ),
            snew<Bitmap<4>>( 25, k_defUfo2 )
        };      

    _ufo.InitSprite( ufo, countof( ufo ) );
}



IStoryboard::StartResult
Defender::UfoFlyby::Start()
{
    _ufo.Start( Point(PANEL_WIDTH*2, random(1,PANEL_HEIGHT-8)), Point(0,0) );
    _ufo.PickNewTarget();

    return Start_None;
}


void 
Defender::UfoFlyby::Loop()
{
    _ufo.Loop();
    if ( _ufo.SpriteMode() == SpriteBase::Mode::Disabled )
    {
        _terrain.End();
    }
}


void
Defender::UfoFlyby::DrawFirst()
{
    _terrain.Draw();
}


/////////////

Defender::Ufo::Ufo()
{
    _started = g_ms;
    _leftRight = random( 2 );

    BitmapBase *bullet[] =
    {
        snew<Bitmap<1>>( 8, k_bullet ),
    };

    for ( Sprite & sprite : _bullet )
    {
        sprite.InitSprite( bullet, countof(bullet) );
    };
}


void
Defender::Ufo::Loop()
{
}


void
Defender::Ufo::PickNewTarget()
{
    _leftRight ^= 1;
    if ( _leftRight )
    {
        _targetPos.x = random( 4 );
    }
    else
    {
        _targetPos.x = random( PANEL_WIDTH-11-4, PANEL_WIDTH-11 );
    }

    if ( random(2) )
    {
        _targetPos.y = random( 3 );
    }
    else
    {
        _targetPos.y = random( PANEL_HEIGHT-6-3, PANEL_HEIGHT-6 );
    }

    _dir.x = Sign( _targetPos.x - _pos.x );
    _moveYRate = random( 4 );

    if ( g_ms - _started > 4000 )
    {
        if ( _leftRight )
        {
            _targetPos.x -= PANEL_WIDTH;
        }
        else
        {
            _targetPos.x += PANEL_WIDTH;
        }
    }

    //Out( F("Target: %d,%d, pos %d,%d, dir %d,%d\n"), _targetPos.x, _targetPos.y, _pos.x, _pos.y, _dir.x, _dir.y );
}


void
Defender::Ufo::NextFrame()
{
    Sprite::NextFrame();

    _moveYDiv += 1;
    if ( _moveYDiv > _moveYRate )
    {
        _moveYDiv = 0;
        _pos.y += Sign( _targetPos.y - _pos.y );
    }

    if ( _pos.x == _targetPos.x )
    {
        _dir = Point();
        if ( !_delay )
        {
            _delay = g_poweredOnTime;
        }
        else if ( _delay != uint16(g_poweredOnTime) )
        {
            _delay = 0;
            PickNewTarget();
        }
    }
}



