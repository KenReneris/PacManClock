/*
 * Rect.h
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


class Rect
{
public:
    Point           pt0;            // upper-left
    Point           pt1;            // lower-right

public:
    inline Rect();
    inline Rect( Point pt0, Point pt1 );l

    inline Rect     operator + ( const Point &rh ) const;
};


inline
Rect::Rect()
{
}

inline
Rect::Rect( Point upt0, Point upt1 )
    : pt0       ( upt0 )
    , pt1       ( upt1 )
{
}


inline 
Rect
Rect::operator + ( const Point &rh ) const
{
    return Rect( pt0 + rh, pt1 + rh );
}



