/*
 * ZString.h
 *  Misc string functions
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


extern void     Out( char const *format, ... );
extern void     Out( FSTR const *format, ... );
extern void     VOut( FSTR const *format, va_list args );
extern void     OutStr( char const *str );
extern String   PrintF( char const *format, ... );
extern String   PrintF( FSTR const *format, ... );
extern String   VPrintF( FSTR const *format, va_list args );
extern void     AppendF( String &str, char const *format, ... );
extern void     AppendF( String &str, FSTR const *format, ... );
extern void     VAppendF( String &str, FSTR const *format, va_list args );
extern void     SPrintF( char *out, char const *format, ... );
extern void     SPrintF( char *out, FSTR const *format, ... );
extern void     VSPrintF( char *out, FSTR const *format, va_list args );

String      Duration( uint32 seconds );
String      UrlEncode( const String &str );
String      u64str( int64 v );


// like a string builder, but invokes the "fluch" callback function when full
class StringBuffer
{
public:
    StringBuffer( function<void( char const* )> const & flush );
    ~StringBuffer();

    void            Reset();

    void            Add( char const c );
    void            Add( char const *c );
    void            Add( FSTR const *c );
    void            Add( String const &c );
    void            Add( char const *c, uint len );
    void            Add( FSTR const *c, uint len );
    void            AddF( char const *format, ... );
    void            AddF( FSTR const *format, ... );
    void            VAddF( char const *format, va_list args );
    void            VAddF( FSTR const *format, va_list args );
    void            Fill( char c, int len );
    void            Flush();

private:
    void            _Add( char const *c, uint len );

private:
    class Print;

private:
    uint8                               _pos;
    char                                _buffer[ 256-1-16 ];
    std::function<void( char const* )>  _flush;
};

