/*
 * Digit.cpp
 *  1 Digit of the clock (time) on the display
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

#include "platform.h"

static uint8        g_dir;
DrawMode            g_clockDrawMode = Draw_;


#define DIGIT_FRAME_RATE      30    // ms between digit sprites moving 

static const uint8 PROGMEM k_zero[]    = { 7, 7, 0x1c, 0x32, 0x63, 0x63, 0x63, 0x26, 0x1c };
static const uint8 PROGMEM k_one[]     = { 6, 7, 0x0c, 0x0e, 0x0c, 0x0c, 0x0c, 0x0c, 0x3f };
static const uint8 PROGMEM k_two[]     = { 7, 7, 0x3e, 0x63, 0x70, 0x3c, 0x1e, 0x07, 0x7f };
static const uint8 PROGMEM k_three[]   = { 7, 7, 0x7e, 0x30, 0x18, 0x3c, 0x60, 0x63, 0x3e };
static const uint8 PROGMEM k_four[]    = { 7, 7, 0x38, 0x3c, 0x36, 0x33, 0x7f, 0x30, 0x30 };
static const uint8 PROGMEM k_five[]    = { 7, 7, 0x3f, 0x03, 0x3f, 0x60, 0x60, 0x63, 0x3e };
static const uint8 PROGMEM k_six[]     = { 7, 7, 0x3c, 0x06, 0x03, 0x3f, 0x63, 0x63, 0x3e };
static const uint8 PROGMEM k_seven[]   = { 7, 7, 0x7f, 0x63, 0x30, 0x18, 0x0c, 0x0c, 0x06 };
static const uint8 PROGMEM k_eight[]   = { 7, 7, 0x1e, 0x23, 0x27, 0x1e, 0x79, 0x61, 0x3e };
static const uint8 PROGMEM k_nine[]    = { 7, 7, 0x3e, 0x63, 0x63, 0x7e, 0x60, 0x30, 0x1e };
static const uint8 PROGMEM k_dots[]    = { 1, 7, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00 };
static const uint8 PROGMEM k_none[]    = { 0, 0 };


static
uint8 const *
DigitBitmap( uint8 value )
{
    uint8 const * bm = nullptr;

    switch ( value )
    {
    case  0:    bm = k_zero;        break;
    case  1:    bm = k_one;         break;
    case  2:    bm = k_two;         break;
    case  3:    bm = k_three;       break;
    case  4:    bm = k_four;        break;
    case  5:    bm = k_five;        break;
    case  6:    bm = k_six;         break;
    case  7:    bm = k_seven;       break;
    case  8:    bm = k_eight;       break;
    case  9:    bm = k_nine;        break;
    case 10:    bm = k_dots;        break;
    default:    bm = k_none;        break;
    }

    return bm;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//
//

Digit::Digit()
{
}


void
Digit::SetColor( uint16 color )
{
    _current.SetColor( color );
    _old.SetColor( color );
}


bool
Digit::IsMoving() const
{
    return _current.IsMoving();
}


uint8
Digit::PixelWidth() const
{
    return _current.PixelWidth();
}


uint8
Digit::GetValue() const
{
    return _current.GetValue();
}


void
Digit::SetX( uint8 x )
{
    if ( x != _current.PosX() )
    {
        StartNewValue();
        _current.SetX( x );
    }
}


void        
Digit::SetValue( uint8 value )
{
    if ( _current.GetValue() != value )
    {
        StartNewValue();
        _current.SetValue( value );
    }
}


void
Digit::RedrawIfDamaged()
{
    if ( (_current.IsModified()) || (_current.SpriteMode() == SpriteBase::Mode::Disabled) )
    {
        StartNewValue();
        _current.SetValue( _current.GetValue() );
    }
}


void
Digit::StartNewValue()
{
    if ( (!_old.IsMoving()) || (!_current.IsMoving()) )
    {
        int16       dirY = (g_dir & 1) ? 1 : -1;

        g_dir += 1;
        _old = _current;
        _old.SetDir( dirY );
        _current.SetStartPos( dirY );
    }
}


void
Digit::BaseDraw( Point offset )
{
    _current.BaseDraw( offset );
    _old.BaseDraw( offset );
}


void
Digit::BaseNextFrame()
{
    _current.BaseNextFrame();
    _old.BaseNextFrame();
}


//////////////////////////////////////////////////////////////////////////////
//
//


DigitSprite::DigitSprite()
    : _color            ( 0 )
    , _value            ( 99 )
    , _width            ( 0 )
    , _forceAdvance1    ( false )
    , _modified         ( false )
{
    _pos.y = Clock::k_DigitY;
}


DigitSprite &
DigitSprite::operator = ( const DigitSprite &rh )
{
    SpriteBase::operator = ( rh );
    _color = rh._color;
    _value = rh._value;
    _width = rh._width;
    _forceAdvance1 = rh._forceAdvance1;
    _modified = rh._modified;
    memcpy( _bitmap, rh._bitmap, sizeof( _bitmap ) );
    return *this;
}


void
DigitSprite::SetColor( uint16 color )
{
    _color = color;
}


uint8
DigitSprite::PixelWidth() const
{
    return _width;
}


void
DigitSprite::SetX( uint8 x )
{
    _pos.x = x;
}


uint8 
DigitSprite::GetValue() const
{
    return _value;
}


void       
DigitSprite::SetDir( int16 dirY )
{
    _drawMode       = SpriteBase::Mode::Started;
    _dir.y          = dirY;
    _forceAdvance1  = true;
}


void
DigitSprite::SetValue( uint8 value )
{
    uint8 const     offset = BitmapBase::Header_SizeX;

    // first bytes are speed, offset.x, offset.y.  ours is always zero
    MemCpy( _bitmap+offset, DigitBitmap(value), sizeof(_bitmap)-offset );
    _modified = false;

    _value = value;
    _width = _bitmap[ BitmapBase::Header_SizeX ];
    if ( _width )
    {
        _width += 1;
    }

    _drawMode = SpriteBase::Mode::Started;
}


void
DigitSprite::SetStartPos( int16 dirY )
{
    _drawMode   = SpriteBase::Mode::Started;
    _pos.y      = ( dirY > 0 ? -6 : 15 );
    _dir.y      = dirY;
}


uint8 
DigitSprite::PosX() const
{
    return _pos.x;
}


bool
DigitSprite::IsMoving() const
{
    return ( (_pos.y != 4) || (_forceAdvance1) );
}


bool
DigitSprite::IsModified() const
{
    return _modified;
}



uint32
DigitSprite::MovementSpeed()
{
    return DIGIT_FRAME_RATE;
}


void
DigitSprite::NextFrame()
{
    if ( IsMoving() )
    {
        _pos += _dir;
        _forceAdvance1 = false;
    }
}


DrawResult
DigitSprite::Draw( Point offset )
{
    DrawResult      result      = DrawResult::None;
    DrawMode        drawMode    = g_clockDrawMode;

    result = Bitmap<1>( _bitmap ).Draw( _pos + offset, drawMode, &_color );

    // if collisions, we change our value to 
    if ( (result & DrawResult::Collision) && (drawMode & Draw_EarseCollisions) )
    {
        _modified = true;
    }

    return result;
}
