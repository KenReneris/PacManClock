/*
 * Pong.cpp
 *  Pong animation
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

#include "platform.h"
#include "Pong.h"


const uint8 PROGMEM k_paddle[] = { 1, 4,0,0, 1, 0,  1,4, 0xFF,0x80,0x80,  0xFF, 0xFF, 0xFF, 0xFF };
const uint8 PROGMEM k_puck[]   = { 1, 4,0,0, 1, 0,  1,1, 0xFF,0x80,0x00,  0xFF };


Pong::Pong()
    : _leftPaddle   ( this )
    , _rightPaddle  ( this )
    , _puck         ( this )
{
    g_flipX = false;

    BitmapBase * paddle[] = 
        {
            snew<Bitmap<1>>( 30, k_paddle )
        };

    BitmapBase * puck[] =
        {
            snew<Bitmap<1>>( 10, k_puck )
        };

    _leftPaddle.InitSprite( paddle, countof(paddle) );
    _rightPaddle.InitSprite( paddle, countof(paddle) );
    _puck.InitSprite( puck, countof(puck) );
}


IStoryboard::StartResult
Pong::Start()
{
    int16   ly = -4;
    int16   ry = PANEL_HEIGHT;

    if ( random(2) )
    {
        std::swap( ly, ry );
    }

    _puck.Start();
    _leftPaddle.Start( Point(0, 0), Point() );
    _rightPaddle.Start( Point(PANEL_WIDTH-1, 0), Point() );

    return Start_DimClock;
}


void 
Pong::Loop()
{
    if ( _puck.GameOver() )
    {
        bool        gameOver = true;

        gameOver &= _leftPaddle.ExitGame();
        gameOver &= _rightPaddle.ExitGame();
        if ( gameOver )
        {
            g_animationManager.Done();
        }
    }
    else
    {
        _leftPaddle.Track();
        _rightPaddle.Track();
    }
}


/////////////////////////////////////////////////////////////////////////////////////

Pong::Paddle::Paddle( Pong *parent )
    : _parent ( parent )
{
}

bool
Pong::Paddle::ExitGame()
{
    if ( !_dir.y )
    {
        _dir.y = ( random(2) ? -1 : +1 );
    }

    return ( SpriteMode() == SpriteBase::Mode::Disabled );
}


void
Pong::Paddle::Track()
{
    Point   pos     = _parent->_puck.GetPos();
    int16   distX   = _pos.x - pos.x;
    int32   speed   = 60;

    if ( Sign(distX) == _parent->_puck.GetDir().x )
    {
        if ( _target > 4 )
        {
            if ( _parent->_hits < 10 )
            {
                _target = random( 0, 2 ) + 1;
            }
            else
            {
                _target = random( 0, 4 );
            }
        }

        _dir.y = Sign( pos.y - (_pos.y + _target) );
        if ( ABS(distX) < PANEL_WIDTH/2 )
        {
            speed = 30;
        }
    }
    else
    {
        _target = PANEL_HEIGHT / 2 - 2;
        _dir.y = Sign( _target - _pos.y );
    }

    ForEachBitmap(
        [speed]( BitmapBase *bm )
        {
            bm->SetMovementSpeed( speed );
        } );
}


/////////////////////////////////////////////////////////////////////////////////////


Pong::Puck::Puck( Pong *parent )
    : _parent ( parent )
{
}


Point
Pong::Puck::GetPos() const
{
    return _pos1;
}


bool
Pong::Puck::GameOver() const
{
    return _gameOver;
}


void
Pong::Puck::Start()
{
    EnsureOnSpriteList();
    _pos = Point( PANEL_WIDTH/2, PANEL_HEIGHT/2 );
    _pos1 = _pos;
    _drawMode = Mode::Started;

    _lastX = g_ms;
    _lastY = g_ms;

    uint16 angle = random( -30, +30 ) + (random( 2 ) ? 180 : 0);
    SetVector( PI/180.0 * angle, 50 );
}


void
Pong::Puck::SetVector( float angle, uint16 vectorRate )
{
    _dir = Point( 1, -1 );

    float   x = cos( angle );
    float   y = sin( angle );

    if ( x < 0 )
    {
        x = -x;
        _dir.x = -1;
    }

    if ( y < 0 )
    {
        y = -y;
        _dir.y = +1;
    }

    _rate.x = float( vectorRate ) / x;
    _rate.y = float( vectorRate ) / y;
}


void
Pong::Puck::BaseNextFrame()
{
    uint32      ms      = g_ms;
    bool        update  = false;
        
    if ( ms - _lastY > _rate.y )
    {
        _pos1.y += _dir.y;
        _lastY = ms;

        // bound off top & bottom
        if ( _pos1.y <= 0 )  
        {
            _dir.y = 1;
            update = true;
        }

        if ( _pos1.y >= PANEL_HEIGHT - 1 )
        {
            _dir.y = -1;
            update = true;
        }
    }

    if ( ms - _lastX > _rate.x )
    {
        _pos1.x += _dir.x;
        _lastX = ms;

        // TODO hit paddel or end
        if ( (_pos1.x == 0) || (_pos1.x == PANEL_WIDTH-1) )
        {
            int16   posY    = 0;

            update = true;
            if ( _pos1.x ) 
            {
                posY = _parent->_rightPaddle.GetPos().y;
            }
            else
            {
                posY = _parent->_leftPaddle.GetPos().y;
            }

            if ( (_pos1.y < posY) || (_pos1.y > posY+4) )
            {
                // missed the paddle
                _gameOver = true;
                Out( F("MISSED\n") );
            }
            else
            {
                float   speedX      = 1.0 / float(_rate.x) * _dir.x;
                float   speedY      = 1.0 / float(_rate.y) * _dir.y;
                float   speed       = sqrt( speedX*speedX + speedY*speedY );
                float   influence   = 0.70;
                float   pos         = ( _pos1.y - posY );       // 0..3

                _parent->_hits += 1;
                speed *= 1.08;

                pos += random( -500, +500 ) / 1000.0;           // -.5 .. +3.5
                pos += 0.5;                                     // 0..4 
                pos = (pos / 2.0) - 1.0;                        // -1 .. +1

                speedY = speed * pos * influence;
                speedX = sqrt( speed*speed - speedY*speedY );
                _dir.x = -_dir.x;

                _dir.y = 1;
                if ( speedY < 0 )
                {
                    _dir.y = -_dir.y;
                    speedY = -speedY;
                }

                _rate.x = 1.0 / speedX;
                _rate.y = 1.0 / speedY;

                _rate.x = MAX( _rate.x, 10 );
                _rate.y = MAX( _rate.y, 10 );

                if ( _pos1.y + _dir.y < 0 )
                {
                    _dir.y = 1;
                }

                if ( _pos1.y + _dir.y >= PANEL_HEIGHT )
                {
                    _dir.y = -1;
                }
            }
        }
    }

    // de-jitter display.. only update the display position when fastest moving axis updates
    if ( _rate.x <= _rate.y )
    {
        update |= ( _pos1.x != _pos.x );
    }
    else
    {
        update |= ( _pos1.y != _pos.y );
    }

    if ( update )
    {
        _pos = _pos1;
    }
}



