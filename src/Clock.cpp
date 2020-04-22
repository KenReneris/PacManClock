/*
 * Clock.cpp
 *  Top level class for the pacman clock.
 *  Holds the digits on the display etc
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */



#include "platform.h"


Clock::Clock()
    : _currentBrightness    ( 255 )
    , _targetBrightness     ( 255 )
{
}


void
Clock::Setup()
{
    UpdateColors();
    _digits[ 2 ].SetValue( 10 );
}


void
Clock::SetBrightness( uint8 level )
{
    _targetBrightness = level;
}


void
Clock::SetTargetOffset( Point offset )
{
    _targetOffset = offset;
    if ( (_currentOffset.x < 0) || (_currentOffset.x > PANEL_WIDTH) )
    {
        SetOffset( offset );
    }
}


void
Clock::SetOffset( Point offset )
{
    _targetOffset = offset;
    _currentOffset = offset;
}

bool
Clock::IsAtIdleOffset() const
{
    return (_targetBrightness == 0xFF) && (_currentOffset == Point());
}


void
Clock::DisableTimeUpdates( bool disableTimeUpdates )
{
    _disableTimeUpdates = disableTimeUpdates;
}


void
Clock::EnsureQueued()
{
    _drawMode = Mode::NotStarted;
    Start( Point(), Point() );
}


uint32
Clock::MovementSpeed()
{
    return 0;
}


void
Clock::BaseNextFrame()
{
    if ( _currentBrightness != _targetBrightness )
    {
        if ( (g_ms - _lastBrightnessChange) > k_BrightnessAdjustRate )
        {
            _currentBrightness += ( _targetBrightness > _currentBrightness ? +1 : -1 );
            _lastBrightnessChange = g_ms;
            UpdateColors();
        }
    }

    if ( _currentOffset != _targetOffset )
    {
        if ( (g_ms - _lastOffsetMove) > k_MovementRate )
        {
            _currentOffset += ( _targetOffset - _currentOffset ).Sign();
            _lastOffsetMove = g_ms;
        }
    }

    if ( !_disableTimeUpdates )
    {
        uint8   hour    = g_nowTm.Hour;
    
        if ( hour > 12 )
        {
            hour -= 12;
        }
        else if ( hour == 0 )
        {
            hour = 12;
        }
    
        const uint8 h0a = hour / 10;
        const uint8 h0  = ( h0a ? h0a : 99 );
        const uint8 h1  = hour - ( h0a * 10 );
        const uint8 m0  = g_nowTm.Minute / 10;
        const uint8 m1  = g_nowTm.Minute - ( m0 * 10 );
    
        _digits[ 0 ].SetValue( h0 );
        _digits[ 1 ].SetValue( h1 );
        // [2] = colon
        _digits[ 3 ].SetValue( m0 );
        _digits[ 4 ].SetValue( m1 );
    }

    uint8 width = 0;
    for ( Digit &digit : _digits )
    {
        width += digit.PixelWidth();
    }
    
    uint8 indent = (32 - width) / 2;
    for ( Digit &digit : _digits )
    {
        digit.SetX( indent );
        indent += digit.PixelWidth();
    }
    
    for ( Digit &digit : _digits )
    {
        digit.BaseNextFrame();
    }

    {
        uint16 colonColor = _colonColor;
        if ( (g_nowTm.Second & 1) && (!_digits[ 2 ].IsMoving()) )
        {
            colonColor = g_black;
        }

        _digits[ 2 ].SetColor( colonColor );
    }
}


void 
Clock::UpdateColors()
{
    //Out( "CurrentBrightness: %d\n", _currentBrightness );
    _clockColor = Color( _currentBrightness, 0x40, 0x7F, 0xFF );
    _colonColor = Color( _currentBrightness, 0, 0, 0xFF );

    _digits[ 0 ].SetColor( _clockColor );
    _digits[ 1 ].SetColor( _clockColor );
    // [2] = colon
    _digits[ 3 ].SetColor( _clockColor );
    _digits[ 4 ].SetColor( _clockColor );
}


void
Clock::AnimationOver()
{
    for ( Digit &digit : _digits )
    {
        digit.RedrawIfDamaged();
    }
}


void
Clock::DrawFirst()
{
    switch ( g_globalState.IsDisplaying() )
    {
    case GlobalState::TimeOff:
        break;

    case GlobalState::TimeNotSet:
    {
        Point   pos ( _currentOffset );

        pos.x += 4;
        pos.y += k_DigitY;
        g_display.setCursor( pos.x, pos.y );
        g_display.setTextColor( _clockColor );
        g_display.print( "!set" );
    }
    break;

    default:
        break;
    }
}


void
Clock::BaseDraw( Point offset )
{
    switch ( g_globalState.IsDisplaying() )
    {
    case GlobalState::TimeOff:
    case GlobalState::TimeNotSet:
        break;

    default:
        // display the time
        for ( Digit &digit : _digits )
        {
            digit.BaseDraw( _currentOffset );
        }
        break;
    }
}


DrawResult
Clock::Draw( Point offset )
{
    // handled in BaseDraw
    return DrawResult::None;
}