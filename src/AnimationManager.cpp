/*
 * AnimationManager.h
 *  Collection of the animiations that are supported.
 *  Which one is "paged in" (only 1 animation at a time has state in memory)
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"

#include "PacMan.h"
#include "SpaceInvaders.h"
#include "Robotron.h"
#include "Defender.h"
#include "Pong.h"
#include "Mario.h"

//#define DEBUG_ANIMATION     Animation::PacManCatchGhost 

bool g_flipX;
bool g_debug;


AnimationManager::AnimationManager()
{
}


IStoryboard::~IStoryboard()
{
}


void
AnimationManager::Setup()
{
    uint16      maxMemory = 0;

    for ( Animation animation = Animation(1); animation < Animation::AnimationMax; animation = Animation(uint(animation)+1) )
    {
        if ( animation != Animation::RandomMax )
        {
            Load( animation );
            maxMemory = MAX( maxMemory, _sallocPos );
            Out( F("Loaded: %s (mem: %d)\n"), _activeName, _sallocPos );
            Done( false );
        }
    }

    Out( F("AM:MaxMemory: %d\n"), maxMemory );
    Assert( !_disabledStoryboards, F("Disabled Storyboards") );
    _lastTime = g_poweredOnTime;
    UpdateNextTime();
}


void
AnimationManager::UpdateNextTime()
{
    if ( (_continueTo == Animation::None) && (_activeAnimation == Animation::None) )
    {
        _continueTime = _lastTime + g_options._animationFrequency.Random();
#ifdef DEBUG_ANIMATION 
        _continueTime = _lastTime + 10;  
#endif
    }
}


void
AnimationManager::OnRun( Animation animation )
{
    if ( _active ) 
    {
        Out( F("Already running: %s\n"), _activeName );
    }
    else if ( !_Name(animation) )
    {
        Out( F("Invalid #") );
    }
    else
    {
        Load( animation );
        Start();
    }
}


uint32
AnimationManager::AnimationCount( Animation animation )
{
    return _storyboardState[ uint( animation ) ]._count;
}


void
AnimationManager::OnFlipX()
{
    if ( _active ) 
    {
        Out( F("Already running\n") );
    }
    else 
    {
        g_flipX = !g_flipX;
    }
}


void
AnimationManager::Loop()
{
    if ( _active )
    {
        if ( (_frozen) && (g_ms - _frozen > _continueTime) )
        {
            _frozen = 0;
        }

        if ( !_frozen )
        {
            _active->Loop();
        }

        if ( g_poweredOnTime - _lastTime > 2*60 )
        {
            Out( F("AM: Timedout - ForceDone\n") );
          //_disabledStoryboards |= (1 << uint(_disabledStoryboards));
            Done();
        }
    }
    else if ( _continueTo != Animation::None )
    {
        // is it time?
        if ( (g_ms - _lastMs) > _continueTime )
        {
            Load( _continueTo );
            Start();
        }
    }
    else if ( g_marquee.IsPending() )
    {
        Load( Animation::Marquee );
        Start();
    }
    else
    {
        //Out( "AM::Loop: %d\n", _activeAnimation );
        switch ( _activeAnimation )
        {
        case Animation::WaitingToResetClockPosition:
            if ( (g_ms - _lastMs) > _continueTime )
            {
                g_clock.SetBrightness( 0xFF );
                g_clock.SetTargetOffset( Point() );
                g_clock.DisableTimeUpdates( false );
                _activeAnimation = Animation::WaitingForTargetClockPosition;
            }
            break;

        case Animation::WaitingForTargetClockPosition:
            if ( g_clock.IsAtIdleOffset() )
            {
                _activeAnimation = Animation::WaitingToRepairClockDigits;
                _lastMs = g_ms;
            }
            break;

        case Animation::WaitingToRepairClockDigits:
            if ( (g_ms - _lastMs) > _continueTime )
            {
                g_clock.AnimationOver();
                _activeAnimation = Animation::None;
                UpdateNextTime();
            }
            break;
        
        default:
            if ( g_poweredOnTime > _continueTime )
            {
                _continueTime = g_poweredOnTime + 60;      // in case PickRandom doesn't pick anything

                if ( g_options._dimOnOff.IsOn() )
                {
                    PickRandomAnimation();

#ifdef DEBUG_ANIMATION
                    _continueTo = DEBUG_ANIMATION;
#else
                    g_flipX = random( 2 ) ? true : false;
#endif
                    if ( _continueTo != Animation::None )
                    {
                        Load( _continueTo );
                        Start();
                    }
                    else
                    {
                        Out( F( "No animation loaded\n" ) );
                    }
                }
            }
            break;
        }
    }
}


void
AnimationManager::FreezeAnimations( uint32 duration )
{
    _frozen = g_ms;
    _continueTime = duration;
    if ( !_frozen )
    {
        _frozen = 1;
    }

    for ( SpriteBase * sprite = g_spriteHead; sprite; sprite = sprite->NextSprite() )
    {
        sprite->Freezing( duration );
    }
}


void
AnimationManager::NextFrame()
{
    g_clockDrawMode = Draw_;

    if ( !_frozen )
    {
        if ( _active )
        {
            _active->NextFrame();
        }

        for ( SpriteBase * sprite = g_spriteHead; sprite; sprite = sprite->NextSprite() )
        {
            sprite->BaseNextFrame();
        }
    }

    ClearZBuffer();

    // wait for a frame to draw
    for ( uint64 frameCount = g_frameCount; (frameCount == g_frameCount); )
    {
        yield();
    }

    g_display.clearDisplay();

    if ( g_options._dimOnOff.IsOn() )
    {
        g_clock.DrawFirst();

        if ( _active )
        {
            _active->DrawFirst();
        }

        for ( SpriteBase * sprite = g_spriteHead; sprite; sprite = sprite->NextSprite() )
        {
            sprite->BaseDraw( Point() );
        }

        if ( _active )
        {
            // for drawing particles ontop
            _active->DrawEnd();
        }
    }

    yield();
}


void
AnimationManager::PickRandomAnimation()
{
    uint32      maxWeight           = 0;
    uint32      activeAnimations    = 0;

    for ( Animation animation = Animation(1); animation < Animation::RandomMax; animation = Animation(uint(animation)+1) )
    {
        if ( !(_disabledStoryboards & (1 << uint(animation))) )
        {
            Options::Animation  * options = AnimationOptions( animation );
            Storyboard          * state   = & _storyboardState[ uint(animation) ];

            if ( state->_lastTime + options->_maxRate*60 < g_poweredOnTime )
            {
                maxWeight += options->_weight;
                activeAnimations |= (1 << uint(animation) );
                //Out( F("Pick: %d, 0x%x, %d\n"), animation, activeAnimations, maxWeight );
            }
        }
    }

    if ( !activeAnimations )
    {
        Out( F("No active animations\n") );
    }

    uint32 const    rnd     = random( maxWeight+2 );
    uint32          weight  = 0;

    //Out( F("Pick: 0x%x, %d, %d\n"), activeAnimations, maxWeight, rnd );

    for ( Animation animation = Animation(1); animation < Animation::RandomMax; animation = Animation(uint(animation)+1) )
    {
        if ( (activeAnimations & (1 << uint(animation))) )
        {
            Options::Animation  * options = AnimationOptions( animation );

            weight += options->_weight;
            if ( weight >= rnd )
            {
                _continueTo = animation;
                break;
            }
        }
    }
}


Animation  
AnimationManager::Active() const
{
    return _activeAnimation;
}


void             
AnimationManager::ContinueTo( Animation nextType, uint32 nextTime )
{
    _continueTo = nextType;
    _continueTime = nextTime;
    Done( false );
 // Out( F("Continue to %d\n"), uint( nextType ) );
}


FSTR const *
AnimationManager::Name( Animation animation )
{
    FSTR const *    result = _Name( animation );

    if ( !result )
    {
        switch( animation )
        {
        case Animation::None:               result = F( "None" );                           break;
        case Animation::Marquee:            result = F( "Marquee" );                        break;
        default:                            result = F( "UnknownAnimationValue" );          break;
        }
    }

    return result;
}


FSTR const *
AnimationManager::_Name( Animation animation )
{
    switch( animation )
    {
    case Animation::ChasePacMan:            return F( "PacMan.Chase" );
    case Animation::ChasePacManCatchGhost:  return F( "PacMan.CatchGhost" );
    case Animation::CatchPacMan:            return F( "PacMan.CatchPacMan" );
    case Animation::GhostEyes:              return F( "PacMan.GhostEyes" );
    case Animation::SpaceInvaderA:          return F( "SpaceInvader.A" );
    case Animation::SpaceInvaderB:          return F( "SpaceInvader.B" );
    case Animation::SpaceInvaderUfoFlyBy:   return F( "SpaceInvader.UfoFlyBy" );
    case Animation::SpaceInvaderShooter:    return F( "SpaceInvader.Shooter" );
    case Animation::RobotronChase:          return F( "Robotron.Chase" );
    case Animation::RobotronRescueLady:     return F( "Robotron.RescueLady" );
    case Animation::RobotronAttack:         return F( "Robotron.Attack" );
    case Animation::Pong:                   return F( "Pong" );
    case Animation::Mario:                  return F( "Mario.RunBy" );
  //case Animation::DefenderTest:           return F( "Defender.Test" );        - not done 
    case Animation::DefenderUfoFlyBy:       return F( "Defender.UfoFlyBy" );

    case Animation::PacManChaseGhost:       return F( "PacManChaseGhost" );
    case Animation::PacManCatchGhost:       return F( "PacManCatchGhost" );
    case Animation::MarioRunBack:           return F( "Mario.RunBack" );
    }

    return nullptr;
}


void
AnimationManager::Load( Animation animation )
{
    Assert( !_active, F("AM::Load") );
    _initializingStoryboard = animation;
    _activeAnimation = animation;
    _activeName = Name( animation );

    #define START( T )                          \
        _active = snew<T>(); 

    switch( animation )
    {
    case Animation::None:                                                               break;
    case Animation::ChasePacMan:                START( PacMan::ChasePacMan );           break;
    case Animation::ChasePacManCatchGhost:      START( PacMan::ChasePacMan );           break;
    case Animation::CatchPacMan:                START( PacMan::CatchPacMan );           break;
    case Animation::GhostEyes:                  START( PacMan::GhostEyes );             break;
    case Animation::SpaceInvaderA:              START( SpaceInvaders::Invaders );       break;
    case Animation::SpaceInvaderB:              START( SpaceInvaders::Invaders );       break;
    case Animation::SpaceInvaderUfoFlyBy:       START( SpaceInvaders::UfoFlyby );       break;
    case Animation::SpaceInvaderShooter:        START( SpaceInvaders::Shooter );        break;
    case Animation::RobotronChase:              START( Robotron::Chase );               break;
    case Animation::RobotronRescueLady:         START( Robotron::RescueLady );          break;
    case Animation::RobotronAttack:             START( Robotron::Chase );               break;
    case Animation::Pong:                       START( Pong );                          break;
    case Animation::Mario:                      START( Mario::RunBy );                  break;
    case Animation::DefenderUfoFlyBy:           START( Defender::UfoFlyby );            break;

    // continuation sequences
    case Animation::PacManChaseGhost:           START( PacMan::ChaseGhost );            break;
    case Animation::PacManCatchGhost:           START( PacMan::CatchGhost );            break;
    case Animation::MarioRunBack:               START( Mario::RunBack );                break;

    case Animation::Marquee:
        _active = &g_marquee;
        break;

    default:
        Assert( F("AnimationMgr::Load") );
    }

    _initializingStoryboard = Animation::None;
}


Options::Animation * 
AnimationManager::AnimationOptions( Animation animation )
{
    if ( (animation > Animation::None) && (uint(animation) < countof(g_options._animation)) )
    {
        return & g_options._animation[ uint(animation) ];
    }
    else
    {
        Assert( F( "AnimationMgr::Options" ) );
    }
}


void
AnimationManager::Start()
{
    IStoryboard::StartResult startResult = IStoryboard::Start_None;

    Assert( _active, F("AM::Start") );
    SpriteBase::ResetSpriteList();
    yield();

    Out( F("Start: %s (mem: %d)" ), _activeName, _sallocPos );

    if ( _activeAnimation < Animation::RandomMax )
    {
        _totalAnimations += 1;
        _storyboardState[ uint(_activeAnimation) ]._count += 1;
        Out( F(" Count: %d out of %d"), _storyboardState[ uint(_activeAnimation) ]._count, _totalAnimations );
    }
    Out( "\n" );

    _continueTo = Animation::None;
    startResult = _active->Start();
    _lastTime = g_poweredOnTime;

    if ( startResult & IStoryboard::Start_DimClock )
    {
        g_clock.SetBrightness( 0x60 );
    }

    if ( startResult & IStoryboard::Start_DimClock2 )
    {
        g_clock.SetBrightness( 0x30 );
    }

    if ( startResult & IStoryboard::Start_ClockOffTop )
    {
        g_clock.SetTargetOffset( Point( 0, - (PANEL_HEIGHT - Clock::k_DigitY - 2) ) );
    }
    
    if ( startResult & IStoryboard::Start_ClockOffBottom )
    {
        g_clock.SetTargetOffset( Point( 0, (PANEL_HEIGHT - Clock::k_DigitY - 1) ) );
    }

    if ( startResult & IStoryboard::Start_ClockToTop )
    {
        g_clock.SetTargetOffset( Point( 0, 0 - Clock::k_DigitY ) );
    }

    if ( startResult & IStoryboard::Start_ClockToBottom )
    {
        g_clock.SetTargetOffset( Point( 0, 9 - Clock::k_DigitY ) );
    }

    bool const disableTimeUpdates = !!( startResult & IStoryboard::Start_DisableTimeUpdates );
    g_clock.DisableTimeUpdates( disableTimeUpdates );
    g_clock.EnsureQueued();
}


void
AnimationManager::Done()
{
    Done( true );
}


void              
AnimationManager::Done( bool report )
{
    if ( _active )
    {
        if ( report )
        {
            Out( F("Done.: %s\n\n"), _activeName );
        }

        if ( _activeAnimation < Animation::RandomMax )
        {
            _storyboardState[ uint(_activeAnimation) ]._lastTime = g_poweredOnTime;
        }

        if ( _active != &g_marquee )
        {
            _active->~IStoryboard();
        }

        _active = nullptr;
        _activeAnimation = Animation::None;
        SpriteBase::ResetSpriteList();
        g_clock.EnsureQueued();

        _sallocPos = 0;
        memset( &_sbuffer, 0, sizeof(_sbuffer) );
        yield();

        if ( _continueTo == Animation::None )
        {
            _activeAnimation = ( g_clock.IsAtIdleOffset() ? Animation::WaitingToRepairClockDigits  : Animation::WaitingToResetClockPosition );
            _continueTime = 750;
        }

        _lastMs = g_ms;
        _lastTime = g_poweredOnTime;
        UpdateNextTime();
    }
}


void * 
AnimationManager::salloc( size_t size, size_t count )
{
    void    * result = nullptr;

    if ( _active != &g_marquee )
    { 
        Assert( _initializingStoryboard != Animation::None, F("salloc") );
    }

    if ( count )
    {
        if ( size > 2 ) 
        { 
            _sallocPos = (_sallocPos + 3) & ~3;
        }
        else if ( size > 1 )
        {
            _sallocPos = (_sallocPos + 1) & ~1;
        }

        size = size * count;
        result = _sbuffer + _sallocPos;
        _sallocPos += size;

        //Out( F( "SAlloc: %s, %d, Total %d:\n" ), _activeName, size, _sallocPos );

        // if we overflowed memory, give this caller an already in use pointer and try to disable it and continue
        // this is useful for tuning the buffer size
        if ( _sallocPos > sizeof(_sbuffer) )
        {
            result = malloc( size );
            _disabledStoryboards |= ( 1 << uint8(_initializingStoryboard) );
            //Out( F("SAlloc: %s, %d, Total %d:\n"), _activeName, size, _sallocPos );
        }
    }

    yield();
    return result;
}


void * 
AnimationManager::scopy( void const *p, size_t size, size_t count )
{
    void * copy = salloc( size, count );

    memcpy( copy, p, size*count );
    return copy;
}


void 
AnimationManager::AssertIsSRam( void const *p )
{
    if ( (p < &_sbuffer) || ( p >= ( _sbuffer + sizeof(_sbuffer) ) ) )
    {
        // if the SB is disabled, salloc() returns malloc-ed() ram
        if ( !(_disabledStoryboards & (1 << uint(_initializingStoryboard))) )
        { 
            Assert( F("AssertIsSRam") );
        }
    }
}

