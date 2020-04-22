/*
 * Particles.h
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


class Particles
{
public:
    Particles( uint8 speed, uint8 maxCount, CRGB color, uint8 distance ); 

    void    Add( Point pt );

    // SpriteBase...
    void    NextFrame();
    void    Draw();

private:
    struct Particle
    {
        int8        _x;
        int8        _y;
        uint8       _dist;          // distance from x,y
        uint8       _last;          // 8 bit ms time
    };

private:
    uint8 const     _maxDist;
    uint8 const     _speed;
    uint8 const     _maxParticles;
    uint8           _inUse;

    uint16        * _colors;
    Particle      * _particles;     // buffer
};

