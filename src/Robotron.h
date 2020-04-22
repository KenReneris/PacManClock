/*
 * Robotron.h
 *  Roobotron animations
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

namespace Robotron
{
    class Shooter : public Sprite
    {
    public:
        Shooter();
        void            Start( Point pt, Point dir );

        void            Loop();
        void            NextFrame() override;

        bool            IsHitByBullet( Range rangex );

    public:
        Sprite          _bullet[ 2 ];
        uint16          _lastFire;

        uint16          _lastMoveY;
        uint16          _lastTargetY;
        uint16          _targetY;

        uint16          _stomperLastSpeedAdj;
        uint16          _stomperSpeed;
        uint8           _stomperSeqRate;

        uint8           _hits;
    };


    class Human : public Sprite
    {
    public:
        Human();
        void            Start( Point pt, Point dir );

        void            Loop();
        void            NextFrame() override;

    public:
        uint16          _lastMoveY;
        uint16          _lastTargetY;
        uint16          _targetY;
    };


    class Stomper : public Sprite
    {
    public:
        void Init( Shooter *shooter, uint16 lastMs );

        uint32          MovementSpeed() override;
        void            NextFrame() override;
        void            MoveY();

    private:
        Shooter       * _shooter;
    };

    
    class Chase
        : public IStoryboard
    {
    public:
        Chase();

        StartResult     Start() override;
        void            Loop() override;

    public:

    private:
        Shooter         _shooter;
        Stomper       * _attackers; //  [7];
        uint8 const     _attackerMax;
        uint8           _attackerMoveYIndex;
        uint16          _attackerMoveYLast;
    };



    class RescueLady
        : public IStoryboard
    {
    public:
        RescueLady();

        StartResult     Start() override;
        void            Loop() override;

    private:
        class ScoreSprite : public Sprite
        {
        public:
            ScoreSprite();

            void        NextFrame() override;

        private:
            void        UpdatePallet();

        private:
            uint16      _pallet[ 3 ];
            uint8       _hue;
        };

    private:
        Shooter         _shooter;
        Sprite          _lady;
        ScoreSprite     _score;
        uint32          _end;
    };

    // global apis
    bool    IsAttack(); 
};
