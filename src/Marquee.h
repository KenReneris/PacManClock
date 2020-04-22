/*
 * Marquee.h
 *  Scrolling marquee text
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


class Marquee
    : public IStoryboard
{
public:
    static const uint32     k_MovementRate = 30;   // ms

public:
    static void         Show( char const *format, ... );
    static void         Show( FSTR const *format, ... );

public:
    class Animation
        : public SpriteBase
    {
    public:
        Animation();

        DrawResult      Draw( Point offset ) override;
        uint32          MovementSpeed() override;
    };


public:
    Marquee();
    void                VShow( FSTR const *format, va_list args );

    // IStoryboard
    bool                IsPending() const;
    StartResult         Start() override;
    void                Loop() override;

private:
    static uint16       ColorFromPallet( uint8 colorIndex );

private:
    struct Text
    {
        Point           _pos;
        uint16          _color;
        uint8           _width;
        char            _text[ 16 ];
        Text          * _next;

    public:
        Text( uint16 color );
        void InitWidth();

        DrawResult      Draw( Point offset ) const;
    };

private:
    uint16              _posX;
    Text              * _head;
    Animation         * _animation;
};


extern Marquee  g_marquee;

