/*
 * Bitmap.cpp
 *  An internally encoded image.
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"



BitmapBase::BitmapBase( uint16 movementSpeed )
    : _movementSpeed        ( movementSpeed )
{
}


void
BitmapBase::Init( uint8 const *bitmap, uint8 bitSize, bool flipx )
{
    if ( IsRom(bitmap) )
    {
        uint32 const    hdr0        = pgm_read_dword_far( bitmap );
        uint8 const     seqSpeed    = ( (hdr0 >>  0) & 0xFF );
        uint8 const     flags       = ( (hdr0 >>  8) & 0xFF );

        uint32 const    hdr4        = pgm_read_dword_far( bitmap + Header_PalletCount );
        uint8 const     palletCount = ( (hdr4 >>  0) & 0xFF );
        uint8 const     offsetY     = ( (hdr4 >>  8) & 0xFF );
        uint8 const     width       = ( (hdr4 >> 16) & 0xFF );
        uint8 const     height      = ( (hdr4 >> 24) & 0xFF );
        uint8 const     stride      = ( (bitSize*width) + 7 ) / 8;

        _width      = width;
        _height     = height;
        _offsetY    = offsetY;
        _seqSpeed   = seqSpeed;
        _flags      = flags;

        if ( palletCount )
        {
            Assert( palletCount < 16, F("PalletCount>16") );

            uint16    * pallet  = salloc<uint16>( palletCount );
            CRGB        rgb[ 16 ];

            MemCpy( rgb, bitmap + Header_Max, sizeof(CRGB)*palletCount );

            _pallet = pallet;
            for (uint8 palletIndex = 0; palletIndex < palletCount; ++palletIndex)
            {
                pallet[ palletIndex ] = Color( rgb[ palletIndex ].r, rgb[ palletIndex ].g, rgb[ palletIndex ].b );
            }
        }

        uint32 const        pixelSize   = height * stride;
        uint8       * const dst1        = salloc<uint8>( pixelSize );
        uint8 const * const srcPixels   = bitmap + Header_Max + sizeof(CRGB)*palletCount;

        _pixels = dst1;
        MemCpy( dst1, srcPixels, pixelSize );

        flipx ^= g_flipX;
        if ( (flipx) && !(flags & Flag_DontFlipX) )
        {
            uint8   * const row0                = dst1;
            uint8 const     mask                = ( 1 << bitSize ) - 1;
            uint8 const     pixelsPerByte       = 8 / bitSize;
            uint8           src[ PANEL_WIDTH ];

            //Out( "BMR: %d, %d, %d, %d\n", bitSize, mask, pixelsPerByte, stride );

            for( uint8 indexY=0; indexY < height; ++indexY )
            {
                uint8     * bm  = row0 + (indexY * stride);

                memcpy( src, bm, stride );
                memset( bm, 0, stride );

                for ( uint8 indexX=0; indexX < width; ++indexX )
                {
                    uint8       color    = 0;

                    {
                        uint8 const     rIndex      = width - indexX - 1;
                        uint8 const     srcByte     = src[ rIndex / pixelsPerByte ];
                        uint8 const     srcShift    = bitSize * ( rIndex % pixelsPerByte );

                        color = ( srcByte >> srcShift ) & mask;
                        //Out( "%d ", color );
                    }

                    bm[ indexX / pixelsPerByte ] |= ( color << (bitSize * (indexX % pixelsPerByte)) );
                }
                //Out( "\n" );
            }
            yield();
        }
    }
    else
    {
        _width      = bitmap[ Header_SizeX ];
        _height     = bitmap[ Header_SizeY ];
        _offsetY    = bitmap[ Header_OffsetY ];
        _flags      = bitmap[ Header_Flags ];
        _pixels     = bitmap + Header_Max;
    }
}


uint16 const *
BitmapBase::Pallet() const
{
    return _pallet;
}


uint8  
BitmapBase::Width() const
{
    return _width;
}


Point
BitmapBase::Size() const
{
    return Point( _width, _height );
}



uint16          
BitmapBase::MovementSpeed() const
{
    return _movementSpeed;
}


uint8
BitmapBase::SequenceSpeed() const
{
    return _seqSpeed;
}


uint8
BitmapBase::Flags() const
{
    return _flags;
}


void
BitmapBase::SetMovementSpeed( uint16 speed )
{
    _movementSpeed = speed;
}


DrawResult 
BitmapBase::Draw( Point pos, DrawMode drawMode, uint16 const * pallet ) const
{
    return const_cast< BitmapBase * >( this )->Draw( pos, drawMode, pallet );
}
