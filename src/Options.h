/*
 * Options.h
 *  User options saved to EEPROM.
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


struct Options
{
public:
    struct Animation
    {
        uint8           _maxRate;               // minutes
        uint8           _weight;
    };

public:
    uint32              _checksum;
    uint8               _version;               // version of this structure
    bool                _dirty;                 // for slow updates mark dirty.  only saved when wifi is lost

    char                _prefixName[ 10 ];      // webserver decroates page names with this
    char                _ssid[ 32 ];            // the AP name.  defaults to "EleksTube"
    char                _password[ 16 ];        // password to edit settings via the web interface

    TimeOnOff           _timeOnOff;             // schedule for when the time (& date) should be shown
    DimOnOff            _dimOnOff;              // schedule for when dimming the display should be in effect
    uint8               _bright;                // the users setting of !Dim
    uint8               _dim;                   // the users setting of Dim

    char                _ntpServer[ 32 ];       // ntp server to use. defauts to "time.google.com"
    HourMinute          _ntpSync;               // hh:mm to align syncing too
    uint8               _ntpFrequency;          // ntp polling rate in 10 minute increments (aligned to _ntpSync)

    bool                _madjEnable;            // if computing micro adjustments of lcoal time-drift are enabled. 
    int8                _madjDir;               // micro adjust +1 or -1 a millisecond 
    uint32              _madjFreq;              // micro adjust _madjDir every n elasped ms's
    Smooth<4>           _madjSmooth;            // smooth out the adjustment rate with a little history

    char                _tzKey[ 16 ];           // lead byte is 0 if disabled.  the users timezonedb.com key
    int32               _gmtOffset;             // delta from gmtTime.  "g_now = g_gmtTime + _gmtOffset"

    uint8               _accessPointLifespan;   // minutes http AP server is active after coming online. 0=off, 255=no-timeout.  Note the STA server always stays on line.

    Range               _animationFrequency;
    Animation           _animation[ uint(::Animation::RandomMax) ];     // index 0 not used
    
public:
    void                Setup();
    void                Load();
    void                Save();
    void                Reset();

private:
    uint32              Checksum();
};


extern Options          g_options;

