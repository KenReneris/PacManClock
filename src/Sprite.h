/*
 * Sprite.h
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


class SpriteBase
{
public:
    const static uint16     k_explodeSpeed = 15;

public:
    enum class Mode : uint8
    {
        NotStarted          = 0,        // never been started
        Disabled            = 1,
        Started             = 2,        // waiting to appear, then changes to UntilDisappears
        UntilDisappears     = 3
    };

public:
    SpriteBase();

    SpriteBase            * NextSprite();

    Mode                    SpriteMode() const;
    bool                    HadCollision() const;
    int16                   FlipX( int16 x ) const;
    Point                   FlipX( Point pt ) const;
    Point                   GetPos() const;
    Point                   GetDir() const;
    Range                   RangeX() const;
    int16                   CenterX() const;
    bool                    IsOnHorzEdge() const;
    uint8                   MaxWidth() const;
    bool                    IsRunning() const;
    bool                    IsWithinWidth() const;
    bool                    IsPastWidth() const;

    void                    Start( Point pt, Point dir );
    void                    StartNoFlip( Point pt, Point dir );
    void                    AdjustPos( Point offset );
    void                    SetDir( Point dir );
    void                    SwitchTo( SpriteBase *start );
    void                    SwitchTo( SpriteBase *start, Point dir );
    void                    Disable();
    void                    EnsureOnSpriteList();
    void                    ExplodeSprite();

    virtual uint32          MovementSpeed() = 0;
    virtual void            BaseNextFrame();
    virtual void            NextFrame();

    virtual void            BaseDraw( Point offset );
    virtual DrawResult      Draw( Point offset ) = 0;
    virtual void            Freezing( uint32 duration );
    virtual void            Freeze();

public:
    static void             ResetSpriteList();

protected:
    Point                   _pos;           
    Point                   _dir;           
    Mode                    _drawMode;     
    uint8                   _maxWidth;
    bool                    _collision;
    uint8                   _explode;
    uint16                  _lastMs;

private:
    SpriteBase            * _next;
};



class Sprite
    : public SpriteBase
{
public:
    Sprite();
    Sprite( bool drawZ );
    void    InitSprite( BitmapBase const * const *bms, uint8 count );
    void    InitSprite( BitmapBase const * const *bms, uint8 count, uint16 *pallet );
    void    InitSprite( BitmapBase const * const *bms, uint8 count, uint16 *pallet, bool dontFlipX );
    void    SetDrawMode( DrawMode drawMode );

    DrawResult              Draw( Point offset ) override;
    uint32                  MovementSpeed() override;
    void                    NextFrame() override;
    void                    Freeze() override;

    void                    SetSequenceEnable( bool enabled );

    void                    ForEachBitmap( const function<void( BitmapBase *bm )> & callback );

protected:
    uint8                       _bmSeq;
    uint8                       _bmSeqDiv;
    uint8                       _bmMax;

private:
    uint16 const              * _pallet;        // overrides the pallet in the bitmap
    BitmapBase const         ** _bms;
    uint8                       _bmSeqEnabled : 1;
    uint8                       _bmExplode;     // defender style explosition of bitmap
    DrawMode                    _bmDrawMode;
};


extern SpriteBase   * g_spriteHead;
