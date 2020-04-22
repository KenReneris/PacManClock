/*
 * Options
 *  User settings presisted to flash.
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

#include "platform.h"
#include <EEPROM.h>

#define VERSION     1

void
Options::Setup()
{
    EEPROM.begin( 512 );        // Initialize EEPROM
}


void
Options::Load()
{
    uint8   * const start   = (uint8 *) this;

    for( int pos=0; pos < sizeof(*this); ++pos )
    {
        start[ pos ] = EEPROM.read( pos );
    }
    _tzKey[ countof(_tzKey)-1 ] = 0;

    if ( (_checksum != Checksum()) || (_version != VERSION) || (!_ntpFrequency) )
    {
        Reset();
    }
}


void
Options::Save()
{
    uint8   * const start = (uint8 *) this;

    Marquee::Show( F("Options: Save\n") ); 
    _checksum = Checksum();
    _dirty = false;
    for (int pos = 0; pos < sizeof(*this); ++pos)
    {
        EEPROM.write( pos, start[pos] );
    }
    EEPROM.commit();
    UpdateWaitTimes();
}


void
Options::Reset()
{
    Marquee::Show( F("Options: Reset\n") );
    memset( this, 0, sizeof(*this) );

    _version                = VERSION;

    strcpy_P( _ssid,      (char *) F("PacManClock") );
    strcpy_P( _ntpServer, (char *) F("time.google.com") );
    strcpy( _tzKey, "2TBMSLD4JDJD" );                   // ************* REMOVE

    _timeOnOff._onDays      = 0x7F;
    _timeOnOff._gpioOnTime  = 10;
    _timeOnOff._gpioOffTime = 8*60*60;

    _dimOnOff._gpioOnTime   = 8*60*60;
    _dimOnOff._gpioOffTime  = 10;
    _bright                 = 0xFF;
    _dim                    = 0x80;

    //_topOfHour              = { (ARGB::BlendOut | 2),  0xFF, 0xFF, 0xFF };          // white
    //_quarterOfHour          = { (ARGB::BlendOut | 1),  0x00, 0xA0, 0xA0 };          // cyan

    //_ntpColor               = { (ARGB::BlendInOut | 0),  0xFF, 0xFF, 0x00 };        // yellow
    _ntpSync._hour          = 4;
    _ntpSync._minute        = 10;
    _ntpFrequency           = 144;                                                  // daily

    _madjEnable             = true;

    //_tzColor                = { (ARGB::BlendInOut | 1),  0xFF, 0xFF, 0x00 };        // yellow
    //_httpClient             = { (ARGB::BlendOut | 3),  0xFF, 0xA5, 0x00 };          // orange

    _accessPointLifespan    = 60;

    _animationFrequency.min     = 60;
    _animationFrequency.max     = 6 * 60;

    for ( uint8 index=0; index < countof(_animation); ++index )
    {
        _animation[ index ]._maxRate    = 1;
        _animation[ index ]._weight     = 10;
    }
  
    _animation[ uint(::Animation::CatchPacMan) ]._weight  = 5;
    _animation[ uint(::Animation::CatchPacMan) ]._maxRate = 30;
                
    _animation[ uint(::Animation::GhostEyes) ]._weight = 2;
    _animation[ uint(::Animation::GhostEyes) ]._maxRate = 60;
                
    _animation[ uint(::Animation::SpaceInvaderUfoFlyBy) ]._weight = 2;
    _animation[ uint(::Animation::SpaceInvaderUfoFlyBy) ]._maxRate = 60;
                
    _animation[ uint(::Animation::Mario) ]._weight = 2;
    _animation[ uint(::Animation::Mario) ]._maxRate = 60;

    Save();
}


uint32
Options::Checksum()
{
    uint8   * const start           = (uint8 *) this;
    uint32          checksum        = 0;
    const uint32    oldChecksum     = _checksum;

    _checksum = 0;
    for ( int pos = 0; pos < sizeof(*this); ++pos )   
    {
        checksum = (checksum << 1) + (start[ pos ] ^ 0x5A);
    }
    _checksum = oldChecksum;

    checksum &= 0x00FFFFFF;
    checksum |= ( sizeof(*this) << 24 );

    return checksum;
}

