/*
 * Point.h
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */



class Point
{
public:
    int16           x;
    int16           y;

public:
    inline Point();
    inline Point( int16 x, int16 y );
   
    inline Point          & operator =  ( const Point &rh );
    inline Point const    & operator += ( const Point &rh );
    inline Point const    & operator *= ( int16 v );
    inline Point            operator +  ( const Point &rh ) const;
    inline Point            operator -  ( const Point &rh ) const;

    inline bool             operator == ( const Point &rh ) const;
    inline bool             operator != ( const Point &rh ) const;

    inline Point            Sign() const;
    inline Point            Abs() const;
};


inline
Point::Point()
    : x     ( 0 )
    , y     ( 0 )
{
}


inline
Point::Point( int16 ux, int16 uy )
    : x     ( ux )
    , y     ( uy )
{
}


inline 
Point &
Point::operator = ( const Point &rh )
{
    x  = rh.x;
    y  = rh.y;
    return *this;
}


inline
bool
Point::operator == ( const Point &rh ) const
{
    return ( (x == rh.x) && (y == rh.y) );
}


inline
bool
Point::operator != ( const Point &rh ) const
{
    return !( *this == rh );
}


inline 
Point const & 
Point::operator += ( const Point &rh ) 
{
    x += rh.x;
    y += rh.y;
    return *this;
}


inline
Point
Point::operator + ( const Point &rh ) const
{
    return Point( x + rh.x, y + rh.y );
}


inline
Point
Point::operator - ( const Point &rh ) const
{
    return Point( x - rh.x, y - rh.y );
}


inline 
Point const &
Point::operator *= ( int16 v )
{
    x *= v;
    y *= v;
    return *this;
}


inline
Point 
Point::Abs() const
{
    return Point( ABS(x), ABS(y) );
}


inline
Point 
Point::Sign() const
{
    return Point( ::Sign<int16>(x), ::Sign<int16>(y) );
}
