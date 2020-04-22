/*
 * EnumStr.cpp
 *  Misc string functions
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

#include "platform.h"

char const      EnumStr::k_zero = 0;


void
EnumStr::iterator::FetchBuffer()
{
    if ( !_eof )
    {
        _pos = _buffer;
        for( _end = _buffer; _end != (_buffer + countof(_buffer)); )
        {
            uint32  w = pgm_read_dword_far( _str );
            _str += 4;

            for ( int8 index=0; index < 4; ++index )
            {
                *_end = (w & 0xFF);
                if ( !*_end )
                {
                    _eof = true;
                    if ( !*_pos )
                    {
                        _pos = &k_zero;
                        _inc = 0;
                    }
                    return;
                }

                _end += 1;
                w >>= 8;
            }
        }
    }
}


void
EnumStr::iterator::ForEachChunk( const function<void( char const *str, uint len )> &callback )
{
    _magic = 0;
    callback( _pos, _end - _pos );

    while( !_eof )
    {
        FetchBuffer();
        callback( _pos, _end - _pos );
    }
}


uint16
EnumStr::iterator::Length() const
{
    uint16      length = 0;

    if ( !IsRom(_str) )
    {
        length = strlen( _str );
    }
    else
    {
        length = (_end - _pos);
        if ( !_eof )
        {
            char const    * str = _str;
            bool            eof = false;

            while( !eof )
            {
                uint32  w = pgm_read_dword_far( str );
                str += 4;

                for ( int8 index=0; index < 4; ++index )
                {
                    if ( !(w & 0xFF) )
                    {
                        eof = true;
                        break;
                    }

                    length += 1;
                    w >>= 8;
                }
            }
        }
    }

    return length;
}

