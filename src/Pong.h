/*
 * Pong.h
 *  Pong animation
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


class Pong
    : public IStoryboard
{
public:
    Pong();

    StartResult         Start() override;
    void                Loop() override;

private:
    class Puck : public Sprite
    {
    public:
        Puck( Pong *parent );

        Point           GetPos() const;
        void            Start();
        void            BaseNextFrame() override;
        bool            GameOver() const;

    private:
        void            SetVector( float angle, uint16 vectorRate );

    private:
        Pong    * const _parent;
        bool            _gameOver;
        Point           _pos1;
        Point           _rate;          // x & y rate in ms
        uint32          _lastX;
        uint32          _lastY;
    };

    class Paddle : public Sprite
    {
    public:
        Paddle( Pong *parent );

        bool            ExitGame();
        void            Track();

    private:
        Pong    * const _parent;
        uint8           _target;
    };

private:
    Paddle              _leftPaddle;
    Paddle              _rightPaddle;
    Puck                _puck;
    uint16              _hits;
};


