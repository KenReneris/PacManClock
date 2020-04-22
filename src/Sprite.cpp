/*
 * Sprite.cpp
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

#include "platform.h"


SpriteBase   * g_spriteHead;
SpriteBase  ** g_spriteTail;



void
SpriteBase::ResetSpriteList()
{
    g_spriteTail = & g_spriteHead;
    g_spriteHead = nullptr;
    g_clock._next = nullptr;
}


SpriteBase::SpriteBase()
    : _drawMode     ( Mode::NotStarted )
    , _maxWidth     ( 0 )
    , _collision    ( false )
    , _lastMs       ( 0 )
    , _next         ( nullptr )
{
}


SpriteBase * 
SpriteBase::NextSprite()
{
    return _next;
}


SpriteBase::Mode
SpriteBase::SpriteMode() const
{
    return _drawMode;
}


bool
SpriteBase::HadCollision() const
{
    return _collision;
}


void
SpriteBase::NextFrame()
{
    _pos += _dir;
}


void
SpriteBase::StartNoFlip( Point pt, Point dir )
{
    EnsureOnSpriteList();
        
    _pos = pt;
    _dir = dir;
    _lastMs   = g_ms16;
    _drawMode = Mode::Started;
}


void
SpriteBase::Start( Point pt, Point dir )
{
    EnsureOnSpriteList();
        
    _pos = FlipX( pt );
    SetDir( dir );
    _lastMs   = g_ms16;
    _drawMode = Mode::Started;
}


void
SpriteBase::SwitchTo( SpriteBase *start )
{
    start->EnsureOnSpriteList();
    start->_pos = _pos;
    start->_dir = _dir;
    start->_lastMs = g_ms16;
    start->_drawMode = Mode::Started;

    _drawMode = Mode::Disabled;
    _collision = false;
}


void
SpriteBase::SwitchTo( SpriteBase *start, Point dir )
{
    start->EnsureOnSpriteList();
    start->_pos = _pos;
    SetDir( dir );
    start->_lastMs = g_ms16;
    start->_drawMode = Mode::Started;

    _drawMode = Mode::Disabled;
    _collision = false;
}


void
SpriteBase::EnsureOnSpriteList() 
{
    if ( (!_next) && (g_spriteTail != &_next) )
    {
        *g_spriteTail = this;
        g_spriteTail = &_next;
    }
}


void
SpriteBase::Disable()
{
    _drawMode = Mode::Disabled;
    _collision = false;
}


void
SpriteBase::Freeze()
{
    _dir = Point();
}


void
SpriteBase::AdjustPos( Point offset )
{
    _pos += offset;
}


void  
SpriteBase::SetDir( Point dir )
{
    _dir = dir;
    if ( (g_flipX) && (_maxWidth) )
    {
        _dir *= -1;
    }
}


void
SpriteBase::ExplodeSprite()
{
    if ( !_explode )
    {
        _explode = (random( 2 ) & 1) + 2; 
    }
}


Point
SpriteBase::FlipX( Point pt ) const
{
    pt.x = FlipX( pt.x );
    return pt;
}


int16
SpriteBase::FlipX( int16 x ) const
{
    if ( (g_flipX) && (_maxWidth) )
    {
        if ( x < PANEL_WIDTH/2 )
        {
            int16   dist = (PANEL_WIDTH / 2) - x - _maxWidth;

            x = (PANEL_WIDTH/2) + dist;
        }
        else
        {
            int16   dist = x - (PANEL_WIDTH / 2);
                
            x = (PANEL_WIDTH / 2) - dist - _maxWidth;
        }
    }

    return x;
}


uint8
SpriteBase::MaxWidth() const
{
    return _maxWidth;
}


Point
SpriteBase::GetPos() const
{
    return _pos;
}


Point
SpriteBase::GetDir() const
{
    return _dir;
}

Range         
SpriteBase::RangeX() const
{
    return Range( _pos.x, _pos.x + _maxWidth );
}


int16         
SpriteBase::CenterX() const
{
    return _pos.x + (_maxWidth / 2);
}


bool
SpriteBase::IsRunning() const
{
    return ( (_drawMode == Mode::Started) || (_drawMode == Mode::UntilDisappears) );
}


bool
SpriteBase::IsWithinWidth() const
{
    return ( (_pos.x + _maxWidth >= 0) && (_pos.x < PANEL_WIDTH) );
}


bool
SpriteBase::IsPastWidth() const
{
    if ( _dir.x > 0 )
    {
        return ( _pos.x >= PANEL_WIDTH );
    }
    else if ( _dir.x < 0 )
    {
        return ( _pos.x < int8(-_maxWidth) );
    }
    else
    {
        return true;
    }
}


bool
SpriteBase::IsOnHorzEdge() const
{
    switch ( _drawMode )
    {
    case Mode::Started:
    case Mode::UntilDisappears:
        if ( _pos.x <= 0 )
        {
            return true;
        }

        if ( (_pos.x + _maxWidth) == PANEL_WIDTH )
        {
            return true;
        }
        break;

    default:
        break;
    }

    return false;
}


void
SpriteBase::BaseNextFrame()
{
    switch ( _drawMode )
    {
    case Mode::Started:
    case Mode::UntilDisappears:
        if ( !_explode )
        {
            uint32  speed = MovementSpeed();

            if ( uint16(g_ms16 - _lastMs) > speed )
            {
                _lastMs += speed;
                NextFrame();
            }
        }
        else
        {
            if ( uint16(g_ms16 - _lastMs) > k_explodeSpeed )
            {
                _lastMs += k_explodeSpeed;
                _explode += 2;
            }
        }
        break;

    default:
        break;
    }
}


void
SpriteBase::Freezing( uint32 duration )
{
    _lastMs += duration;
}


void 
SpriteBase::BaseDraw( Point offset )
{
    DrawResult    result = DrawResult::None;

    switch ( _drawMode )
    {
    case Mode::NotStarted:
    case Mode::Disabled:
        break;

    case Mode::Started:
        result = Draw( offset );
        if ( result )
        {
            _drawMode = Mode::UntilDisappears;
        }
        break;

    case Mode::UntilDisappears:
        result = Draw( offset );
        if ( !result )
        {
            _drawMode = Mode::Disabled;
            _collision = false;
        }
        break;

    default:
        Out( F("SpriteBase::BaseDraw: FIX NEEDED (state: %d)\n"), _drawMode );
        break;
    }

    if ( result & DrawResult::Collision )
    {
        _collision = true;
    }
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//
//


Sprite::Sprite()
    : _pallet       ( nullptr )
    , _bms          ( nullptr )
    , _bmSeq        ( 0 )
    , _bmMax        ( 0 )
    , _bmSeqDiv     ( 0 )
    , _bmSeqEnabled ( 1 )
    , _bmDrawMode   ( Draw_Z )
{
}


void
Sprite::SetDrawMode( DrawMode drawMode )
{
    _bmDrawMode = drawMode;
}


void
Sprite::InitSprite( BitmapBase const * const * bms, uint8 count )
{
    InitSprite( bms, count, nullptr, false );
}


void
Sprite::InitSprite( BitmapBase const * const * bms, uint8 count, uint16 *pallet )
{
    InitSprite( bms, count, pallet, false );
}


void
Sprite::InitSprite( BitmapBase const * const * bms, uint8 count, uint16 *pallet, bool dontFlipX )
{
    if ( pallet )
    {
        _pallet = pallet;
        AssertIsSRam( pallet );
    }

    for ( uint8 index=0; index < count; ++index )
    {
        AssertIsSRam( bms[ index ] );
        _maxWidth = MAX( _maxWidth, bms[ index ]->Width() );
    }

    _bmMax  = count;
    _bms    = scopy( bms, count );

    if ( (g_flipX) && (!dontFlipX) )
    {
        //Out( "Flags: %d, %d\n", bms[ 0 ]->Flags(), count );
        for ( uint8 index = 0; index < count; ++index )
        {
            _bms[ index ] = bms[ count - index - 1 ];
        }
    }
}


void 
Sprite::ForEachBitmap( const function<void( BitmapBase *bm )> & callback )
{
    for ( uint8 index=0; index < _bmMax; ++index )
    {
        callback( const_cast<BitmapBase *>( _bms[ index ] ) );
    }
}


uint32
Sprite::MovementSpeed()
{
    return _bms[ _bmSeq ]->MovementSpeed();
}


void 
Sprite::SetSequenceEnable( bool enabled )
{
    _bmSeqEnabled = enabled;
}


void
Sprite::Freeze()
{
    SpriteBase::Freeze();
    _bmSeqEnabled = false;
}


void    
Sprite::NextFrame()
{
    SpriteBase::NextFrame();

    _bmSeqDiv += _bmSeqEnabled;
    if ( _bmSeqDiv > _bms[ _bmSeq ]->SequenceSpeed() )
    {
        uint8   flags = _bms[ _bmSeq ]->Flags();

        if ( !(flags & BitmapBase::Flag_SequenceEnd) )
        {
            if ( flags & BitmapBase::Flag_ClockEraseCollisions )
            {
                g_clockDrawMode = Draw_EarseCollisions;
            }

            _bmSeqDiv = 0;
            _bmSeq += 1;
            if ( _bmSeq >= _bmMax )
            {
                _bmSeq = 0;
            }
        }
        else
        {
            Disable();
        }
    }
}


DrawResult
Sprite::Draw( Point offset )
{
    // add offset?

    if ( !_explode )
    {
        return  _bms[ _bmSeq ]->Draw( _pos, _bmDrawMode, _pallet );
    }
    else
    {
        BitmapBase const  * bm      = _bms[ _bmSeq ];
        Point const         bmSize  = bm->Size();
        uint8               centerY = bmSize.y / 2;
        DrawResult          result  = DrawResult::None;

        for( uint8 y=0; y < bmSize.y; ++y )
        {
            int16           dist    = (y < centerY) ? (centerY - y) : (y - centerY);
            Point           pos     = _pos + Point(0, y);

            if ( dist <= (_explode/2) )
            {
                dist = (_explode/2) - dist;

                if ( _explode & 1 )
                {
                    pos.x -= dist;
                    pos.y += dist;
                }
                else
                {
                    pos.x += dist;
                    pos.y -= dist;
                }
            }

            DrawResult  r = bm->DrawRow( pos, y, Draw_OnTop );
            result = DrawResult( uint(result) | uint(r) );
        }

        return result;
    }
}

