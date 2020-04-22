/*
 * Clock.h
 *  Top level object for the pacman clock
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */



class Clock
    : public SpriteBase
{
public:
    static const uint8      k_DigitY                =  4;
    static const uint32     k_BrightnessAdjustRate  =  4;   // ms (fast because the effect is not great)
    static const uint32     k_MovementRate          = 80;   // ms

public:
    Clock();

    bool                IsAtIdleOffset() const;

    void                Setup();

    // called by animation manager
    void                DrawFirst();
    void                SetTargetOffset( Point offset );    // moves to this location 
    void                SetOffset( Point offset );          // immediate jump
    void                SetBrightness( uint8 level );
    void                DisableTimeUpdates( bool disableTimeUpdates );
    void                EnsureQueued();
    void                AnimationOver();

    uint32              MovementSpeed() override;
    void                BaseNextFrame() override;
    void                BaseDraw( Point offset ) override;
    DrawResult          Draw( Point offset ) override;

private:
    void                UpdateColors();

private:
    bool                _disableTimeUpdates;

    uint16              _clockColor;
    uint16              _colonColor;
    Digit               _digits[ 5 ];

    uint32              _lastBrightnessChange;      // time of last level change
    uint8               _currentBrightness;
    uint8               _targetBrightness;

    uint32              _lastOffsetMove;            // time of last offset move
    Point               _currentOffset;            
    Point               _targetOffset;
};


extern Clock        g_clock;
extern DrawMode     g_clockDrawMode;

