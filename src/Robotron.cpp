/*
 * Robotron.cpp
 *  Roobotron animations
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"
#include "Robotron.h"


const uint8 PROGMEM k_robotronAttacker0[] = {
    1, 0,0,0, 5, 0, 9, 12,
    0x6e,0xff,0x00, 0xf3,0x00,0xff, 0xff,0x00,0x00, 0xff,0xff,0x00, 0xff,0xff,0xff,
        0x00, 0x30, 0x33, 0x00, 0x00,
        0x00, 0x11, 0x11, 0x01, 0x00,
        0x00, 0x22, 0x22, 0x02, 0x00,
        0x00, 0x30, 0x33, 0x00, 0x00,
        0x33, 0x35, 0x33, 0x35, 0x03,
        0x34, 0x53, 0x55, 0x33, 0x04,
        0x04, 0x33, 0x35, 0x03, 0x04,
        0x04, 0x30, 0x33, 0x00, 0x04,
        0x00, 0x30, 0x33, 0x00, 0x00,
        0x00, 0x33, 0x30, 0x03, 0x00,
        0x00, 0x33, 0x30, 0x03, 0x00,
        0x40, 0x44, 0x40, 0x44, 0x00 };

const uint8 PROGMEM k_robotronAttacker1[] = {
    1, 0,0,0, 5, 0, 9, 12,
    0x6e,0xff,0x00, 0xf3,0x00,0xff, 0xff,0x00,0x00, 0xff,0xf3,0x00, 0xff,0xff,0xff,
        0x00, 0x30, 0x33, 0x00, 0x00,
        0x00, 0x11, 0x11, 0x01, 0x00,
        0x00, 0x22, 0x22, 0x02, 0x00,
        0x00, 0x30, 0x33, 0x00, 0x00,
        0x33, 0x35, 0x33, 0x35, 0x03,
        0x34, 0x53, 0x55, 0x33, 0x04,
        0x04, 0x33, 0x35, 0x03, 0x04,
        0x04, 0x30, 0x33, 0x00, 0x04,
        0x00, 0x30, 0x33, 0x00, 0x00,
        0x00, 0x33, 0x30, 0x03, 0x00,
        0x40, 0x44, 0x30, 0x03, 0x00,
        0x00, 0x00, 0x40, 0x44, 0x00 };

const uint8 PROGMEM k_robotronLady0[] = {
    0, 0,0,0, 5, 0, 6, 13,
    0x00,0xff,0x00, 0xc8,0x96,0x64, 0xff,0x00,0xff, 0xff,0xff,0x00, 0xff,0xff,0xff,
        0x00, 0x44, 0x00,
        0x40, 0x24, 0x00,
        0x40, 0x12, 0x00,
        0x44, 0x22, 0x00,
        0x30, 0x03, 0x00,
        0x30, 0x32, 0x00,
        0x30, 0x02, 0x00,
        0x30, 0x35, 0x00,
        0x13, 0x11, 0x03,
        0x10, 0x11, 0x00,
        0x00, 0x05, 0x00,
        0x00, 0x05, 0x00,
        0x00, 0x33, 0x00 };

const uint8 PROGMEM k_robotronLady1[] = {
    0, 0,0,0, 5, 0, 6, 13,
    0x00,0xff,0x00, 0xc8,0x96,0x64, 0xff,0x00,0xff, 0xff,0xff,0x00, 0xff,0xff,0xff,
        0x00, 0x44, 0x00,
        0x40, 0x24, 0x00,
        0x40, 0x12, 0x00,
        0x44, 0x22, 0x00,
        0x30, 0x03, 0x00,
        0x32, 0x33, 0x00,
        0x32, 0x03, 0x00,
        0x35, 0x33, 0x05,
        0x11, 0x31, 0x03,
        0x11, 0x51, 0x00,
        0x50, 0x00, 0x05,
        0x50, 0x00, 0x05,
        0x30, 0x03, 0x33 };

const uint8 PROGMEM k_robotronLady2[] = {
    0, 0,0,0, 5, 0, 6, 13,
    0x00,0xff,0x00, 0xc8,0x96,0x64, 0xff,0x00,0xff, 0xff,0xff,0x00, 0xff,0xff,0xff,
        0x00, 0x44, 0x00,
        0x40, 0x24, 0x00,
        0x40, 0x12, 0x00,
        0x44, 0x22, 0x00,
        0x30, 0x03, 0x00,
        0x32, 0x33, 0x00,
        0x32, 0x03, 0x00,
        0x35, 0x33, 0x05,
        0x11, 0x31, 0x03,
        0x11, 0x51, 0x00,
        0x50, 0x00, 0x05,
        0x50, 0x00, 0x05,
        0x30, 0x03, 0x33 };

const uint8 PROGMEM k_robo4000[] = {
    0, 1,0,0, 0, 0, 10, 5,
        0x9d, 0xaa, 0x0a,
        0x9d, 0xbb, 0x0b,
        0x95, 0xbb, 0x0b,
        0x90, 0xbb, 0x0b,
        0x90, 0xaa, 0x0a };


const uint8 PROGMEM k_robotronShooter0[] = { 2, 0,0,0, 3, 0, 5,11, 0xff,0x00,0x00, 0xff,0xff,0xff, 0xff,0x00,0xff,  0x54, 0x01, 0x55, 0x01, 0x55, 0x01, 0x54, 0x00, 0x20, 0x00, 0xa8, 0x00, 0xb8, 0x00, 0xb8, 0x00, 0x30, 0x00, 0x20, 0x00, 0xa0, 0x00 };
const uint8 PROGMEM k_robotronShooter1[] = { 2, 0,0,0, 3, 0, 5,11, 0xff,0x00,0x00, 0xff,0xff,0xff, 0xff,0x00,0xff,  0x54, 0x01, 0x55, 0x01, 0x55, 0x01, 0x54, 0x00, 0x20, 0x00, 0x28, 0x00, 0xeb, 0x03, 0x2b, 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 0x02 };
const uint8 PROGMEM k_robotronShooter2[] = { 2, 0,0,0, 3, 0, 5,11, 0xff,0x00,0x00, 0xff,0xff,0xff, 0xff,0x00,0xff,  0x54, 0x01, 0x55, 0x01, 0x55, 0x01, 0x54, 0x00, 0x20, 0x00, 0x28, 0x00, 0xfb, 0x03, 0x2b, 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 0x02 };

const uint8 PROGMEM k_robotronBullet[]   = { 0, 4,0,0, 1, 0, 4, 1, 0xff,0xff,0xff,  0xff };


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//


Robotron::Chase::Chase()
    : _attackerMax  ( IsAttack() ? 5 : 7 )
{
    BitmapBase *shooter[] =
                {
                    snew<Bitmap<2>>( 40, k_robotronShooter0, true ),
                    snew<Bitmap<2>>( 40, k_robotronShooter1, true ),
                    snew<Bitmap<2>>( 40, k_robotronShooter2, true )
                };

    BitmapBase *attacker[] =
                {
                    snew<Bitmap<4>>( 40, k_robotronAttacker0 ),
                    snew<Bitmap<4>>( 40, k_robotronAttacker1 ),
                    snew<Bitmap<4>>( 40, k_robotronAttacker1, true )
                };

    _shooter.InitSprite( shooter, countof(shooter) );

    _attackers = salloc<Stomper>( _attackerMax );
    for ( uint32 index=0; index < _attackerMax; ++index )
    {
        Stomper * stomper = new( _attackers + index )Stomper();

        stomper->InitSprite( attacker, countof( attacker ) );
    }
}


bool 
Robotron::IsAttack()
{
    return ( g_animationManager.Active() == Animation::RobotronAttack );
}


IStoryboard::StartResult
Robotron::Chase::Start()
{
    bool const  isAttack    = IsAttack();
    int16 const stepMs      = 200;
    int16       lastMs      = g_ms16;
    Range       range( 0, 5 );
    Point       pos( PANEL_WIDTH, 0 );
    Point       dir( -1, 0 );

    if ( isAttack )
    {
        pos.x = -9;
        dir.x = 1;
        _shooter.Start( Point(PANEL_WIDTH+65, range.Random()), Point(-1,0) );
    }
    else
    {
        _shooter.Start( Point(PANEL_WIDTH, range.Random()), Point(-1,0) );
    }

    _attackerMoveYLast = g_ms16;
    for ( uint32 index=0; index < _attackerMax; ++index )
    {
        Stomper   & Stomper     = _attackers[ index ];

        pos.y = range.Random();

        Stomper.Start( pos, dir );
        Stomper.Init( &_shooter, lastMs );

        pos.x -= ( 11 * dir.x );
        if ( !isAttack )
        {
            range.min = -12*2;
            range.max = PANEL_HEIGHT+12;
        }

        lastMs -= stepMs;
    }

    return Start_DimClock;
}


void
Robotron::Chase::Loop()
{
    bool const  isAttack = IsAttack();

    // if attacking, don't start shooting until on the display
    if ( (isAttack) && (_shooter.SpriteMode() != SpriteBase::Mode::UntilDisappears) )
    {
        _shooter._lastFire = g_ms16;     // disable fire
    }

    _shooter.Loop();

    {
        uint16     speedMoveY = _shooter._stomperSpeed;

        speedMoveY = MIN( speedMoveY, 50 );
        if ( (uint16(g_ms16 - _attackerMoveYLast) > speedMoveY) )
        {
            _attackerMoveYLast += speedMoveY;

            _attackers[ _attackerMoveYIndex ].MoveY();
            _attackerMoveYIndex += 1;
            if ( _attackerMoveYIndex >= _attackerMax )
            {
                _attackerMoveYIndex = 0;
            }
        }
    }

    if ( isAttack )
    {
        if ( _shooter.IsPastWidth() )
        {
            g_animationManager.Done();
        }
    }
    else
    {
        if ( _attackers[ _attackerMax-1 ].IsPastWidth() )
        {
            g_animationManager.Done();
        }
    }
}


/////////////////////////////////////////////////////////////////////////


Robotron::Shooter::Shooter()
    : _lastFire         ( g_ms16 )
    , _lastMoveY        ( g_ms16 )
    , _lastTargetY      ( g_ms16 )
    , _targetY          ( random(5) )
    , _stomperSpeed     ( 400 )
{
    BitmapBase *bullet[] =
                {
                    snew<Bitmap<1>>( 8, k_robotronBullet ),
                };

    for ( Sprite & sprite : _bullet )
    {
        sprite.InitSprite( bullet, countof(bullet) );
    };
}

void      
Robotron::Shooter::Start( Point pt, Point dir )
{
    // draw first
    for ( Sprite &bullet : _bullet )
    {
        bullet.EnsureOnSpriteList();
    }

    // then clock to detect collisions
    g_clock.EnsureOnSpriteList();

    Sprite::Start( pt, dir );
}


void            
Robotron::Shooter::Loop()
{
    uint16 const    speedMoveY          = 50;
    uint16 const    speedTargetNewY     = 300;
    uint16 const    speedStompFaster    = 200;
    uint16 const    speedFire           = (IsAttack() ? 200 : 150);

    if ( (uint16(g_ms16 - _lastMoveY) > speedMoveY) )
    {
        _lastMoveY += speedMoveY;
        _pos.y += Sign( _targetY - _pos.y );
    }

    if ( (_pos.y == _targetY) &&  (uint16(g_ms16 - _lastTargetY ) > speedTargetNewY) )
    {
        _lastTargetY += speedTargetNewY;
        _targetY = random( 5 );
    }

    if ( (uint16(g_ms16 - _lastFire) > speedFire) )
    {
        for ( Sprite &bullet : _bullet )
        {
            if ( !bullet.IsRunning() )
            {
                Point   pos = _pos;

                pos.y += 5;
                pos.x += ( _dir.x < 0 ? -4 : 5 );
                bullet.StartNoFlip( pos, _dir );
                _lastFire += speedFire;
                break;
            }
        }
    }

    if ( uint16(g_ms16 - _stomperLastSpeedAdj) > speedStompFaster)
    {
        _stomperLastSpeedAdj = g_ms16;
        _stomperSpeed = _stomperSpeed - (_stomperSpeed / 8);
        _stomperSpeed = MAX( _stomperSpeed, 20 );
        if ( _stomperSpeed < 100 )
        {
            _stomperSeqRate = 1;
        }
    }

}


bool
Robotron::Shooter::IsHitByBullet( Range rangex )
{
    for ( Sprite &bullet : _bullet )
    {
        if ( bullet.IsRunning() )
        {
            if ( rangex.InRange( bullet.CenterX() ) )
            {
                bullet.Disable();
                _hits += 1;
                return true;
            }
        }
    }

    return false;
}


void            
Robotron::Shooter::NextFrame()
{
    Point   dir     = _dir;

    if ( (IsAttack()) && (_hits) && (_hits < 3) )
    {
        _dir = Point();
    }

    Sprite::NextFrame();
    _dir = dir;
}




/////////////////////////////////////////////////////////////////////////


void 
Robotron::Stomper::Init( Shooter *shooter, uint16 lastMs )
{
    _shooter = shooter;
    _lastMs = lastMs;
}


uint32      
Robotron::Stomper::MovementSpeed()
{
    return  _shooter->_stomperSpeed;
}


void
Robotron::Stomper::NextFrame()
{
    _pos.x += _dir.x;
    if ( (_pos.y < 0) || (_pos.y+12 > PANEL_HEIGHT) )
    {
        MoveY();
    }

    _bmSeqDiv += 1;
    if ( _bmSeqDiv > _shooter->_stomperSeqRate )
    { 
        _bmSeqDiv = 0;
        _bmSeq += 1;
        if (_bmSeq >= _bmMax )
        {
            _bmSeq = 0;
        }
    }

    if ( IsAttack() )
    {
        if ( _shooter->IsHitByBullet(RangeX()) )
        {
            ExplodeSprite();
        }
    }
}


void
Robotron::Stomper::MoveY()
{
    if ( IsWithinWidth() )
    {
        _pos.y += Sign( _shooter->GetPos().y - _pos.y );
    }
}



////////////////////////////////////////////////////////////////////////////////////////
//
//
//


Robotron::RescueLady::RescueLady()
{
    BitmapBase *shooter[] =
    {
        snew<Bitmap<2>>( 50, k_robotronShooter0, true ),
        snew<Bitmap<2>>( 50, k_robotronShooter1, true ),
        snew<Bitmap<2>>( 50, k_robotronShooter2, true )
    };
    
    BitmapBase *lady[] =
    {
        snew<Bitmap<4>>( 100, k_robotronLady0 ),
        snew<Bitmap<4>>( 100, k_robotronLady1 ),
        snew<Bitmap<4>>( 100, k_robotronLady2 )
    };

    _shooter.InitSprite( shooter, countof(shooter) );
    _lady.InitSprite( lady, countof(lady) );
}



IStoryboard::StartResult
Robotron::RescueLady::Start()
{
    _shooter.Start( Point(PANEL_WIDTH+20+random(20), random(5)), Point(-1,0) );
    _lady.Start( Point(-6,random(3)+1), Point(1,0) );

    return Start_DimClock;
}


void
Robotron::RescueLady::Loop()
{
    if ( !_end )
    {
        _shooter._lastFire = g_ms16;     // disable fire
    }
    _shooter.Loop();

    if ( _shooter.RangeX().InRange( _lady.CenterX() ) )
    {
        _score.Start( Point(_lady.CenterX()-5, 3), Point() );
        _lady.Disable();
        _end = g_ms + 2000;
    }

    if ( (_end) && (g_ms > _end) )
    {
        g_animationManager.Done();
    }
}


Robotron::RescueLady::ScoreSprite::ScoreSprite()
{
    BitmapBase *score[] =
    {
        snew<Bitmap<2>>( 8, k_robo4000 ),
    };

    UpdatePallet();
    _pallet[ 2 ] = Color( 0, 0, 0 );

    InitSprite( score, countof(score), _pallet );
}


void
Robotron::RescueLady::ScoreSprite::UpdatePallet()
{
    CRGB    colors[ 2 ];

    _hue += 2;
    colors[ 0 ].setHue( _hue );
    colors[ 1 ].setHue( _hue + 32 );

    for ( int index=0; index < countof(colors); ++index )
    {
        _pallet[ index ] = Color( colors[ index ] );
    }
}


void
Robotron::RescueLady::ScoreSprite::NextFrame()
{
    UpdatePallet();
    Sprite::NextFrame();
}

