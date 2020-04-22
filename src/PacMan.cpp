
/*
 * PacMan.cpp
 *  PacMan animations
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"
#include "PacMan.h"

const uint8 PROGMEM k_leftPac0[]      = { 2, 0,0,0, 1, 0, 13,13, 0xff,0xff,0x00,  0xf0, 0x01, 0xfc, 0x07, 0xfe, 0x0f, 0xfe, 0x0f, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1f, 0xff, 0x1f, 0xfe, 0x0f, 0xfe, 0x0f, 0xfc, 0x07, 0xf0, 0x01 };
const uint8 PROGMEM k_leftPac1[]      = { 2, 0,0,0, 1, 0, 13,13, 0xff,0xff,0x00,  0xf0, 0x01, 0xfc, 0x07, 0xfe, 0x0f, 0xfe, 0x0f, 0xf8, 0x1f, 0xc0, 0x1f, 0x00, 0x1f, 0xc0, 0x1f, 0xf8, 0x1f, 0xfe, 0x0f, 0xfe, 0x0f, 0xfc, 0x07, 0xf0, 0x01 };
const uint8 PROGMEM k_leftPac2[]      = { 2, 0,0,0, 1, 0, 13,13, 0xff,0xff,0x00,  0xf0, 0x01, 0xf0, 0x07, 0xe0, 0x0f, 0xc0, 0x0f, 0x80, 0x1f, 0x00, 0x1f, 0x00, 0x1e, 0x00, 0x1f, 0x80, 0x1f, 0xc0, 0x0f, 0xe0, 0x0f, 0xf0, 0x07, 0xf0, 0x01 };
                                          
const uint8 PROGMEM k_blueGhost0[]    = { 2, 0,0,0, 2, 0, 14,14, 0x00,0x00,0xff, 0xff,0xff,0x00,                  0x00, 0x54, 0x01, 0x00, 0x40, 0x55, 0x15, 0x00, 0x50, 0x55, 0x55, 0x00, 0x54, 0x55, 0x55, 0x01, 0x54, 0x55, 0x55, 0x01, 0x54, 0x5a, 0x5a, 0x01, 0x55, 0x5a, 0x5a, 0x05, 0x55, 0x55, 0x55, 0x05, 0x55, 0x55, 0x55, 0x05, 0xa5, 0xa5, 0xa5, 0x05, 0x59, 0x5a, 0x5a, 0x06, 0x55, 0x55, 0x55, 0x05, 0x55, 0x54, 0x51, 0x05, 0x14, 0x50, 0x40, 0x01 };
const uint8 PROGMEM k_blueGhost1[]    = { 2, 0,0,0, 2, 0, 14,14, 0x00,0x00,0xff, 0xff,0xff,0x00,                  0x00, 0x54, 0x01, 0x00, 0x40, 0x55, 0x15, 0x00, 0x50, 0x55, 0x55, 0x00, 0x54, 0x55, 0x55, 0x01, 0x54, 0x55, 0x55, 0x01, 0x54, 0x5a, 0x5a, 0x01, 0x55, 0x5a, 0x5a, 0x05, 0x55, 0x55, 0x55, 0x05, 0x55, 0x55, 0x55, 0x05, 0xa5, 0xa5, 0xa5, 0x05, 0x59, 0x5a, 0x5a, 0x06, 0x55, 0x55, 0x55, 0x05, 0x45, 0x05, 0x15, 0x05, 0x01, 0x05, 0x05, 0x04 };
const uint8 PROGMEM k_ghost0[]        = { 2, 0,0,0, 3, 0, 14,14, 0x00,0xff,0xff, 0x00,0x00,0xff, 0xff,0xff,0xff,  0x00, 0x54, 0x01, 0x00, 0x40, 0x55, 0x15, 0x00, 0x50, 0x55, 0x55, 0x00, 0x54, 0x7d, 0xf5, 0x01, 0x54, 0xff, 0xfd, 0x03, 0x54, 0xaf, 0xbd, 0x06, 0x55, 0xaf, 0xbd, 0x06, 0x55, 0x7d, 0xf5, 0x05, 0x55, 0x55, 0x55, 0x05, 0x55, 0x55, 0x55, 0x05, 0x55, 0x55, 0x55, 0x05, 0x55, 0x55, 0x55, 0x05, 0x55, 0x54, 0x51, 0x05, 0x14, 0x50, 0x40, 0x01 };
const uint8 PROGMEM k_ghost1[]        = { 2, 0,0,0, 3, 0, 14,14, 0x00,0xff,0xff, 0x00,0x00,0xff, 0xff,0xff,0xff,  0x00, 0x54, 0x01, 0x00, 0x40, 0x55, 0x15, 0x00, 0x50, 0x55, 0x55, 0x00, 0x54, 0x7d, 0xf5, 0x01, 0x54, 0xff, 0xfd, 0x03, 0x54, 0xaf, 0xbd, 0x06, 0x55, 0xaf, 0xbd, 0x06, 0x55, 0x7d, 0xf5, 0x05, 0x55, 0x55, 0x55, 0x05, 0x55, 0x55, 0x55, 0x05, 0x55, 0x55, 0x55, 0x05, 0x55, 0x55, 0x55, 0x05, 0x45, 0x05, 0x15, 0x05, 0x01, 0x05, 0x05, 0x04 };
                                         
const uint8 PROGMEM k_ghostEyes[]     = { 9, 0,0,0, 3, 2, 14, 7, 0x00,0x00,0x00, 0x00,0x00,0xff, 0xff,0xff,0xff,  0x00, 0x14, 0x50, 0x00, 0x00, 0x7d, 0xf4, 0x01, 0x40, 0xff, 0xfd, 0x03, 0x40, 0xaf, 0xbd, 0x06, 0x40, 0xaf, 0xbd, 0x06, 0x00, 0x7d, 0xf4, 0x01, 0x00, 0x14, 0x50, 0x00 };

const uint8 PROGMEM k_pacManCaught0[] = { 2, 1,0,0, 1, 2, 13,11, 0xff,0xff,0x00,  0x02, 0x08, 0x06, 0x0c, 0x0f, 0x1e, 0x1f, 0x1f, 0xbf, 0x1f, 0xff, 0x1f, 0xff, 0x1f, 0xfe, 0x0f, 0xfe, 0x0f, 0xfc, 0x07, 0xf0, 0x01 };
const uint8 PROGMEM k_pacManCaught1[] = { 2, 1,0,0, 1, 4, 13, 9, 0xff,0xff,0x00,  0x03, 0x18, 0x0f, 0x1e, 0xbf, 0x1f, 0xff, 0x1f, 0xff, 0x1f, 0xfe, 0x0f, 0xfe, 0x0f, 0xfc, 0x07, 0xb0, 0x01 };
const uint8 PROGMEM k_pacManCaught2[] = { 2, 1,0,0, 1, 6, 13, 7, 0xff,0xff,0x00,  0x0f, 0x1e, 0xff, 0x1f, 0xff, 0x1f, 0xfe, 0x0f, 0xfe, 0x0f, 0xfc, 0x07, 0xb0, 0x01 };
const uint8 PROGMEM k_pacManCaught3[] = { 2, 1,0,0, 1, 7, 13, 6, 0xff,0xff,0x00,  0xf8, 0x03, 0xff, 0x1f, 0xfe, 0x0f, 0xfe, 0x0f, 0xfc, 0x07, 0xb0, 0x01 };
const uint8 PROGMEM k_pacManCaught4[] = { 2, 1,0,0, 1, 7, 13, 6, 0xff,0xff,0x00,  0xe0, 0x00, 0xf8, 0x03, 0xfe, 0x0f, 0xfe, 0x0f, 0xfc, 0x07, 0xb0, 0x01 };
const uint8 PROGMEM k_pacManCaught5[] = { 2, 1,0,0, 1, 7, 13, 6, 0xff,0xff,0x00,  0x40, 0x00, 0xe0, 0x00, 0xf8, 0x03, 0xfc, 0x07, 0xbc, 0x07, 0x10, 0x01 };
const uint8 PROGMEM k_pacManCaught6[] = { 2, 1,0,0, 1, 7, 13, 6, 0xff,0xff,0x00,  0x40, 0x00, 0xe0, 0x00, 0xf0, 0x01, 0xf8, 0x03, 0xbc, 0x07, 0xb0, 0x01 };
const uint8 PROGMEM k_pacManCaught7[] = { 2, 1,0,0, 1, 7, 13, 6, 0xff,0xff,0x00,  0x40, 0x00, 0x40, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xb0, 0x01, 0xb0, 0x01 };
const uint8 PROGMEM k_pacManCaught8[] = { 2, 1,0,0, 1, 7,  7, 6, 0xff,0xff,0x00,  0x40, 0x40, 0x40, 0x40, 0x00, 0x00 };
const uint8 PROGMEM k_pacManCaught9[] = { 4, 5,0,0, 1, 0,  1, 1, 0xff,0xff,0x00,  0x00 };
const uint8 PROGMEM k_pacManCaught10[]= { 4, 7,0,0, 2, 0, 13,12,  0xff,0xff,0x00, 0x00,0x00,0x00,  0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x10, 0x00, 0x00, 0x01, 0x04, 0x00, 0x04, 0x00, 0x40, 0x00, 0x50, 0x20, 0x14, 0x00, 0x00, 0x88, 0x00, 0x00, 0x05, 0x22, 0x42, 0x01, 0x00, 0x88, 0x00, 0x00, 0x50, 0x20, 0x14, 0x00, 0x04, 0x00, 0x40, 0x00, 0x00, 0x01, 0x01, 0x00, 0x40, 0x00, 0x04, 0x00 };


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//


PacMan::ChasePacMan::ChasePacMan()
{
    uint16      ghostColor[ 4 ] =
                {
                    Color( 0xFF, 0x00, 0x00 ),                  // blinky
                    Color( 0xFF, 0x48, 0xFF ),                  // pinky
                    Color( 0x10, 0x80, 0xFF ),                  // inky
                    Color( 0xFF, 0x5A, 0x10 )                   // clyde
                };

    BitmapBase *pacMan[ 3 ] =
                {
                    snew<Bitmap<1>>( 30, k_leftPac2, true ),
                    snew<Bitmap<1>>( 30, k_leftPac1, true ),
                    snew<Bitmap<1>>( 30, k_leftPac0 )           // round
                };

    BitmapBase *ghosts[ 2 ] =
                {
                    snew<Bitmap<2>>( 25, k_ghost0 ),
                    snew<Bitmap<2>>( 25, k_ghost1 )
                };
    
    _pacMan.InitSprite( pacMan, countof(pacMan) );
    for ( uint8 index=0; index < countof(_ghost); ++index )
    {
        uint16  * pallet    = scopy<uint16>( ghosts[0]->Pallet(), 3 );
    
        pallet[ 0 ] = ghostColor[ index ];
        _ghost[ index ].InitSprite( ghosts, countof(ghosts), pallet );
    }
}


IStoryboard::StartResult
PacMan::ChasePacMan::Start()
{
    Point       dir = Point( 1, 0 );

    _pacMan.    Start( Point( -14, 1 ),         dir );
    _ghost[ 0 ].Start( Point( -14-27-19*0, 1 ), dir );
    _ghost[ 1 ].Start( Point( -14-27-19*1, 1 ), dir );
    _ghost[ 2 ].Start( Point( -14-27-19*2, 1 ), dir );
    _ghost[ 3 ].Start( Point( -14-27-19*3, 1 ), dir );

    return Start_DimClock;
}


void
PacMan::ChasePacMan::Loop()
{
    if ( _ghost[ 3 ].SpriteMode() == SpriteBase::Mode::Disabled )
    {
        switch( g_animationManager.Active() )
        {
        case Animation::ChasePacMan:
            g_animationManager.ContinueTo( Animation::PacManChaseGhost, 1100 );
            break;

        case Animation::ChasePacManCatchGhost:
            g_animationManager.ContinueTo( Animation::PacManCatchGhost, 1100 );
            break;

        default:
            Out( F("ChasePacMan::Loop: unknown animation") );
            g_animationManager.Done();
            break;
        }
    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//


PacMan::ChaseGhost::ChaseGhost()
{
    BitmapBase  *pacMan[ 3 ] =
                {
                    snew<Bitmap<1>>( 27, k_leftPac0 ),
                    snew<Bitmap<1>>( 27, k_leftPac1 ),
                    snew<Bitmap<1>>( 27, k_leftPac2 )
                };

    BitmapBase  *ghosts[ 2 ] =
                {
                    snew<Bitmap<2>>( 33, k_blueGhost0 ),
                    snew<Bitmap<2>>( 33, k_blueGhost1 )
                };

    _pacMan.InitSprite( pacMan, countof(pacMan) );
    for ( uint8 index=0; index < countof(_ghost); ++index )
    {
        _ghost[ index ].InitSprite( ghosts, countof(ghosts) );
    }
}


IStoryboard::StartResult
PacMan::ChaseGhost::Start()
{
    Point       dir = Point( -1, 0 );

    _ghost[ 3 ].Start( Point( 32+19*0, 1 ),    dir );
    _ghost[ 2 ].Start( Point( 32+19*1, 1 ),    dir );
    _ghost[ 1 ].Start( Point( 32+19*2, 1 ),    dir );
    _ghost[ 0 ].Start( Point( 32+19*3, 1 ),    dir );
    _pacMan    .Start( Point( 32+19*4+20, 1 ), dir );

    return Start_DimClock;
}


void
PacMan::ChaseGhost::Loop()
{
    if ( _pacMan.SpriteMode() == SpriteBase::Mode::Disabled )
    {
        g_animationManager.Done();
    }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//


PacMan::CatchPacMan::CatchPacMan()
    : _step     ( 0 )
{
    uint16      ghostColor[] =
                {
                    Color( 0xFF, 0x00, 0x00 ),                  // blinky
                    Color( 0xFF, 0x48, 0xFF ),                  // pinky
                    Color( 0x10, 0x80, 0xFF ),                  // inky
                    Color( 0xFF, 0x5A, 0x10 )                   // clyde
                };

    BitmapBase *ghosts[] =
                {
                    snew<Bitmap<2>>( 25, k_ghost0 ),
                    snew<Bitmap<2>>( 25, k_ghost1 )
                };


    BitmapBase *pacManRound = snew<Bitmap<1>>( 27, k_leftPac0 );

    BitmapBase *pacMan[] =
                {
                    snew<Bitmap<1>>( 27, k_leftPac2, true ),
                    snew<Bitmap<1>>( 27, k_leftPac1, true ),
                    pacManRound
                };

    BitmapBase *pacManCaught[] =
                {
                    pacManRound,
                    snew<Bitmap<1>>( 30, k_pacManCaught0 ),
                    snew<Bitmap<1>>( 30, k_pacManCaught1 ),
                    snew<Bitmap<1>>( 30, k_pacManCaught2 ),
                    snew<Bitmap<1>>( 30, k_pacManCaught3 ),
                    snew<Bitmap<1>>( 30, k_pacManCaught4 ),
                    snew<Bitmap<1>>( 30, k_pacManCaught5 ),
                    snew<Bitmap<1>>( 30, k_pacManCaught6 ),
                    snew<Bitmap<1>>( 30, k_pacManCaught7 ),
                    snew<Bitmap<1>>( 30, k_pacManCaught8 ),
                    snew<Bitmap<1>>( 30, k_pacManCaught9 ),
                    snew<Bitmap<2>>( 30, k_pacManCaught10 ),
                };

    _pacMan.InitSprite( pacMan, countof(pacMan) );
    _pacManCaught.InitSprite( pacManCaught, countof(pacManCaught), nullptr, true );

    uint8 ghostColorsMax = countof( ghostColor );
    for ( uint8 index=0; index < countof(_ghost); ++index )
    {
        uint16    * pallet  = scopy<uint16>( ghosts[0]->Pallet(), 3 );
        uint8       who     = random( ghostColorsMax );

        pallet[ 0 ] = ghostColor[ who ];
        ghostColor[ who ] = ghostColor[ ghostColorsMax-1 ];
        ghostColorsMax -= 1;

        _ghost[ index ].InitSprite( ghosts, countof(ghosts), pallet );
    }
}


IStoryboard::StartResult
PacMan::CatchPacMan::Start()
{
    _ghost[ 0 ].Start( Point( -14-27-19*0, 1 ), Point( 1, 0 ) );
    _ghost[ 1 ].Start( Point(  32 + 28, 1 ),    Point(-1, 0 ) );        // try 28
    _pacMan    .Start( Point( -14, 1 ),         Point( 1, 0 ) );
    _pacManCaught.EnsureOnSpriteList();

    return Start_DimClock;
}


void
PacMan::CatchPacMan::Loop()
{
    switch ( _step )
    { 
    case 0:
        // if the blocking ghost has appeared, stop pacman
        if ( _ghost[ 1 ].SpriteMode() == SpriteBase::Mode::UntilDisappears )
        {
            _pacMan.Freeze();
            _step += 1;
        }
        break;

    case 1:
        // if a ghost has hit pacman, pause the animations for a bit
        if ( _pacMan.HadCollision() )
        {
            g_animationManager.FreezeAnimations( 750 );
            _step += 1;
        }
        break;

    case 2:
        // if frozen time over, start die-ing sprite
        _pacMan.SwitchTo( &_pacManCaught );
        _ghost[ 0 ].Disable();
        _ghost[ 1 ].Disable();
        _step += 1;
        break;

    case 3:
        if ( _pacManCaught.SpriteMode() == SpriteBase::Mode::Disabled )
        {
            g_animationManager.Done(); //  ContinueTo( Animation::PacManChaseGhost, 1100 );
        }
        break;
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//


PacMan::CatchGhost::CatchGhost()
{
    BitmapBase  *pacMan[] =
                {
                    snew<Bitmap<1>>( 27, k_leftPac0 ),
                    snew<Bitmap<1>>( 27, k_leftPac1 ),
                    snew<Bitmap<1>>( 27, k_leftPac2 )
                };

    BitmapBase  *ghosts[] =
                {
                    snew<Bitmap<2>>( 36, k_blueGhost0 ),
                    snew<Bitmap<2>>( 36, k_blueGhost1 )
                };

    BitmapBase  *ghostEyes[] =
            {
                snew<Bitmap<2>>( 16, k_ghostEyes )
            };

    _pacMan.InitSprite( pacMan, countof(pacMan) );
    for ( uint8 index=0; index < countof(_ghost); ++index )
    {
        _ghost[ index ].InitSprite( ghosts, countof(ghosts) );
        _ghostEyes[ index ].InitSprite( ghostEyes, countof(ghostEyes) );
    }
}


IStoryboard::StartResult
PacMan::CatchGhost::Start()
{
    Point       dir = Point( -1, 0 );

    for ( Sprite & ghostEye : _ghostEyes )
    {
        ghostEye.Start( Point( 50, 0 ), Point() );
    }

    _pacMan    .Start( Point( 32+19*4+25, 1 ), dir );
    _ghost[ 0 ].Start( Point( 32+19*0, 1 ),    dir );
    _ghost[ 1 ].Start( Point( 32+19*1, 1 ),    dir );
    _ghost[ 2 ].Start( Point( 32+19*2, 1 ),    dir );
    _ghost[ 3 ].Start( Point( 32+19*3, 1 ),    dir );

    return Start_DimClock;
}


void
PacMan::CatchGhost::Loop()
{
    auto startEyes =
        [&]( uint8 index )
        {
            _ghost[ index ].SwitchTo( &_ghostEyes[ index ] );
            _ghostEyes[ index ].SetDir( Point( 1, 0 ) );
            _start = g_ms16;
            _step += 1;
        };

    switch ( _step )
    {
    case 0:
        if ( _ghost[ 3 ].HadCollision() )
        {
            g_animationManager.FreezeAnimations( 750 );
            _step += 1;
        }
        break;

    case 1:
        startEyes( 3 );
        break;

    case 2:
        if ( uint16(g_ms16 - _start) > 1000 )
        {
            startEyes( 2 );
        }
        break;

    case 3:
        if ( uint16(g_ms16 - _start) > 1000 )
        {
            startEyes( 1 );
            g_clock.SetBrightness( 0xFF );
        }
        break;
        
    case 4:
        if ( uint16(g_ms16 - _start) > 2500 )
        {
            startEyes( 0 );
        }
        break;

    case 5:
        if ( _ghostEyes[0].SpriteMode() == SpriteBase::Mode::Disabled )
        {
            g_animationManager.Done();
        }
        break;
    }
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//


PacMan::GhostEyes::GhostEyes()
{
    BitmapBase  *ghostEyesLR[] =
            {
                snew<Bitmap<2>>( 16, k_ghostEyes )
            };

    BitmapBase  *ghostEyesRL[] =
            {
                snew<Bitmap<2>>( 16, k_ghostEyes, true )
            };

    _ghostEyes[ 0 ].InitSprite( ghostEyesLR, countof(ghostEyesLR) );
    _ghostEyes[ 1 ].InitSprite( ghostEyesRL, countof(ghostEyesRL) );
}


IStoryboard::StartResult
PacMan::GhostEyes::Start()
{
    _ghostEyes[ 0 ].Start( Point( -14, 1 ), Point(1, 0) );
    _ghostEyes[ 1 ].Start( Point( PANEL_WIDTH*4, 1 ), Point( -1, 0 ) );
    return Start_None;
}


void
PacMan::GhostEyes::Loop()
{
    if ( _ghostEyes[ 1 ].SpriteMode() == SpriteBase::Mode::Disabled )
    {
        g_animationManager.Done();
    }
}


