/*
 * IStoryBoard.h
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */



enum class Animation : uint8
{
    None = 0,

    ChasePacMan,
    ChasePacManCatchGhost,
    CatchPacMan,
    GhostEyes,

    SpaceInvaderA,
    SpaceInvaderB,
    SpaceInvaderUfoFlyBy,
    SpaceInvaderShooter,

    Pong,
    Mario,

    RobotronChase,
    RobotronRescueLady,
    RobotronAttack,

    DefenderUfoFlyBy,

    RandomMax,

    // these animations aren't randomly selected and don't require some of the state the above animations do
    Marquee,
    PacManChaseGhost,
    PacManCatchGhost,
    MarioRunBack,
    AnimationMax,

    //
    WaitingToResetClockPosition,
    WaitingForTargetClockPosition,
    WaitingToRepairClockDigits,
};



class IStoryboard
{
public:
    enum StartResult : uint8
    {
        Start_None                  = 0x00,
        Start_DimClock              = 0x01,
        Start_DimClock2             = 0x02,
        Start_ClockOffTop           = 0x04,
        Start_ClockOffBottom        = 0x08,
        Start_ClockToTop            = 0x10,
        Start_ClockToBottom         = 0x20,
        Start_DisableTimeUpdates    = 0x40,
    };

public:
    virtual ~IStoryboard();
    virtual StartResult             Start() = 0;
    virtual void                    Loop() = 0;
    virtual void                    NextFrame() { };
    virtual void                    DrawFirst() { };
    virtual void                    DrawEnd() { };
};




