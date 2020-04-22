/*
 * PacMan.ino
 *  Replacement for PacMan clock
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"
#include "WebServer.h"
#include "WiFiAp.h"
#include "NtpClient.h"
#include "TimeZone.h"
#include "Console.h"

#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2


enum class UpdateType
{
    Init,
    Start,
    Ntp,
    TimeZone,
    OnOff,
    AnimationManager,
    End,
};


Ticker              g_displayTicker;
static Console      g_console;                      // global instance of console 
static time_t       g_lastTime;                     // to notice when the time changes
bool                g_wifiIsConnected;              // global of last wifi connection status
volatile uint64     g_frameCount;                   // # of frame events
static UpdateType   g_update;
Options             g_options;                      // global instance of user settings
WiFiAp              g_wifiAp;                       // global instance of soft AP (and webserver)
NtpClient           g_ntp;                          // global instance of ntp client time sync
TimeZone            g_timeZone;                     // global instance of timezone time sync
GlobalState         g_globalState;                  // global instance of global state..
Clock               g_clock;
Marquee             g_marquee;
AnimationManager    g_animationManager;

PxMATRIX            g_display( PANEL_WIDTH, PANEL_HEIGHT, P_LAT, P_OE, P_A, P_B, P_C );

// Some standard colors
uint16              g_black         = g_display.color565(0, 0, 0);


// ISR for display refresh
void 
display_updater()
{
    g_display.display( 64 );    // too long and it starts to interfere with the animation updates (as they only update 1 step per target ms)
    g_frameCount += 1;
}


void 
display_update_enable( bool enable )
{
    if ( enable )
    {
        g_displayTicker.attach( 0.002, display_updater) ;
    }
    else
    {
        g_displayTicker.detach();
    }
}


void 
setup() 
{
    Serial.begin( 115200 );
    Out( F("\n\nArcadeClock: Initialize (setup)\n") );

    // Define your display layout here, e.g. 1/8 step
    g_display.begin( 8 );
    // // Define your scan pattern here {LINE, ZIGZAG, ZAGGIZ} (default is LINE)
    // //display.setScanPattern(LINE);
    // 
    // // Define multiplex implemention here {BINARY, STRAIGHT} (default is BINARY)
    // //display.setMuxPattern(BINARY);
    // display_update_enable( false );

    g_display.setFastUpdate( true );
    g_display.setBrightness( 255 );
    g_display.clearDisplay();
    g_display.setTextWrap( false );
    ClearZBuffer();

    g_clock.Setup();
    g_options.Setup();
    g_options.Load();

    g_wifiAp.Setup();
    g_ntp.Setup();
    g_timeZone.Setup();

    SpriteBase::ResetSpriteList();
    g_animationManager.Setup();

    Log( F("\n") );
    display_update_enable( true );
    Out( F("Setup Done\n") );
    g_update = UpdateType::Start;
}


bool
IsInit()
{
    return g_update == UpdateType::Init;
}


void
PinModePullUp( int gpio )
{
    if ( gpio )
    {
        pinMode( gpio, INPUT_PULLUP );
    }
}


void 
loop() 
{
    UpdateTime();
    g_options._dimOnOff.NextFrame();
    g_animationManager.NextFrame();

    g_update = UpdateType( int( g_update ) + 1 );
    switch ( g_update )
    {
    case UpdateType::Ntp:
        g_ntp.Loop();           
        break;

    case UpdateType::TimeZone:
        g_timeZone.Loop();
        break;

    case UpdateType::OnOff:
        g_options._dimOnOff.Loop();
        break;

    case UpdateType::AnimationManager:
        g_animationManager.Loop();
        break;

    case UpdateType::End:
        g_update = UpdateType::Start;
        break;
    }

    g_console.Loop();
    g_wifiAp.Loop();
}


void
UpdateWaitTimes()
{
    // skip if not running yet
    if ( g_poweredOnTime )
    { 
        // Settings or Time changed
        g_ntp.UpdateNextTime();
        g_timeZone.UpdateNextTime();
        g_wifiAp.UpdateNextTime();
        g_animationManager.UpdateNextTime();

        if ( !g_options._madjEnable )
        {
            g_options._madjFreq = 0;
            g_options._madjSmooth.Reset();
        }
    }
}
