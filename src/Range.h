/*
 * Range.h
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


class Range
{
public:
    int16           min;
    int16           max;

public:
    inline Range();
    inline Range( int16 min, int16 max );

    inline bool             InRange( int16 v ) const;

    inline Range          & operator = ( const Range &rh );
    inline int16            Random() const;
};


inline
Range::Range()
    : min   ( 0 )
    , max   ( 0 )
{
}


inline
Range::Range( int16 umin, int16 umax )
    : min   ( umin )
    , max   ( umax )
{
}


inline 
Range &
Range::operator = ( const Range &rh )
{
    min = rh.min;
    max = rh.max;
    return *this;
}


inline 
bool 
Range::InRange( int16 v ) const
{
    return (v >= min) && (v < max);
}


inline
int16
Range::Random() const
{
    return random( min, max );
}

