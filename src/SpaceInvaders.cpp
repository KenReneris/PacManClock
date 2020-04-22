
/*
 * SpaceInvaders.cpp
 *  SpaceInvaders animations
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */



#include "platform.h"
#include "SpaceInvaders.h"

const uint8 PROGMEM k_invaderA1[]     = { 1, 4,0,0, 0, 0, 11,8, 0x04, 0x01, 0x89, 0x04, 0xfd, 0x05, 0x77, 0x07, 0xff, 0x07, 0xfe, 0x03, 0x04, 0x01, 0x02, 0x02 };
const uint8 PROGMEM k_invaderA2[]     = { 1, 4,0,0, 0, 0, 11,8, 0x04, 0x01, 0x88, 0x00, 0xfc, 0x01, 0x76, 0x03, 0xff, 0x07, 0xfd, 0x05, 0x05, 0x05, 0xd8, 0x00 };
const uint8 PROGMEM k_invaderB1[]     = { 1, 4,0,0, 0, 0,  8,8, 0x18, 0x3c, 0x7e, 0xdb, 0xff, 0x5a, 0x81, 0x42 };
const uint8 PROGMEM k_invaderB2[]     = { 1, 4,0,0, 0, 0,  8,8, 0x18, 0x3c, 0x7e, 0xdb, 0xff, 0x24, 0x5a, 0xa5 };

const uint8 PROGMEM k_invaderUfo[]    = { 1, 0,0,0, 0, 0, 17,7, 0xe0, 0x0f, 0x00, 0xf8, 0x3f, 0x00, 0xfc, 0x7f, 0x00, 0x6e, 0xdb, 0x00, 0xff, 0xff, 0x01, 0x1c, 0x73, 0x00, 0x08, 0x20, 0x00 };
                                                                                
const uint8 PROGMEM k_invaderShooter[]= { 1, 0,0,0, 0, 0, 11,5, 0x20, 0x00, 0x70, 0x00, 0xfe, 0x03, 0xff, 0x07, 0xff, 0x07 };
const uint8 PROGMEM k_shooterBullet[] = { 1, 0,0,0, 1, 0,  1,1, 0xFF,0x80,0x00,  0xFF };


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//

static
uint16 * 
SpaceInvaderColor()
{
    uint16      color = 0;

    switch( random(3) )
    {
    case 0:     color = Color( 0x20, 0xE0, 0x20 );      break;
    case 1:     color = Color( 0xEF, 0x5A, 0x10 );      break;
    case 2:                                       
    default:    color = Color( 0xA0, 0x20, 0xF0 );      break;
    }

    uint16  * pallet = salloc<uint16>( 1 );
    pallet[ 0 ] = color;
    return pallet;
}


SpaceInvaders::Invaders::Invaders()
{
    _speed = 80;
    BitmapBase * bms[ 2 ];
    
    if ( g_animationManager.Active() == Animation::SpaceInvaderA )
    {
        bms[ 0 ] = snew<Bitmap<1>>( _speed, k_invaderA1 );
        bms[ 1 ] = snew<Bitmap<1>>( _speed, k_invaderA2 );
    }
    else
    {
        bms[ 0 ] = snew<Bitmap<1>>( _speed, k_invaderB1 );
        bms[ 1 ] = snew<Bitmap<1>>( _speed, k_invaderB2 );
    }

    uint16* color = SpaceInvaderColor();
    for ( Sprite & invader : _invaders )
    {
        invader.InitSprite( bms, 2, scopy<uint16>(color, 1) );
    }

    _invaderCount = ( random( 5 ) >= 4 ? 2 : 1 );
}


IStoryboard::StartResult
SpaceInvaders::Invaders::Start()
{
    Point       pos;

    pos.y = -6;
    _dir.x = 1;

    for ( uint8 index=0; index < _invaderCount; ++index )
    {
        Sprite & invader = _invaders[ index ];

        invader.Start( pos, _dir );
        pos.x += invader.MaxWidth() + 1;
    }

    _lastPos0 = _invaders[ 0 ].GetPos();
    return Start_None;
}


void
SpaceInvaders::Invaders::NextFrame()
{
   if ( (_invaders[0].GetPos() != _lastPos0) && (_invaders[ 0 ].GetPos().y + 8 < PANEL_HEIGHT) )
   {
       if ( (_invaders[0].IsOnHorzEdge()) || (_invaders[1].IsOnHorzEdge()) )
        {
            //Out( "OnHorz: %d,%d\n", invader.GetPos().x, invader.GetPos().y );
            _dir.x *= -1;
            _speed = max( _speed - 7, 20 );

            for (uint8 index = 0; index < _invaderCount; ++index)
            {
                Sprite & invader = _invaders[ index ];

                invader.AdjustPos( Point( 0, 1 ) );
                invader.SetDir( _dir );
                invader.ForEachBitmap(
                    [this]( BitmapBase *bm )
                    {
                        bm->SetMovementSpeed( _speed );
                    } );
            }

            _lastPos0 = _invaders[ 0 ].GetPos();

            if ( _invaders[ 0 ].GetPos().y + 8 == Clock::k_DigitY )
            {
                g_clock.SetTargetOffset( Point( 0, 9 - Clock::k_DigitY ) );
            }
        }
   }
}


void
SpaceInvaders::Invaders::Loop()
{
    bool        enabled = false;

    for ( uint8 index = 0; index < _invaderCount; ++index )
    {
        enabled |= (_invaders[ 0 ].SpriteMode() != SpriteBase::Mode::Disabled);
    }

    if ( !enabled )
    {
        g_animationManager.Done();
    }
}


//////////////////////////////////////////////////////////////////////////////////////
//
//
//

SpaceInvaders::UfoFlyby::UfoFlyby()
{
    BitmapBase * ufo[] = 
        {
            snew<Bitmap<1>>( 50, k_invaderUfo )
        };

       
    _ufo.InitSprite( ufo, countof(ufo), SpaceInvaderColor() );
}



IStoryboard::StartResult
SpaceInvaders::UfoFlyby::Start()
{
    g_clock.EnsureOnSpriteList();       // draw first (so we appear behind the clock)
    _ufo.Start( Point(PANEL_WIDTH+1, random(1,PANEL_HEIGHT-8)), Point(-1,0) );
    return Start_None;
}


void        
SpaceInvaders::UfoFlyby::NextFrame()
{
    // put the clock in front
    g_clockDrawMode = Draw_Z;
}


void 
SpaceInvaders::UfoFlyby::Loop()
{
    if ( _ufo.SpriteMode() == SpriteBase::Mode::Disabled )
    {
        g_animationManager.Done();
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//


SpaceInvaders::Shooter::Shooter()
    : _start        ( g_ms )
    , _particles    ( 20, 3, CRGB( 0xFF, 0x00, 0x00 ), 3 )
    , _bullet       ( this )
{
    g_flipX = false;
    _runTime = random( 5000, 15000 );

    BitmapBase * shooter[] =
    {
        snew<Bitmap<1>>( 50, k_invaderShooter )
    };

    BitmapBase * bullet[] =
    {
        snew<Bitmap<1>>( 50, k_shooterBullet )
    };

    _shooter.SetDrawMode( Draw_ );
    _shooter.InitSprite( shooter, countof(shooter), SpaceInvaderColor() );
    _bullet.InitSprite( bullet, countof(bullet) );
}


IStoryboard::StartResult
SpaceInvaders::Shooter::Start()
{
    Point       shooterPos = Point( (random(2) ? -12 : PANEL_WIDTH), PANEL_HEIGHT - 5 );

    _bullet.EnsureOnSpriteList();
    _shooterPos = (PANEL_WIDTH / 2) - 11 / 2;
    _shooter.Start( shooterPos, Point(shooterPos.x < 0 ? 1 : -1, 0) );
    _shooterWait = 0;

    return Start_ClockToTop;
}


void         
SpaceInvaders::Shooter::Loop()
{
    if ( _shooter.SpriteMode() == SpriteBase::Mode::Disabled )
    {
        g_animationManager.Done();
    }
}


void
SpaceInvaders::Shooter::NextFrame()
{
    g_clockDrawMode = DrawMode( Draw_Z | Draw_EarseCollisions );

    _particles.NextFrame();

    if ( _shooter.GetPos().x == _shooterPos )
    {
        if ( (g_ms - _start < _runTime) || (!_shooter.GetDir().x) )
        {
            if ( !_shooterWait )
            {
                _shooterDelayStart = g_ms;
                _shooterWait = random( 0, 250 );
                _shooter.SetDir( Point() );
            }

            if ( g_ms - _shooterDelayStart > _shooterWait )
            {
                _shooterWait = 0;
                _shooterPos = random( PANEL_WIDTH - 11 );
                int16 dirX = Sign( _shooterPos - _shooter.GetPos().x );
                _shooter.SetDir( Point(dirX, 0) );
                _fireBullet = true;
            }
        }
    }

    // fire bullet?
    if ( (!_bullet.IsRunning()) && (_fireBullet) ) 
    {
        _bullet.Start( Point( _shooter.GetPos().x + 5, PANEL_HEIGHT - 4 ), Point( 0, -1 ) );
    }
}


void
SpaceInvaders::Shooter::DrawEnd()
{
    _particles.Draw();
}


/////////////////////////////////////////////

SpaceInvaders::Shooter::Bullet::Bullet( Shooter * parent )
    : _parent   ( parent )
{
}


void
SpaceInvaders::Shooter::Bullet::NextFrame()
{
    // call parent to apply our move
    Sprite::NextFrame();

    // called when it's time to move
    if ( _hit )
    {
        _hit -= 1;
        if ( !_hit )
        {
            Disable();
        }
    }
    else if ( CheckZBuffer(_pos) )
    {
        _hit = 2;
        _parent->_particles.Add( _pos );
    }
}

