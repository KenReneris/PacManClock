/*
 * MemCpy
 *  utility (handles rom source addresses)
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"


void 
MemCpy( void *dst, void const *src, size_t size )
{
    if ( IsRom(src) )
    {
        uint32       * dst4 = reinterpret_cast<uint32 *>( dst );
        uint32 const * src4 = reinterpret_cast<uint32 const *>( src );
        
        // align source
        if ( ((uint) src4) & 3 )
        {
            uint8       * dst1 = reinterpret_cast<uint8 *>( dst4 );
            uint8 const * src1 = reinterpret_cast<uint8 const *>( src4 );

            while ( (size) && (((uint) src4) & 3) )
            {
                *dst1 = pgm_read_byte_far( src1 );
                dst1 += 1;
                src1 += 1;
                size -= 1;
            }

            src4 = reinterpret_cast<uint32 const *>( src1 );
            dst4 = reinterpret_cast<uint32 *>( dst1 );
        }

        if ( !( ((uint)dst4) & 3) )
        {
            // src & dst is aligned
            while( size >= 4 )
            {
                *dst4 = pgm_read_dword_far( src4 );
                dst4 += 1;
                src4 += 1;
                size -= 4;
            }
        }
        else
        {
            uint8       * dst1 = reinterpret_cast<uint8 *>( dst4 );

            // src is aligned, but dst is not
            while( size >= 4 )
            {
                uint32  d = pgm_read_dword_far( src4 );
                src4 += 1;
                size -= 4;

                *(dst1++) = (d >>  0);
                *(dst1++) = (d >>  8);
                *(dst1++) = (d >> 16);
                *(dst1++) = (d >> 32);
            }

            dst4 = reinterpret_cast<uint32 *>( dst1 );
        }
        
        // any extra bytes at the end
        uint8       * dst1 = reinterpret_cast<uint8 *>( dst4 );
        uint8 const * src1 = reinterpret_cast<uint8 const *>( src4 );
        while ( size )
        {
            *dst1 = pgm_read_byte_far( src1 );
            dst1 += 1;
            src1 += 1;
            size -= 1;
        }
    }
    else
    {
        memcpy( dst, src, size );
    }
}



