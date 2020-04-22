/*
 * Defender.h
 *  Defender animation
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


namespace Defender
{
    class Terrain
    {
    public:
        Terrain();

        void            Draw();
        void            End();

    private:
        uint8 const   * _terrain;
        uint16          _color;
        uint16          _posx;          // x pos of left of screen
        uint16          _endx;          // if ending, the last x-pos to draw
        uint16          _lastMove;      // time terrain last moved
        uint16          _lastAccel;     // time speed was last increased
        uint8           _speedMs;       // current moving speed
        uint8           _startx;        // if started, the amount to skip
    };


    class Ufo : public Sprite
    {
    public:
        Ufo();

        void            Loop();
        void            NextFrame() override;
        void            PickNewTarget();

    private:
        Sprite          _bullet[ 2 ];
        Point           _targetPos;
        bool            _leftRight;
        uint32          _started;
        uint16          _delay;
        uint8           _moveYDiv;
        uint8           _moveYRate;
    };

    ///////

    
    class UfoFlyby
        : public IStoryboard
    {
    public:
        UfoFlyby();

        StartResult         Start() override;
        void                Loop() override;
        void                DrawFirst() override;


    private:
        Terrain             _terrain;
        Ufo                 _ufo;
    };
};

