/*
 * Digit.h
 *  1 Digit of the clock (time) on the display
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


class DigitSprite
    : public SpriteBase
{
public:
    DigitSprite();
    void InitIsColon();
    void                SetColor( uint16 color );
                    
    void                SetX( uint8 x );
                    
    DigitSprite       & operator = ( const DigitSprite &rh );
                    
    bool                IsMoving() const;
    bool                IsModified() const;
                    
    uint8               PosX() const;
    uint8               GetValue() const;
    void                SetValue( uint8 value );
    void                SetDir( int16 dirY );
    void                SetStartPos( int16 dirY );
    uint8               PixelWidth() const;
                    
    uint32              MovementSpeed() override;
    void                NextFrame() override;

    DrawResult          Draw( Point offset ) override;

private:
    uint16              _color;
    uint8               _value;
    uint8               _width;
    bool                _forceAdvance1;
    bool                _modified;
    uint8               _bitmap[ BitmapBase::Header_Max + 9 ];     // copy of our bitmap (we allow Draw() to modify it sometimes)
};


class Digit
{
public:
    Digit();
    bool                IsMoving() const;

    uint8               GetValue() const;
    void                SetColor( uint16 color );
    void                SetX( uint8 x );
    void                SetValue( uint8 value );
    void                RedrawIfDamaged();
    uint8               PixelWidth() const;

    void                BaseDraw( Point offset );
    void                BaseNextFrame();

private:
    void                StartNewValue();

private:
    DigitSprite         _current;
    DigitSprite         _old;
};



