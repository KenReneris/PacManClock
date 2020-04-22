/*
 * Marquee.cpp
 *  Scrolling marquee text
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"


Marquee::Marquee()
    : _posX     ( 0 )
{
}


void
Marquee::Show( char const *format, ... )
{
    va_list     args;

    va_start ( args, format );
    g_marquee.VShow( (FSTR const *) format, args );
    va_end ( args );
}


void
Marquee::Show( FSTR const *format, ... )
{
    va_list     args;

    va_start ( args, format );
    g_marquee.VShow( format, args );
    va_end ( args );
}


void 
Marquee::VShow( FSTR const *format, va_list args )
{
    Text      * text        = nullptr;
    uint16      color       = ColorFromPallet( 0 );
    uint8       outIndex    = 0;

    auto flush=
        [&]()
        {
            if ( text )
            {
                text->_text[ outIndex ] = 0;
                text->InitWidth();
                text->_pos.x = _posX;
                _posX += text->_width;  
                text->_next = _head;
                _head = text;
                text = nullptr;
            }
        };

    auto output =
        [&]( char const *str )
        {
            for ( char const *p = str; *p; ++p )      // BUGBUG
            {
                char    c = *p;

                if ( (c == '^') && (p[1]) )
                {
                    flush();
                    color = ColorFromPallet( *p - '0' );
                    p += 1;
                }
                else
                {
                    ets_putc( c );
                    if ( c == '\n' )
                    {
                        c = ' ';
                    }

                    if ( !text )
                    {
                        text = new Text( color );
                        outIndex = 0;
                    }

                    text->_text[ outIndex ] = c;
                    outIndex += 1;

                    // full?
                    if ( outIndex == countof(text->_text)-2 )     
                    {
                        flush();
                    }
                }
            }
            flush();
        };

    Out( F("Marquee: ") );
    if ( !IsInit() )
    {
        StringBuffer( output ).VAddF( format, args );
    }
    else
    {
        StringBuffer( OutStr ).VAddF( format, args );
    }
}


uint16
Marquee::ColorFromPallet( uint8 colorIndex )
{
    uint16      color = Color( 255, 255, 255 );

    switch (colorIndex)
    {
    case 1:         return  Color( 0xFF, 0x00, 0x00 );
    case 2:         return  Color( 0x00, 0xFF, 0x00 );
    case 3:         return  Color( 0x00, 0x00, 0xFF );
    // etc..

    case 0:
    default:        return  Color( 0xFF, 0xFF, 0xFF );
    }
}


bool 
Marquee::IsPending() const
{
    return !!_head;
}


IStoryboard::StartResult
Marquee::Start()
{
    snew<Marquee::Animation>()->Start( Point(PANEL_WIDTH, 0), Point(-1, 0) );
    return StartResult( Start_ClockToBottom | Start_DisableTimeUpdates );
}


void 
Marquee::Loop()
{
    if ( (_animation) && (_animation->SpriteMode() == SpriteBase::Mode::Disabled) )
    {
        Text * text = _head;
            
        _head = nullptr;
        _posX = 0;
        _animation = nullptr;

        for (; text; text = text->_next)
        {
            delete text;
        }

        g_animationManager.Done();
    }
}


//////////////////////////////////////////////////////////////////////////////
//
//
//


Marquee::Text::Text( uint16 color )
    : _color    ( color )
    , _width    ( 0 )
    , _next     ( nullptr )
{
    _text[ 0 ] = 0;
}


void     
Marquee::Text::InitWidth()
{
    int16       x1, y1;
    uint16      h = 0;
    uint16      w = 0;

    g_display.getTextBounds( _text, _pos.x, _pos.y, &x1, &y1, &w, &h );
    //Out( "TextBounds: '%s' %d,%d %d,%d %d,%d\n", _text, _pos.x, _pos.y, x1, y1, w, h );
    _width = w;
}


DrawResult
Marquee::Text::Draw( Point offset ) const
{
    Point       pos     = offset + _pos;
    DrawResult  result = DrawResult::None;

    if ( (pos.x+_width >= 0) && (pos.x < PANEL_WIDTH) )
    {
        g_display.setCursor( pos.x, pos.y );
        g_display.setTextColor( _color );
        g_display.print( _text );
        result = DrawResult::Drawn;
    }

    return result;
}



//////////////////////////////////////////////////////////////////////////////
//
//
//


Marquee::Animation::Animation()
{
    Assert( !g_marquee._animation, F("Marquee") );
    g_marquee._animation = this;
}


uint32                  
Marquee::Animation::MovementSpeed()
{
    return k_MovementRate;
}


DrawResult
Marquee::Animation::Draw( Point offset )
{
    uint8      result = 0;

    for ( Text * text=g_marquee._head; text; text=text->_next )
    {
        result |= text->Draw( offset + _pos );
    }

    return DrawResult( result );
}

