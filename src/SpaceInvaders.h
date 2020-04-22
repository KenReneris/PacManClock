/*
 * SpaceInvaders.h
 *  SpaceInvaders animations
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

namespace SpaceInvaders
{
    class Invaders
        : public IStoryboard
    {
    public:
        Invaders();

        StartResult         Start() override;
        void                Loop() override;
        void                NextFrame() override;

    private:
        uint16              _speed;
        Point               _dir;
        Point               _lastPos0;
        uint8               _invaderCount;
        Sprite              _invaders[ 2 ];
    };


    class UfoFlyby
        : public IStoryboard
    {
    public:
        UfoFlyby();

        StartResult         Start() override;
        void                Loop() override;
        void                NextFrame() override;

    private:
        Sprite              _ufo;
    };


    class Shooter
        : public IStoryboard
    {
    public:
        Shooter();

        StartResult         Start() override;
        void                Loop() override;
        void                NextFrame() override;
        void                DrawEnd() override;

    private:
        class Bullet : public Sprite
        {
        public:
            Bullet( Shooter * parent );

            void            NextFrame() override;

        private:
            Shooter       * _parent;
            uint8           _hit;
        };

    private:
        uint32 const        _start;
        uint32              _runTime;
        Sprite              _shooter;
        int16               _shooterPos;
        bool                _fireBullet;
        uint32              _shooterDelayStart;
        uint32              _shooterWait;
        Particles           _particles;
        Bullet              _bullet;
    };
}

