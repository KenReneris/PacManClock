/*
 * Debug.cpp
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"

void 
Assert( bool f, FSTR const *msg )
{
    if ( !f )
    {
        Assert( msg );
    }
}


void 
Assert( FSTR const *msg )
{
    OutStr( "\n" );
    OutStr( "ASSERT: " );
    EnumStr( msg ).ForEachChunk(
        []( char const *str, uint len )
        {
            OutStr( str );
        } );
    OutStr( "\n" );
    system_restart();
    for (; ; );
}
