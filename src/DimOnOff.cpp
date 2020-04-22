/*
 * DimOnOff
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"


void
DimOnOff::Loop()
{
    //OnOff::DigitalRead( GPIO_FORCE_BRIGHT, GPIO_FORCE_DIM );
    _target = OnOff::Loop() ? g_options._dim : g_options._bright;
    if ( (!g_options._timeOnOff.IsOn()) && (g_animationManager.Active() == Animation::None) )
    {
        _target = 0;
    }
}


bool
DimOnOff::IsOn() const
{
    return (_current != 0);
}


void
DimOnOff::NextFrame()
{
    if ( _target != _current )
    {
        if ( (g_ms - _lastUpdate) > k_BrightnessAdjustRate )
        {
            int16 const  diff    = _target - _current;
            int16 const  step    = 4;
            int16 const  delta   = ( diff > 0 ? +step : -step );

            if ( ABS(diff) < step )
            {
                _current = _target;
            }
            else
            {
                _current += delta;
            }

            //Out( "%d: %s\n", g_ms, _current );
            g_display.setBrightness( _current );
            _lastUpdate = g_ms;
        }
    }
}
