/*
 * Columns
 *  Output column/table data
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"
#include "Columns.h"


Columns::Columns( uint indent, char const *fill, char const sep, std::function<void( char const * )> const &htmlTable )
    : _fill         ( fill )
    , _sep          ( sep )
    , _pass         ( 0 )
    , _indent       ( indent )
    , _htmlRowEnd   ( F("") )
    , _isHtml       ( !!htmlTable )
    , _row          ( 0 )
    , _buffer       ( htmlTable ? htmlTable : OutStr )
{
    memset( _width, 0, sizeof( _width ) );
    if ( _isHtml )
    {
        _buffer.Add( F("<div style='margin-left: 10px'><table border='1'>") );
    }
}


Columns::~Columns()
{
    if ( _isHtml )
    {
        _buffer.Add( _htmlRowEnd );
        _buffer.Add( F("</table></div><br><br>") );
    }
    else
    {
        NewLine();
        NewLine();
    }
}


void
Columns::SetPass( uint pass )
{
    _pass = pass;
}


void        
Columns::Row( FSTR const *str )
{
    if ( _pass )
    {
        _buffer.Add( str );
    }
}


//void   
//Columns::Row2( char const *c0, Fill fill, FSTR const * c1 );
//{
//    Row2( (FSTR const *) c0, fill, c1 );
//}



void  
Columns::Row2( char const *c0, Fill fill, FSTR const * c1 )
{
    Row2( (FSTR const *) c0, fill, c1 );
}


void
Columns::Row2( FSTR const *c0, Fill fill, String const & str )
{
    Row2( c0, fill, (FSTR const *) str.c_str() );
}


void       
Columns::Row2( FSTR const *c0, Fill fill, FSTR const *c1 )
{
    Cell( 0, Fill::Right, c0 );
    Cell( 1, fill,        c1 );
}


void
Columns::Row2( FSTR const *c0, FSTR const *c1 )
{
    Row2( c0, Fill::Left, c1 );
}


//void
//Columns::Row2( char const *c0, FSTR const *c1 )
//{
//    Row2( (FSTR const *) c0, Fill::Left, c1 );
//}



void
Columns::Row2( FSTR const *c0, char const *c1 )
{
    Row2( c0, Fill::Left, (FSTR const *) c1 );
}


void       
Columns::Row2( FSTR const *c0, String const &c1 )
{
    Row2( c0, Fill::Left, (FSTR const *) c1.c_str() );
}


void
Columns::Row2( FSTR const *c0, uint c1 )
{
    char    buffer[ 32 ];

    SPrintF( buffer, "%d", c1 );
    Row2( c0, Fill::Left, (FSTR const *) buffer );
}


void
Columns::Row2Flag( FSTR const *c0, bool c1 )
{
    Row2( c0, Fill::Left, c1 ? F("true") : F("false") );
}


// void       
// Columns::Row3a( FSTR const *c0, char const *link )
// {
//     Row3a( c0, "", link );
// }
// 
// 
// void
// Columns::Row3a( FSTR const *c0, char const *c1, char const *link )
// {
//     Cell ( 0, Fill::Right, c0 );
//     Cell ( 1, Fill::Right, c1 );
//     Cella( 2, link );
// }
// 
// 
// void       
// Columns::Cella( uint col, char const *link )
// {
//     char const  * desc = link;
// 
//     if ( _isHtml )
//     {
//         Cell( col, Fill::None, PrintF(F("<a href='https://%s'>%s</a>"), link, link).c_str() );
//     }
//     else
//     {
//         Cell( col, Fill::None, link );
//     }
// }


void
Columns::Cell( uint col, Fill fill, FSTR const *txt )
{
    // EnumStr             txt( utxt );
    // EnumStr::iterator   itTxt       = txt.begin();

    const uint          width       = EnumStr( txt ).begin().Length();       //  strlen( txt );
    char                fillChar    = _fill[ col ];
   
    yield();
    if ( _pass )
    {
        _row += 1;       
        if ( col == 0 )
        {
            if ( _row != 1 )
            {
                NewLine();
            }

            if ( !_isHtml )
            {
                _buffer.Fill( ' ', _indent );
            }
        }

        if ( _isHtml )
        {
            _buffer.AddF( F("<td>%s</td>"), txt );
        }
        else
        {
            if ( fill == Fill::Left )
            {
                _buffer.Fill( fillChar, _width[col] - width );
            }

            _buffer.Add( txt );

            // if there's a next column
            if ( (_width[col+1]) && (fillChar) )
            {
                if ( fill == Fill::Right )
                {
                    _buffer.Fill( fillChar, _width[col] - width );
                }

                _buffer.AddF( F("%c%c "), fillChar, _sep );
            }
        }
    }
    else
    {
        if ( width > _width[col] )
        {
            _width[col] = width;
        }
    }
}



void
Columns::NewLine()
{
    if ( _isHtml )
    {
        _buffer.Add( _htmlRowEnd );
        _buffer.Add( F("<tr>") );
        _htmlRowEnd = F("</tr>");
    }
    else
    {
        _buffer.Add( "\n" );
    }
}


