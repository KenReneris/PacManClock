/*
 * EnumStr.h
 *  Misc string functions
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


inline 
bool
IsRom( void const * p )
{
    return ( ((uint) p) > 0x40200000 );
}


inline
char
ReadChar( FSTR const * p )
{
    if ( IsRom(p) )
    {
        return pgm_read_byte_far( p );
    }
    else
    {
        return *reinterpret_cast<char const *>( p );
    }
}


class EnumStr
{
public:
    class iterator
    {
    public:
        iterator( char const *str );

        char        operator * () const;
        bool        operator == ( const iterator &rh ) const;
        bool        operator != ( const iterator &rh ) const;
        iterator &  operator ++ ();

        // used by ZString.  must be called on initial begin() 
        uint16      Length() const;
        void        ForEachChunk( const function<void( char const *str, uint len )> &callback );

    private:
        void        FetchBuffer();
        friend class EnumStr;

    private:
        char const    * _str;
        char const    * _pos;
        bool            _inc;           // if 0, then eof
        bool            _eof;
        char          * _end;
        char            _buffer[ 80 ];
        char            _magic;
    };

public:
    EnumStr( char const *str );
    EnumStr( FSTR const *str );

    void            ForEachChunk( const function<void(char const *str, uint len)> &callback );
    iterator        begin() const;              
    iterator        end() const;

public:
    static char const   k_zero;

private:
    const char    * const _str;
};



inline
EnumStr::EnumStr( char const *str )
    : _str      ( str )
{
}


inline
EnumStr::EnumStr( FSTR const *str )
    : _str      ( (char const *) str )
{
}


inline
void
EnumStr::ForEachChunk( const function<void( char const *str, uint len )> &callback )
{
    if ( !IsRom(_str) )
    { 
        if ( _str )
        {
            callback( _str, strlen(_str) );
        }
    }
    else
    {
        begin().ForEachChunk( callback );
    }
}


inline 
EnumStr::iterator
EnumStr::begin() const
{
    return iterator( _str );
}

inline
EnumStr::iterator
EnumStr::end() const
{
    return iterator( &k_zero );
}


//////////////////////////////////////////////////////////
//
//
//


inline
EnumStr::iterator::iterator( char const * str )
    : _str      ( str )
    , _pos      ( (char *) str )
    , _inc      ( 1 )
    , _eof      ( false )
    , _end      ( nullptr )
    , _magic    ( 1 )
{
    if ( _pos )
    {
        if ( IsRom(str) )
        {
            FetchBuffer();
        }
    }
   
    if ( !*_pos )
    {
        _pos = & k_zero;
        _inc = 0;
    }
}


inline
EnumStr::iterator & 
EnumStr::iterator::operator ++ ()
{
    _pos += _inc;
    if ( *_pos )
    {
        if ( _pos == (_buffer + countof(_buffer)) )
        {
            FetchBuffer();
        }
    }
    else
    {
        _pos = & k_zero;
        _inc = 0;
    }

    return *this;
}


inline
char
EnumStr::iterator::operator * () const
{
    return *_pos;
}


inline
bool
EnumStr::iterator::operator == ( const iterator &rh ) const
{
    return( _pos == rh._pos );
}


inline
bool
EnumStr::iterator::operator != ( const iterator &rh ) const
{
    return( _pos != rh._pos );
}



