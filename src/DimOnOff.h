/*
 * DimOnOff.h
 *  Tracks when the clock should be dimmed.
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


class DimOnOff : public OnOff
{
public:
    static const uint8      k_BrightnessAdjustRate = 10;

public:
    void        Loop();
    void        NextFrame();
    bool        IsOn() const;

private:
    uint32      _lastUpdate;
    uint8       _current;
    uint8       _target;            // the target brightness
};
