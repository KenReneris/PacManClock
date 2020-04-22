/*
 * Bitmap.h
 *  Our internally encoded images
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */



enum DrawResult : uint8
{
    None        = 0,
    Drawn       = 0x01,         // sprite was drawn (else sprite was totally off-screen)
    Collision   = 0x02,         // collided on draw zbuffer
};


class BitmapBase
{
public:
    enum Header : uint8
    {
        Header_SequenceSpeed,               // in increments of movement.
        Header_Flags,
        Header_align2,
        Header_align3,

        Header_PalletCount,                 // these 4 items are assumed to be in this order in bitmap.cpp
        Header_OffsetY,                     // in the low bits. 
        Header_SizeX,                       // assumed X&Y are last by Digit.cpp
        Header_SizeY,           

        Header_Max
        // 3 byte color codes start here. for Header_PalletCount times
        // then the pixel bits
    };

    enum Flag : uint8
    {
        Flag_DontFlipX              = 0x01,
        Flag_SequenceEnd            = 0x02,
        Flag_ClockEraseCollisions   = 0x04,         // sets global
    };

public:
    BitmapBase( uint16 movementSpeed );
    void Init( uint8 const *bitmap, uint8 bitSize, bool flipx );

    uint8                   Width() const;
    Point                   Size() const;
    uint16 const          * Pallet() const;

    DrawResult              Draw( Point pos, DrawMode drawMode, uint16 const *pallet ) const;
    virtual DrawResult      Draw( Point pos, DrawMode drawMode, uint16 const *pallet ) = 0;
    virtual DrawResult      DrawRow( Point pos, int16 row, DrawMode drawMode ) const = 0;

    uint16                  MovementSpeed() const;
    uint8                   SequenceSpeed() const;
    uint8                   Flags() const;
    
    void                    SetMovementSpeed( uint16 speed );

protected:
    uint16                  _movementSpeed;
    uint8                   _width;
    uint8                   _height;
    uint8                   _offsetY : 4;
    uint8                   _seqSpeed : 4;
    uint8                   _flags;
    uint16 const          * _pallet;
    uint8 const           * _pixels;
};


template<int BITS>
class Bitmap
    : public BitmapBase
{
public:
    Bitmap( uint8 const *bitmap );
    Bitmap( uint8 const *bitmap, bool flipx );
    Bitmap( uint16 movementSpeed, uint8 const *bitmap );
    Bitmap( uint16 movementSpeed, uint8 const *bitmap, bool flipx );

    DrawResult      Draw( Point pos, DrawMode drawMode, uint16 const *pallet ) override;
    DrawResult      DrawRow( Point pos, int16 row, DrawMode drawMode ) const override;
};


template<int BITS>
Bitmap<BITS>::Bitmap( uint16 movementSpeed, uint8 const *bitmap, bool flipx )
    : BitmapBase    ( movementSpeed )
{
    Init( bitmap, BITS, flipx );
}


template<int BITS>
Bitmap<BITS>::Bitmap( uint16 movementSpeed, uint8 const *bitmap )
    : Bitmap        ( movementSpeed, const_cast<uint8 *>(bitmap), false )
{
}


template<int BITS>
Bitmap<BITS>::Bitmap( uint8 const *bitmap, bool flipx )
    : Bitmap        ( 0, const_cast<uint8 *>(bitmap), flipx )
{
}


template<int BITS>
Bitmap<BITS>::Bitmap( uint8 const *bitmap )
    : Bitmap        ( 0, const_cast<uint8 *>(bitmap), false )
{
}


template<int BITS>
DrawResult
Bitmap<BITS>::Draw( Point pos, DrawMode drawMode, uint16 const *pallet )
{
    pos.y += _offsetY;     

    Point           size            = Point( _width, _height );  
    uint8 const     stride          = ( (BITS*size.x) + 7 ) / 8;
    Point           max             = size;
    uint8 const   * row             = _pixels;  
    DrawResult      result          = DrawResult::None;
    Point           offset;

    if ( !pallet )
    {
        pallet = _pallet;
    }

    // clip top
    if ( pos.y < 0 )
    {
        offset.y = -pos.y;
        pos.y = 0;
    }
    
    // clip bottom
    if ( pos.y+max.y > PANEL_HEIGHT )
    {
        max.y = PANEL_HEIGHT - pos.y;
    }
  
    // clip left (note assumes maxWidth is < 64)
    if ( pos.x < 0 )
    {
        if ( pos.x + max.x < 0 )
        {
            max.x = 0;
        }
        else
        {
            offset.x = -pos.x;
        }
    }
  
    // clip right
    if ( pos.x + max.x >= PANEL_WIDTH ) 
    {
        max.x = PANEL_WIDTH - pos.x;
    }

    if ( (max.x > 0) && (offset.y < max.y) )
    {
        bool        collision   = false;
        Point       index;

        result = DrawResult( uint(result) | Drawn );

        // adjust our startinf row for the initial offset
        row += offset.y * stride;

        for ( index.y=offset.y; index.y < max.y; index.y += 1 )
        {
            for ( index.x=offset.x; index.x < max.x; index.x += 1 )
            {
                uint8 const     pixelPerByte    = ( 8 / BITS );
                uint8 const   * cbm             = & row[ index.x / pixelPerByte ];
                uint8 const     bmShift         = ( BITS * (index.x % pixelPerByte) );
                uint8         * bm              = const_cast<uint8 *>( cbm );
                uint8 const     bits            = (*bm) >> bmShift;
                uint8 const     color           = bits & ( (1 << BITS) - 1 );
                bool            collision;

                if ( color )
                {
                    uint16 const    x           = pos.x + index.x;
                    uint16 const    y           = pos.y;
                    uint16 const    pixelIndex  = ( y * PANEL_WIDTH + x );
                    uint8 const     byteIndex   = pixelIndex / 8;
                    uint8 const     bit         = 1 << ( pixelIndex % 8 );

                    if ( !(g_zbuffer[ byteIndex ] & bit) )
                    {
                        if ( drawMode & Draw_Z )
                        {
                            g_zbuffer[ byteIndex ] |= bit;
                        }
                        g_display.drawPixel( x, y, pallet[ color-1 ] );
                    }
                    else
                    {
                        if ( drawMode & Draw_OnTop )
                        {
                            g_display.drawPixel( x, y, pallet[ color-1 ] );
                        }

                        result = DrawResult( uint( result ) | Collision );
                        if ( drawMode & Draw_EarseCollisions )
                        {
                            *bm &= ~(color << bmShift);
                        }
                    }
                }
            }

            pos.y += 1;
            row += stride;
        }
    }

    return result;
}




template<int BITS>
DrawResult
Bitmap<BITS>::DrawRow( Point pos, int16 drawRow, DrawMode drawMode ) const
{
    pos.y += _offsetY;
    // pos.y += drawRow;

    uint16 const  * pallet          = _pallet;
    Point           size            = Point( _width, _height );  
    uint8 const     stride          = ( (BITS*size.x) + 7 ) / 8;
    Point           max             = size;
    uint8 const   * row             = _pixels;  
    DrawResult      result          = DrawResult::None;
    Point           offset;

    // clip left (note assumes maxWidth is < 64)
    if ( pos.x < 0 )
    {
        if ( pos.x + max.x < 0 )
        {
            max.x = 0;
        }
        else
        {
            offset.x = -pos.x;
        }
    }
  
    // clip right
    if ( pos.x + max.x >= PANEL_WIDTH ) 
    {
        max.x = PANEL_WIDTH - pos.x;
    }

    if ( (max.x > 0) && (pos.y >= 0) && (pos.y < PANEL_HEIGHT) )     // (offset.y < max.y) )
    {
        bool        collision   = false;
        Point       index;

        result = DrawResult( uint( result ) | Drawn );

        row += drawRow * stride;
        index.y = pos.y;

        for ( index.x=offset.x; index.x < max.x; index.x += 1 )
        {
            uint8 const     pixelPerByte    = ( 8 / BITS );
            uint8 const   * cbm             = & row[ index.x / pixelPerByte ];
            uint8 const     bmShift         = ( BITS * (index.x % pixelPerByte) );
            uint8         * bm              = const_cast<uint8 *>( cbm );
            uint8 const     bits            = (*bm) >> bmShift;
            uint8 const     color           = bits & ( (1 << BITS) - 1 );
            bool            collision;

            if ( color )
            {
                uint16 const    x           = pos.x + index.x;
                uint16 const    y           = pos.y;
                uint16 const    pixelIndex  = ( y * PANEL_WIDTH + x );
                uint8 const     byteIndex   = pixelIndex / 8;
                uint8 const     bit         = 1 << ( pixelIndex % 8 );

                if ( !(g_zbuffer[ byteIndex ] & bit) )
                {
                    g_display.drawPixel( x, y, pallet[ color-1 ] );
                }
                else
                {
                    if ( drawMode & Draw_OnTop )
                    {
                        g_display.drawPixel( x, y, pallet[ color-1 ] );
                    }
                }
            }
        }
    }

    return result;
}

