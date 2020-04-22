/*
 * AnimationManager.h
 *  Collection of the animiations that are supported.
 *  Which one is "paged in" (only 1 animation at a time has state in memory)
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */



class AnimationManager
{
public:
    AnimationManager();

    // called by ino
    void                Setup();
    void                Loop();
    void                NextFrame();
    void                UpdateNextTime();

    // console
    void                OnRun( Animation animation );
    void                OnFlipX();
    uint32              AnimationCount( Animation animation );

    // apis for IStoryboard
    Animation           Active() const;
    void                ContinueTo( Animation nextType, uint32 nextTime );
    void                FreezeAnimations( uint32 duration );
    void                Done();

    // storyboard ram
    void              * salloc( size_t size, size_t count );
    void              * scopy( void const *p, size_t size, size_t count );
    void                AssertIsSRam( void const *p );

public:
    static FSTR const * Name( Animation animation );
    static FSTR const * _Name( Animation animation );

private:
    struct Storyboard
    {
        uint32              _lastTime;
        uint16              _count;
        uint16              _spare1;                // 
    };

private:
    void                    PickRandomAnimation();
    Options::Animation    * AnimationOptions( Animation animation );
    void                    Load( Animation animation );
    void                    Start();
    void                    Done( bool report );

private:
    uint32                  _lastMs;
    uint32                  _lastTime;              // last time an animation was run (seconds)
    uint16                  _totalAnimations;
    Animation               _initializingStoryboard;
    Animation               _activeAnimation;
    Animation               _continueTo;
    uint32                  _continueTime;          // next time for an animation (milliseconds or seconds depending on _continueTo)
    uint32                  _frozen;                // if frame advances are frozen
    IStoryboard           * _active;                // current active Storyboard
    FSTR const            * _activeName;
                       
    // for picking     
    uint32                  _disabledStoryboards;   // same may be disabled if they are using more salloc() ram then supported
    Storyboard              _storyboardState[ uint(Animation::RandomMax) ];
    uint32                  _maxWeight;
                       
    uint16                  _sallocStart;
    uint16                  _sallocPos;
    uint8                   _sbuffer[ 1500 ];
};


extern AnimationManager g_animationManager;
extern bool             g_flipX;
extern bool             g_debug;

/////////////////////////////////////////////////////
//
//
//


template<typename TYPE>
TYPE *
salloc( size_t count )
{
    return (TYPE *) g_animationManager.salloc( sizeof(TYPE), count );
}


template<typename TYPE>
TYPE *
scopy( TYPE const *items, size_t count )
{
    return (TYPE *) g_animationManager.scopy( items, sizeof(TYPE), count );
}


template<typename TYPE, typename ...ARGS>
TYPE *
snew( ARGS const &... args )
{
    return new( salloc<TYPE>(1) ) TYPE( args... );
}


inline
void
AssertIsSRam( void const *p )
{
    g_animationManager.AssertIsSRam( p );
}
