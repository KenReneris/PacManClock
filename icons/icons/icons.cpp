// icons.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS 


#include "pch.h"
#include <tuple>  
#include <iostream>

#define countof(a)      ( (int) ( sizeof(a)/sizeof(a[0]) ) )
#define countofx(a)     #a, a, countof(a)

using   PCHAR = char const * const;

using   int8    = int8_t;
using   int16   = int16_t;
using   int64   = int64_t;

using   uint8   = uint8_t;
using   uint16  = uint16_t;
using   uint32  = uint32_t;


PCHAR k_dots[] = {      " ",
                        "X",
                        "X",
                        " ",
                        "X",
                        "X",
                        " "   };

PCHAR k_zero[] = {      "  XXX  ",
                        " X  XX ",
                        "XX   XX",
                        "XX   XX",
                        "XX   XX",
                        " XX  X ",
                        "  XXX  "   };

PCHAR k_one[] = {       "  XX  ",
                        " XXX  ",
                        "  XX  ",
                        "  XX  ",
                        "  XX  ",
                        "  XX  ",
                        "XXXXXX"   };

PCHAR k_two[] = {      " XXXXX ",
                       "XX   XX",
                       "    XXX",
                       "  XXXX ",
                       " XXXX  ",
                       "XXX    ",
                       "XXXXXXX"   };

PCHAR k_three[] = {     " XXXXXX",
                        "    XX ",
                        "   XX  ",
                        "  XXXX ",
                        "     XX",
                        "XX   XX",
                        " XXXXX "   };

PCHAR k_four[] = {      "   XXX ",
                        "  XXXX ",
                        " XX XX ",
                        "XX  XX ",
                        "XXXXXXX",
                        "    XX ",
                        "    XX "   };

PCHAR k_five[] = {      "XXXXXX ",
                        "XX     ",
                        "XXXXXX ",
                        "     XX",
                        "     XX",
                        "XX   XX",
                        " XXXXX "   };

PCHAR k_six[] = {       "  XXXX ",
                        " XX    ",
                        "XX     ",
                        "XXXXXX ",
                        "XX   XX",
                        "XX   XX",
                        " XXXXX "   };

PCHAR k_seven[] = {     "XXXXXXX",
                        "XX   XX",
                        "    XX ",
                        "   XX  ",
                        "  XX   ",
                        "  XX   ",
                        " XX    "   };

PCHAR k_eight[] = {     " XXXX  ",
                        "XX   X ",
                        "XXX  X ",
                        " XXXX  ",
                        "X  XXXX",
                        "X    XX",
                        " XXXXX "   };

PCHAR k_nine[] = {      " XXXXX ",
                        "XX   XX",
                        "XX   XX",
                        " XXXXXX",
                        "     XX",
                        "    XX ",
                        " XXXX  "   };


PCHAR k_leftPac0[] = {  "    XXXXX    ",
                        "  XXXXXXXXX  ",
                        " XXXXXXXXXXX ",
                        " XXXXXXXXXXX ",
                        "XXXXXXXXXXXXX",
                        "XXXXXXXXXXXXX",
                        "XXXXXXXXXXXXX",
                        "XXXXXXXXXXXXX",
                        "XXXXXXXXXXXXX",
                        " XXXXXXXXXXX ",
                        " XXXXXXXXXXX ",
                        "  XXXXXXXXX  ",
                        "    XXXXX    "     };

PCHAR k_leftPac1[] = {  "    XXXXX    ",
                        "  XXXXXXXXX  ",
                        " XXXXXXXXXXX ",
                        " XXXXXXXXXXX ",
                        "   XXXXXXXXXX",
                        "      XXXXXXX",
                        "        XXXXX",
                        "      XXXXXXX",
                        "   XXXXXXXXXX",
                        " XXXXXXXXXXX ",
                        " XXXXXXXXXXX ",
                        "  XXXXXXXXX  ",
                        "    XXXXX    "     };

PCHAR k_leftPac2[] = {  "    XXXXX    ",
                        "    XXXXXXX  ",
                        "     XXXXXXX ",
                        "      XXXXXX ",
                        "       XXXXXX",
                        "        XXXXX",
                        "         XXXX",
                        "        XXXXX",
                        "       XXXXXX",
                        "      XXXXXX ",
                        "     XXXXXXX ",
                        "    XXXXXXX  ",
                        "    XXXXX    "     };


PCHAR k_blueGhost0[] ={ "     XXXX     ",
                        "   XXXXXXXX   ",
                        "  XXXXXXXXXX  ",
                        " XXXXXXXXXXXX ",
                        " XXXXXXXXXXXX ",
                        " XXX..XX..XXX ",
                        "XXXX..XX..XXXX",
                        "XXXXXXXXXXXXXX",
                        "XXXXXXXXXXXXXX",
                        "XX..XX..XX..XX",
                        "X.XX..XX..XX.X",
                        "XXXXXXXXXXXXXX",
                        "XXXX XXXX XXXX",
                        " XX   XX   XX "    };

PCHAR k_blueGhost1[] ={ "     XXXX     ",
                        "   XXXXXXXX   ",
                        "  XXXXXXXXXX  ",
                        " XXXXXXXXXXXX ",
                        " XXXXXXXXXXXX ",
                        " XXX..XX..XXX ",
                        "XXXX..XX..XXXX",
                        "XXXXXXXXXXXXXX",
                        "XXXXXXXXXXXXXX",
                        "XX..XX..XX..XX",
                        "X.XX..XX..XX.X",
                        "XXXXXXXXXXXXXX",
                        "XX XXX  XXX XX",
                        "X   XX  XX   X"    };

PCHAR k_ghost0[] = {    "     XXXX     ",
                        "   XXXXXXXX   ",
                        "  XXXXXXXXXX  ",
                        " XXXX**XXX**X ",
                        " XXX****X**** ",
                        " XXX**..X**..X",
                        "XXXX**..X**..X",
                        "XXXXX**XXX**XX",
                        "XXXXXXXXXXXXXX",
                        "XXXXXXXXXXXXXX",
                        "XXXXXXXXXXXXXX",
                        "XXXXXXXXXXXXXX",
                        "XXXX XXXX XXXX",
                        " XX   XX   XX "    };

PCHAR k_ghost1[] = {    "     XXXX     ",
                        "   XXXXXXXX   ",
                        "  XXXXXXXXXX  ",
                        " XXXX**XXX**X ",
                        " XXX****X**** ",
                        " XXX**..X**..X",
                        "XXXX**..X**..X",
                        "XXXXX**XXX**XX",
                        "XXXXXXXXXXXXXX",
                        "XXXXXXXXXXXXXX",
                        "XXXXXXXXXXXXXX",
                        "XXXXXXXXXXXXXX",
                        "XX XXX  XXX XX",
                        "X   XX  XX   X"    };

PCHAR k_ghostEyes[] = { "              ",
                        "              ",
                        "     XX   XX  ",
                        "    X**X X**X ",
                        "   X****X**** ",
                        "   X**..X**..X",
                        "   X**..X**..X",
                        "    X**X X**X ",
                        "     XX   XX  " };


PCHAR k_pacCaught0[]  = {   "             ",              
                            "             ",              
                            " X         X ",              
                            " XX       XX ",              
                            "XXXX     XXXX",              
                            "XXXXX   XXXXX",              
                            "XXXXXX XXXXXX",              
                            "XXXXXXXXXXXXX",              
                            "XXXXXXXXXXXXX",              
                            " XXXXXXXXXXX ",              
                            " XXXXXXXXXXX ",              
                            "  XXXXXXXXX  ",              
                            "    XXXXX    " };        

PCHAR k_pacCaught1[]  = {   "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "XX         XX",              
                            "XXXX     XXXX",              
                            "XXXXXX XXXXXX",              
                            "XXXXXXXXXXXXX",              
                            "XXXXXXXXXXXXX",              
                            " XXXXXXXXXXX ",              
                            " XXXXXXXXXXX ",              
                            "  XXXXXXXXX  ",              
                            "    XX XX    " };  

PCHAR k_pacCaught2[]  = {   "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "XXXX     XXXX",              
                            "XXXXXXXXXXXXX",              
                            "XXXXXXXXXXXXX",              
                            " XXXXXXXXXXX ",              
                            " XXXXXXXXXXX ",              
                            "  XXXXXXXXX  ",              
                            "    XX XX    " };        

PCHAR k_pacCaught3[]  = {   "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "   XXXXXXX   ",              
                            "XXXXXXXXXXXXX",              
                            " XXXXXXXXXXX ",              
                            " XXXXXXXXXXX ",              
                            "  XXXXXXXXX  ",              
                            "    XX XX    " }; 


PCHAR k_pacCaught4[]  = {   "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "     XXX     ",              
                            "   XXXXXXX   ",              
                            " XXXXXXXXXXX ",              
                            " XXXXXXXXXXX ",              
                            "  XXXXXXXXX  ",              
                            "    XX XX    " }; 

PCHAR k_pacCaught5[]  = {   "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "      X      ",              
                            "     XXX     ",              
                            "   XXXXXXX   ",              
                            "  XXXXXXXXX  ",              
                            "  XXXX XXXX  ",              
                            "    X   X    " }; 

PCHAR k_pacCaught6[]  = {   "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "      X      ",              
                            "     XXX     ",              
                            "    XXXXX    ",              
                            "   XXXXXXX   ",              
                            "  XXXX XXXX  ",              
                            "    XX XX    " }; 

PCHAR k_pacCaught7[]  = {   "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "             ",              
                            "      X      ",              
                            "      X      ",              
                            "     XXX     ",              
                            "     XXX     ",              
                            "    XX XX    ",              
                            "    XX XX    " }; 

PCHAR k_pacCaught8[]  = {   "       ",              
                            "       ",              
                            "       ",              
                            "       ",              
                            "       ",              
                            "       ",              
                            "       ",              
                            "      X",              
                            "      X",              
                            "      X",              
                            "      X",              
                            "       ",              
                            "       " }; 

PCHAR k_pacCaught9[] = { " " };

PCHAR k_pacCaught10[] = {   "             ",
                            "   X      X  ",
                            "    X    X   ",
                            " X         X ",
                            "  XX  .  XX  ",
                            "     . .     ",
                            "XX  . . .  XX",
                            "     . .     ",
                            "  XX  .  XX  ",
                            " X         X ",
                            "    X   X    ",
                            "   X     X   " };
                        //  "             "     };

PCHAR k_invaderA1[] = {     "  X     X  ",
                            "X  X   X  X",
                            "X XXXXXXX X",
                            "XXX XXX XXX",
                            "XXXXXXXXXXX",
                            " XXXXXXXXX ",
                            "  X     X  ",
                            " X       X " };

PCHAR k_invaderA2[] = {     "  X     X  ",
                            "   X   X   ",
                            "  XXXXXXX  ",
                            " XX XXX XX ",
                            "XXXXXXXXXXX",
                            "X XXXXXXX X",
                            "X X     X X",
                            "   XX XX   " };

PCHAR k_invaderB1[] = {     "   XX   ",
                            "  XXXX  ",
                            " XXXXXX ",
                            "XX XX XX"   ,
                            "XXXXXXXX",
                            " X XX X ",
                            "X      X",
                            " X    X " };

PCHAR k_invaderB2[] = {     "   XX   ",
                            "  XXXX  ",
                            " XXXXXX ",
                            "XX XX XX"   ,
                            "XXXXXXXX",
                            "  X  X  ",
                            " X XX X ",
                            "X X  X X" };

                          // 01234567890123456
PCHAR k_invaderUfo[] = {    "     XXXXXXX     ",
                            "   XXXXXXXXXXX   ",
                            "  XXXXXXXXXXXXX  ",
                            " XXX XX XX XX XX ",
                            "XXXXXXXXXXXXXXXXX",
                            "  XXX   XX  XXX  ",
                            "   X         X   " };


PCHAR k_invaderShooter[] = {    "     X     ",
                                "    XXX    ",
                                " XXXXXXXXX ",
                                "XXXXXXXXXXX",
                                "XXXXXXXXXXX" } ;


PCHAR k_robotronShooter0[] = {  " XXXX",
                                "XXXXX",
                                "XXXXX",
                                " XXX ",
                                "  M  ",
                                " MMM ",
                                " M*M ",
                                " M*M ",
                                "  *  ",
                                "  M  ",
                                "  MM " };

PCHAR k_robotronShooter1[] = {  " XXXX",
                                "XXXXX",
                                "XXXXX",
                                " XXX ",
                                "  M  ",
                                " MM  ",
                                "*MM**",
                                "*MM  ",
                                " M M ",
                                " M M ",
                                " M MM" };

PCHAR k_robotronShooter2[] = {  " XXXX",
                                "XXXXX",
                                "XXXXX",
                                " XXX ",
                                "  M  ",
                                " MM  ",
                                "*M***",
                                "*MM  ",
                                " M M ",
                                " M M ",
                                " M MM" };


PCHAR k_defenderTerrain[] = { 
            //            1         2         3         4         5         6
            //   1234567890123456789012345678901234567890123456789012345678901234
    /* 00 */    "                                                                ",
    /* 01 */    "                                                                ",
    /* 02 */    "                                                                ",
    /* 03 */    "                                                                ",
    /* 04 */    "                                                                ",
    /* 05 */    "                                                                ",
    /* 06 */    "                                                                ",
    /* 07 */    "                                                                ",
    /* 08 */    "                                                                ",
    /* 09 */    "                                                                ",
    /* 10 */    "                                        *                       ",
    /* 11 */    "                *                      * **                     ",
    /* 12 */    "              ** *     **           ***    *               *    ",
    /* 13 */    "   *         *    *   *  *         *        *     **      * *   ",
    /* 14 */    "  * **  *   *      ***    *    ****          **  *  **   *   *  ",
    /* 15 */    "**    ** ***               ****                **     ***     **"      };


void
Terrain( PCHAR lines[] )
{

    uint8   results[ 32 ];
    memset( results, 0, sizeof( results ) );

    for ( int col=0; col < 64; ++col )
    {
        for ( int row=0; row < 16; ++row )
        {
            if ( lines[row][col] != ' ' )
            {
                results[ col/2 ] |= ( row << (col%2 ? 4 : 0 ));
                break;
            }
        }
    }

    printf( "const uint8 PROGMEM k_terrain[] = { " );
    for (uint8 r : results)
    {
        printf( "0x%02x, ", r );
    }
    printf( "};\n" );
}

void
test( char  const *symbol, PCHAR lines[], size_t maxLines )
{
    size_t      lineWidth = strlen( lines[0] );


    for (size_t index = 0; index < maxLines; ++index)
    {
        if (strlen( lines[ index ] ) != lineWidth)
        {
            printf( "***** FAIL1\n" );
        }
    }

    printf( "%s. %d x %d\n", symbol, lineWidth, maxLines );
}


void
Monocrhome( char const *symbol, PCHAR lines[], size_t maxLines, bool reverse = false )
{
    size_t      lineWidth   = strlen( lines[ 0 ] );
    size_t      lineIndex   = 0;
    int         offsetY     = 0;

    printf( "const uint8 PROGMEM %s[] = { ", symbol );       
    
    for (; lineIndex < maxLines; ++lineIndex)
    {
        char const    * line    = lines[ lineIndex ];
        bool            hasChar = false;

        for ( char const *p = line; *p; ++p )
        {
            if ( *p != ' ' )
            {
                hasChar = true;
            }
        }

        if ( hasChar )
        {
            break;
        }

        offsetY += 1;
    }


    printf( "0, %d", offsetY );        // header

    printf( ", %d,%d", lineWidth, maxLines-offsetY );
    for ( ; lineIndex < maxLines; ++lineIndex)
    {
        char const    * line = lines[ lineIndex ];
        int             bits = 0;
        char            buffer[ 128 ];

        if ( lineWidth != strlen( line ) )
        {
            printf( "***** FAIL1\n" );
        }

        if ( reverse )
        {
            for ( size_t index=0; index < lineWidth; ++index )
            {
                buffer[ index ] = line[ lineWidth - index - 1 ];
            }
            buffer[ lineWidth ] = 0;
            line = buffer;
        }

        for (size_t index = 0; index < lineWidth; ++index)
        {
            if (line[ index ] != ' ')
            {
                bits |= (1 << index);
            }
        }

        printf( ", 0x%02x", bits & 0xFF );
        if ( lineWidth > 8 )
        {
            printf( ", 0x%02x", (bits >> 8) & 0xFF );
        }

        if ( lineWidth > 16 )
        {
            printf( ", 0x%02x", (bits >> 16) & 0xFF );
        }
    }
    printf( " };\n" );
}



void
Color4( char const *symbol, PCHAR lines[], size_t maxLines )
{
    size_t      lineWidth   = strlen( lines[ 0 ] );
    int const   bitWidth    = 2;

    printf( "const uint8 PROGMEM %s[] = { ", symbol );
    printf( "0, 0" );        // header

    printf( ", %d,%d", lineWidth, maxLines );
    for ( size_t lineIndex = 0; lineIndex < maxLines; ++lineIndex)
    {
        char const    * line = lines[ lineIndex ];
        unsigned int    bits = 0;

        if ( lineWidth != strlen( line ) )
        {
            printf( "***** FAIL1\n" );
        }

        for (size_t index = 0; index < lineWidth; ++index)
        {
            int      color = 0;

            switch (line[ index ] )
            {
            case ' ':
                color = 0;
                break;

            case 'X':
                color = 1;
                break;

            case '.':
            case 'M':
                color = 2;
                break;

            case '*':
                color = 3;
                break;

            default:
                printf( "***** FAIL2\n" );
                break;
            }

            bits |= (color << (index*2) );
        }

        printf( ", 0x%02x", bits & 0xFF );
        if ( lineWidth*bitWidth > 8 )
        {
            printf( ", 0x%02x", (bits >> 8) & 0xFF );
        }
        if ( lineWidth*bitWidth > 16 )
        {
            printf( ", 0x%02x", (bits >> 16) & 0xFF );
        }
        if ( lineWidth*bitWidth > 24 )
        {
            printf( ", 0x%02x", (bits >> 24) & 0xFF );
        }
    }
    printf( " };\n" );
}



struct BmpHeader
{
    uint16      FileType;     /* File type, always 4D42h ("BM") */
    uint16      FileSize;     /* Size of the file in bytes */
    uint16      FileSize1;
    uint16      Reserved1;    /* Always 0 */
    uint16      Reserved2;    /* Always 0 */
    uint16      BitmapOffset; /* Starting position of image data in bytes */
    uint16      BitmapOffset1;

    uint16      Size;
    uint16      Size1;
    uint16      Width;
    uint16      Width1;
    uint16      Height;
    uint16      Height1;
    uint16      Planes;
    uint16      BitsPerPixel;
    uint16      Compression0;
    uint16      Compression1;
    uint16      SizeOfBitmap;
    uint16      SizeOfBitmap1;
    uint16      HorzRes;
    uint16      HorzRes1;
    uint16      VertRes;
    uint16      VertRes1;
    uint16      ColorsUsed;
    uint16      ColorsUsed1;
    uint16      ColorsImport[ 2 ];

};

struct BmpRGB
{
    uint8   b;
    uint8   g;
    uint8   r;
    uint8   a;
};


void 
Assert( bool cond )
{
    if (!cond)
    {
        printf( "FAILED\n" );
    }
}

uint8   bmColorsUsed;
BmpRGB  bmColors[ 16 ];         // index is bm color
uint8   bmColorMapF[ 16 ];      // bm-color-index to remap-index
uint8   bmColorMapR[ 16 ];      // remap-index to bm-color-index

bool
RgbIsLess( BmpRGB lh, BmpRGB rh )
{
    return std::tie( lh.r, lh.g, lh.b ) < std::tie( rh.r, rh.g, rh.b );
}

void
RemapColors( char const *filename )
{
    FILE       *fp = fopen( filename, "r" );
    BmpHeader   hdr = {};
    uint32      stride          = 0;
    uint8       line[ 256 ];

    bmColorsUsed = 0;
    memset( bmColors, 0, sizeof( bmColors ) );
    memset( bmColorMapF, 0xFF, sizeof( bmColorMapF ) );
    memset( bmColorMapR, 0xFF, sizeof( bmColorMapR ) );

    fread( &hdr, sizeof( hdr ), 1, fp );
    Assert( hdr.BitsPerPixel == 4 );
    fread( bmColors, sizeof(BmpRGB), hdr.ColorsUsed, fp );

    stride = (hdr.Width + 1) / 2;
    stride = (stride + 4-1) & ~3;
    Assert( stride < sizeof( line ) );

    printf( "%dx%d %d\n", hdr.Width, hdr.Height, stride );
    for ( int y =0; y < hdr.Height; ++y )
    {
        int     offset = hdr.BitmapOffset + (hdr.Height-y-1) * stride;

        fseek( fp, offset, 0 );
        fread( line, stride, 1, fp );

        for ( int x = 0; x < hdr.Width; ++x )
        {
            uint8   c = line[ x / 2 ];
        
            if ( !(x & 1) )
            {
                c >>= 4;
            }
            c &= 0xF;
        
            if ( bmColorMapF[c] == 0xFF )
            {
                bmColorMapF[ c ] = bmColorsUsed;
                bmColorMapR[ bmColorsUsed ] = c;
                bmColorsUsed += 1;
            }

            if (c)
            {
                printf( "%d ", c );
            }
            else
            {
                printf( ". " );
            }
        }
        printf( "\n" );
    }

    printf( "\n" );

    // sort 
    for( uint8 index0=0; index0 < bmColorsUsed; ++index0 )
    {
        for( uint8 index1=0; index1 < bmColorsUsed; ++index1 )
        {
            uint8   rgb0Index = bmColorMapR[ index0 ];
            uint8   rgb1Index = bmColorMapR[ index1 ];

            if ( RgbIsLess( bmColors[ rgb0Index ], bmColors[ rgb1Index ] ) )
            {
                std::swap( bmColorMapR[ index0 ], bmColorMapR[ index1 ] );
            }
        }
    }

    for( uint8 index=0; index < bmColorsUsed; ++index )
    {
        bmColorMapF[ bmColorMapR[ index ] ] = index;
    }

    fclose( fp );
    printf( "\n" );
}


void
Bitmap( char const *symbol, char const *filename )
{
    RemapColors( filename );

    FILE       *fp  = fopen( filename, "r" );
    BmpHeader   hdr             = {};
    uint32      stride          = 0;
    char const *sep             = "";
    BmpRGB      junk[ 16 ];
    uint8       line[ 256 ];

    fread( &hdr, sizeof( hdr ), 1, fp );
    Assert( hdr.BitsPerPixel == 4 );
    fread( junk, sizeof(BmpRGB), hdr.ColorsUsed, fp );

    stride = (hdr.Width + 1) / 2;
    stride = (stride + 4-1) & ~3;
    Assert( stride < sizeof( line ) );

    printf( "const uint8 PROGMEM %s[] = {\n", symbol );
    printf( "    0, 0,0,0, " );                 // sequence, flags, align
    printf( "%d, ", bmColorsUsed-1 );
    printf( "0, " );                            // offsetY
    printf( "%d, %d,", hdr.Width, hdr.Height );
    printf( "\n    " );
    for (int index = 1; index < bmColorsUsed; ++index)
    {
        BmpRGB    & rgb = bmColors[ bmColorMapR[index] ];

        printf( "0x%02x,0x%02x,0x%02x, ", rgb.r, rgb.g, rgb.b );
    }

    sep = "";
    for ( int y =0; y < hdr.Height; ++y )
    {
        int     offset = hdr.BitmapOffset + (hdr.Height-y-1) * stride;

        fseek( fp, offset, 0 );
        fread( line, stride, 1, fp );

        printf( "%s\n        ", sep );
        sep = "";

        for ( int x = 0; x < (hdr.Width+1)/2; ++x )
        {
            uint8   cc = line[ x ];
            uint8   c  = (cc >> 4) | (cc << 4);
            
            c = (bmColorMapF[ (c >> 4) & 0xF ] << 4) | bmColorMapF[ c & 0xF ];

            printf( "%s0x%02x", sep, c );
            sep = ", ";
        }
    }

    printf( " };\n" );

    fclose( fp );
    printf( "\n" );
}


void
Bitmap2( char const *symbol, char const *filename )
{
    RemapColors( filename );

    FILE       *fp  = fopen( filename, "r" );
    BmpHeader   hdr             = {};
    uint32      stride          = 0;
    char const *sep             = "";
    BmpRGB      junk[ 16 ];
    uint8       line[ 256 ];

    fread( &hdr, sizeof( hdr ), 1, fp );
    Assert( hdr.BitsPerPixel == 4 );
    fread( junk, sizeof(BmpRGB), hdr.ColorsUsed, fp );

    stride = (hdr.Width + 1) / 2;
    stride = (stride + 4-1) & ~3;
    Assert( stride < sizeof( line ) );

    printf( "const uint8 PROGMEM %s[] = {\n", symbol );
    printf( "    0, 0,0,0, " );                 // sequence, flags, align
    printf( "%d, ", bmColorsUsed-1 );
    printf( "0, " );                            // offsetY
    printf( "%d, %d,", hdr.Width, hdr.Height );
    printf( "\n    " );
    for (int index = 1; index < bmColorsUsed; ++index)
    {
        BmpRGB    & rgb = bmColors[ bmColorMapR[index] ];

        printf( "0x%02x,0x%02x,0x%02x, ", rgb.r, rgb.g, rgb.b );
    }

    sep = "";
    for ( int y =0; y < hdr.Height; ++y )
    {
        int     offset  = hdr.BitmapOffset + (hdr.Height-y-1) * stride;
        uint8   pixel   = 0;
        uint8   pcnt    = 0;

        fseek( fp, offset, 0 );
        fread( line, stride, 1, fp );

        printf( "%s\n        ", sep );
        sep = "";

        for ( int x = 0; x < (hdr.Width+1)/2; ++x )
        {
            uint8   cc = line[ x ];
            uint8   c  = (cc >> 4) | (cc << 4);
            
            pixel = ( pixel >> 2 ) | ( bmColorMapF[ (c >> 0) & 0xF ] << 6 );
            pcnt += 2;

            pixel = ( pixel >> 2 ) | ( bmColorMapF[ (c >> 4) & 0xF ] << 6 );
            pcnt += 2;

            if ( pcnt >= 8 )
            {
                printf( "%s0x%02x", sep, pixel );
                sep = ", ";
                pixel = 0;
                pcnt = 0;
            }
        }

        if ( pcnt )
        {
            while ( pcnt < 8 )
            {
                pixel >>= 2;
                pcnt += 2;
            }
            printf( "%s0x%02x", sep, pixel );
        }
    }

    printf( " };\n" );

    fclose( fp );
    printf( "\n" );
}


int 
main()
{
    Terrain( k_defenderTerrain );

    Bitmap( "k_speeder", "defSpeeder.bmp" );

    Bitmap( "k_defUfo0", "defUfo0.bmp" );
    Bitmap( "k_defUfo1", "defUfo1.bmp" );
    Bitmap( "k_defUfo2", "defUfo2.bmp" );


    Bitmap2( "k_mario0", "mario0.bmp" );     // todo: these could be 2 bit encoded
    Bitmap2( "k_mario1", "mario1.bmp" );
    Bitmap2( "k_mario2", "mario2.bmp" );

    Bitmap2( "k_robo4000", "robo4000.bmp" );
    Bitmap( "k_robotronLady0", "roboLady0.bmp" );
    Bitmap( "k_robotronLady1", "roboLady1.bmp" );
    Bitmap( "k_robotronLady2", "roboLady1.bmp" );

    Bitmap( "k_robotronAttacker0", "roboAttacker0.bmp" );
    Bitmap( "k_robotronAttacker1", "roboAttacker1.bmp" );


    Monocrhome( countofx( k_dots ) );
    Monocrhome( countofx( k_zero ) );
    Monocrhome( countofx( k_one ) );
    Monocrhome( countofx( k_two ) );
    Monocrhome( countofx( k_three ) );
    Monocrhome( countofx( k_four ) );
    Monocrhome( countofx( k_five ) );
    Monocrhome( countofx( k_six ) );
    Monocrhome( countofx( k_seven ) );
    Monocrhome( countofx( k_eight ) );
    Monocrhome( countofx( k_nine ) );

    Monocrhome( countofx( k_leftPac0 ) );
    Monocrhome( countofx( k_leftPac1 ) );
    Monocrhome( countofx( k_leftPac2 ) );

  //Monocrhome( "k_rightPac0", k_leftPac0, countof( k_leftPac0 ), true );   - round
  //Monocrhome( "k_rightPac1", k_leftPac1, countof( k_leftPac1 ), true );
  //Monocrhome( "k_rightPac2", k_leftPac2, countof( k_leftPac2 ), true );

    Color4( countofx( k_blueGhost0 ) );
    Color4( countofx( k_blueGhost1 ) );

    Color4( countofx( k_ghost0 ) );
    Color4( countofx( k_ghost1 ) );

    Color4( countofx( k_ghostEyes ) );

    Monocrhome( countofx( k_pacCaught0 ) );
    Monocrhome( countofx( k_pacCaught1 ) );
    Monocrhome( countofx( k_pacCaught2 ) );
    Monocrhome( countofx( k_pacCaught3 ) );
    Monocrhome( countofx( k_pacCaught4 ) );
    Monocrhome( countofx( k_pacCaught5 ) );
    Monocrhome( countofx( k_pacCaught6 ) );
    Monocrhome( countofx( k_pacCaught7 ) );
    Monocrhome( countofx( k_pacCaught8 ) );
    Monocrhome( countofx( k_pacCaught9 ) );
    Color4( countofx( k_pacCaught10 ) );

    Monocrhome( countofx( k_invaderA1 ) );
    Monocrhome( countofx( k_invaderA2 ) );
    Monocrhome( countofx( k_invaderB1 ) );
    Monocrhome( countofx( k_invaderB2 ) );
    Monocrhome( countofx( k_invaderUfo ) );
    Monocrhome( countofx( k_invaderShooter ) );

    Color4( countofx(k_robotronShooter0) );
    Color4( countofx(k_robotronShooter1) );
    Color4( countofx(k_robotronShooter2) );


    printf( "done\n" );
}

