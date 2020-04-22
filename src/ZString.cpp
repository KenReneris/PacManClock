/*
 * ZString
 *  Misc string functions & classes
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

#include "platform.h"


void     
Out( char const *format, ... )
{
    va_list     args;

    va_start ( args, format );
    VOut( (FSTR const *) format, args );
    va_end ( args );
}


void 
Out( FSTR const *format, ... )
{
    va_list     args;

    va_start ( args, format );
    VOut( format, args );
    va_end ( args );
}


void
VOut( FSTR const *format, va_list args )
{
    StringBuffer( OutStr ).VAddF( format, args );
}


void
OutStr( char const *str )
{
    for ( char c = *str; c; ++str, c = *str )
    {
        ets_putc( c );
        yield();        // Can't use Out() during redraw if we have this here or display might flicker (can update before redrawn)
    }
}   


////////////////////////////////////////////////////////////////////////////////////////
//
//
//


void     
VAppendF( String &str, FSTR const *format, va_list args )
{
    StringBuffer        
        buffer( 
            [&str]( char const *add )
            {
                str += add;
            } );

    buffer.VAddF( format, args );
}


String
PrintF( char const *format, ... )
{
    va_list     args;

    va_start ( args, format );
    return VPrintF( (FSTR const *) format, args );
    // skipped va_end
}


String
PrintF( FSTR const *format, ... )
{
    va_list     args;

    va_start ( args, format );
    return VPrintF( format, args );
    // skipped va_end
}


String
VPrintF( FSTR const *format, va_list args )
{
    String      str;

    VAppendF( str, format, args );
    return str;
}


void 
AppendF( String &str, char const *format, ... )
{
    va_list args;
    va_start ( args, format );

    VAppendF( str, (FSTR const *) format, args );

    va_end ( args );
}


void
AppendF( String &str, FSTR const *format, ... )
{
    va_list args;
    va_start ( args, format );

    VAppendF( str, format, args );

    va_end ( args );
}


void     
SPrintF( char *out, char const *format, ... )
{
    va_list args;
    va_start ( args, format );

    VSPrintF( out, (FSTR const *) format, args );

    va_end ( args );
}


void     
SPrintF( char *out, FSTR const *format, ... )
{
    va_list args;
    va_start ( args, format );

    VSPrintF( out, format, args );

    va_end ( args );
}


void
VSPrintF( char *out, FSTR const *format, va_list args )
{
    StringBuffer
        buffer(
            [&out]( char const *add )
            {
                strcpy( out, add );
                out += strlen( add );
            } );

    buffer.VAddF( format, args );
}


///////////////////////////////////////////////////////////////////////////////////////////
//
//
//


class StringBuffer::Print
{
public:
    Print( StringBuffer *sb, FSTR const *format, va_list args );

private:
    void        PrintArg();
    int         PrintInt( char ftype );
    int         PrintFloat( char ftype );
    int         PrintStr();
    void        FillRight( int width );

private:
    StringBuffer          * _sb;
    EnumStr::iterator       _format;
    va_list                 _args;

    uint8                   _width;
    bool                    _zeroPad;
    bool                    _fillLeft;
    
};


StringBuffer::Print::Print( StringBuffer *sb, FSTR const *format, va_list args )
    : _sb           ( sb )
    , _format       ( EnumStr(format).begin() )
    , _args         ( args )
    , _width        ( 0 )
    , _zeroPad      ( false )
    , _fillLeft     ( false )
{
    for( char c = *_format ; c ; c = *_format )
    {
        // NEXT
        ++_format;
    
        switch ( c )
        {
        case '\\':
            _sb->Add( *_format );
            ++_format;
            break;
    
        case '%':
            PrintArg();
            break;
    
        default:
            _sb->Add( c );
            break;
        }
    }
}


void
StringBuffer::Print::PrintArg()
{
    _width = 0;
    _zeroPad = false;
    _fillLeft = true;

    for ( char c = *_format; c; c = *_format )
    {
        // NEXT
        ++_format;

        switch( c )
        {
        case '%':
            _sb->Add( '%' );
            return;         // RETURN

        case 'x':
        case 'X':
        case 'd':
        case 'u':
        case 'c':
            FillRight( PrintInt( c ) );
            return;

        case 'f':
            FillRight( PrintFloat( c ) );
            return;

        case 's':
            FillRight( PrintStr() );
            return;

        case '0':
            _zeroPad = ( !_width );
            // FALLTHROUGH
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            _width = (_width * 10) + (c - '0');
            break;

        case '-':
            _fillLeft = false;
            break;

        default:
            Out( F("\nPrintArg: %d, %c\n" ), c, c );
            Assert( F("PrintArg") );
            break;
        }
    }
}


int
StringBuffer::Print::PrintInt( char ftype )
{
    int const  arg      = va_arg( _args, int );
    int         len     = 0;
    char        format[ 4 ];
    char        str[ 32 ];

    format[ 0 ] = '%';
    format[ 1 ] = ftype;
    format[ 2 ] = 0;

    len = sprintf( str, format, arg );

    if ( (len < _width) && (_fillLeft) )
    {
        _sb->Fill( (_zeroPad ? '0' : ' '), _width - len );
    }

    _sb->_Add( str, len );
    return len;
}


int
StringBuffer::Print::PrintFloat( char ftype )
{
    double const    arg = va_arg( _args, double );
    int             len = 0;
    String          str( arg, 6 );

    len = str.length();
    if ((len < _width) && (_fillLeft))
    {
        _sb->Fill( (_zeroPad ? '0' : ' '), _width - len );
    }

    _sb->_Add( str.c_str(), len );
    return len;
}


int
StringBuffer::Print::PrintStr()
{
    char const    * arg     = va_arg( _args, char * );
    uint16          strLen  = 0;

    EnumStr( arg ).ForEachChunk(
        [&]( char const *str, uint len )
        {
            _sb->_Add( str, len );
            strLen += len;
        } );

    return strLen;
}


void
StringBuffer::Print::FillRight( int w )
{
    if ( (w < _width) && (!_fillLeft) )
    {
        _sb->Fill( ' ', _width - w );
    }
}


///////////////////////////////////////////////////////////////////////////////////////////
//
//
//


StringBuffer::StringBuffer( std::function<void( char const * )> const & flush )
    : _pos      ( 0 )
    , _flush    ( flush )
{
}


StringBuffer::~StringBuffer()
{
    Flush();
}


void
StringBuffer::Reset()
{
    _pos = 0;
}


void
StringBuffer::Flush()
{
    if ( _pos )
    {
        _buffer[ _pos ] = 0;
        _flush( _buffer );
        _pos = 0;
    }
}


void
StringBuffer::Add( char c )
{
    if ( _pos >= sizeof(_buffer) - 2 )
    {
        Flush();
    }

    _buffer[ _pos ] = c;
    _pos += 1;
}


void
StringBuffer::Add( const char *str )
{
    Add( (FSTR const *) str );
}


void 
StringBuffer::Add( FSTR const *str )
{
    EnumStr( str ).ForEachChunk(
        [this]( char const *str, uint len )
        {
            _Add( str, len );
        } );
}


void 
StringBuffer::Add( const String &s )
{
    _Add( s.c_str(), s.length() );
}


void
StringBuffer::Add( char const *c, uint len )
{
    Add( (FSTR const *) c, len );
}


void
StringBuffer::Add( FSTR const *str, uint len )
{
    EnumStr( str ).ForEachChunk( 
        [this]( char const *chunk, uint len ) 
        { 
            _Add( chunk, len ); 
        } );
}


void
StringBuffer::_Add( char const *c, uint len )
{
    if ( _pos+len > sizeof(_buffer)-1 )
    {
        Flush();
        if ( len > sizeof(_buffer)-1 )
        {
            _flush( c );
            len = 0;
        }
    }

    memcpy( _buffer + _pos, c, len );
    _pos += len;
}


void 
StringBuffer::AddF( char const *format, ... )
{
    va_list         args;

    va_start( args, format );
    VAddF( (FSTR const *) format, args );
    va_end( args );
}


void 
StringBuffer::AddF( FSTR const *format, ... )
{
    va_list         args;

    va_start( args, format );
    VAddF( format, args );
    va_end( args );
}


void 
StringBuffer::VAddF( char const *format, va_list args )
{
    VAddF( (FSTR const *) format, args );
}

void
StringBuffer::VAddF( FSTR const *format, va_list args )
{
    Print( this, format, args );
}


void
StringBuffer::Fill( const char c, int len )
{
    for ( ; len > 0; --len )
    {
        Add( c );
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//



//////////////////////////////////////////////////////////////////////////////////
//
//
//

String
Duration( uint32 x )
{
    int         seconds;
    int         hours;
    int         minutes;
    int         days;
    String      str;

    auto addSep = 
        [&]()
        {
            if ( str.length() )
            {
                str += " ";
            }
        };

    auto addNum =
        [&]( FSTR const *text, int v )
        {
            if ( v )
            {
                addSep();
                AppendF( str, F("%d %s%s"), v, text, (v > 1 ? "s" : "") );
            }
        };

    seconds = x % 60;
    x /= 60;

    minutes = x % 60;
    x /= 60;

    hours = x % 24;
    x /= 24;

    days = x;

    addNum( F("day"), days );
    addNum( F("hour"), hours );
    addNum( F("minute"), minutes );
    addNum( F("second"), seconds );
    if ( !str.length() )
    {
        str += " ";
    }

    return str;
}


String 
UrlEncode( const String &str )
{
    String      r;

    for ( char c : str )
    {
        if ( c == ' ' )
        {
            r += '+';
 
        }
        else if ( isalnum(c) )
        {
            r += c;
        }
        else
        {
            AppendF( r, "%%%02X", c );
        }
    }

    return r;
}


String
u64str( int64 v )
{
    char    buffer[ 32 ];
    char  * p = buffer + sizeof( buffer ) - 1;
    bool    neg = false;

    if (v < 0)
    {
        v = -v;
        neg = true;
    }

    *p = 0;
    do
    {
        p -= 1;
        *p = '0' + (v % 10);
        v /= 10;
    }
    while (v);

    if (neg)
    {
        p -= 1;
        *p = '-';
    }

    return String( p );
}
