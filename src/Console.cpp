/*
 * Console
 *  Serial console commands.  Debugging etc..
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"

#include "Console.h"
#include "WebServer.h"
#include "WiFiAp.h"
#include "NtpClient.h"
#include "TimeZone.h"
#include "Columns.h"



Console::Cmd const  PROGMEM   Console::g_cmds[] =
{
    "?",        /* "help",                     */    & Console::OnHelp,
    "s",        /* "stats",                    */    & Console::OnStats,
    "sr",       /* "run states                 */    & Console::OnRunStats,
    "t",        /* "time",                     */    & Console::OnTime,
    "setnm",    /* "set personalization name", */    & Console::OnSetName,
    "setssid",  /* "set ssid name",            */    & Console::OnSetSsid,
    "ap-on",    /* "turn access point on/off", */    & Console::OnAccessPointOn,
    "ap-off",   /* "turn access point on/off", */    & Console::OnAccessPointOff,
    "wf-off",   /* "disconnect wifi",          */    & Console::OnWiFiDisconnect,
    "scan",     /* "scan wifi",                */    & Console::OnWiFiScan,
    "ntp",      /* "force ntp sync",           */    & Console::OnSyncNtp,
    "wcnt",     /* "WiFi.begin()",             */    & Console::OnWiFiBegin,
    "flipx",                                         & Console::OnFlipX,
    "run",                                           & Console::OnRun,
    "fdone",                                         & Console::OnForceDone,
    "sdebug",                                        & Console::OnSetDebug,
    "reboot",   /* "reboot",                   */    & Console::OnReboot,
    "PReset",   /* "reset password",           */    & Console::OnPasswordReset,
    "FReset",   /* "factory reset",            */    & Console::OnFactoryReset,
    "about",    /* "about",                    */    & Console::OnAbout,
};


void
Console::Loop()
{
    yield();
    if ( Serial.available() )
    {
        const char  c = Serial.read();

        ets_putc( c );
        if ( c == '\r' || c =='\n' )
        {
            if ( _bufferPos )
            {
                bool        found   = false;
                uint        argIndex    = 0;

                _buffer[ _bufferPos ] = 0;
                for ( uint index=0; (_buffer[ index ]) && (index < countof(_buffer)); ++index )
                {
                    if ( _buffer[index] == ' ' )
                    {
                        _buffer[ index ] = 0;
                        argIndex = index + 1;
                        break;
                    }
                }

                for ( uint index=0; index < countof(g_cmds); ++index )
                {
                    Cmd             cmd;

                    MemCpy( &cmd, &g_cmds[index], sizeof( Cmd ) );
                    if ( strcmp(_buffer, cmd._cmd) == 0 )
                    {
                        Out( F("\nCmd: %s\n"), _buffer );

                        _buffer[ 0 ] = 0;
                        if ( argIndex )
                        {
                            // move the arg to the buffer
                            memcpy( _buffer, _buffer + argIndex, countof(_buffer)-argIndex );
                        }
                        (this->*cmd._handler)();
                        found = true;
                        break;
                    }
                }

                if ( !found )
                {
                    Out( F("Cmd: '%s' not found\n"), _buffer );
                    OnHelp();
                }
            }

            _bufferPos = 0;
        }
        else if ( (_bufferPos < countof(_buffer)-1) && (c >= ' ' && c <= 'z') )
        {
            _buffer[ _bufferPos ] = c;
            _bufferPos += 1;
        }
    }
}


void
Console::OnHelp()
{
    Columns     out( 2, "  ", '-', nullptr );

    for( uint pass=0; pass < 2; ++pass )
    {
        out.SetPass( pass );
        for (uint index = 0; index < countof( g_cmds ); ++index)
        {
            Cmd             cmd;

            MemCpy( &cmd, &g_cmds[ index ], sizeof( Cmd ) );
            out.Row2( cmd._cmd, Columns::Fill::Right, F("") /*cmd._desc*/ );
        }
    }
}


void
Console::OnTime()
{
    Out( "CurTime: %d, %s\n", g_now, TimeStr().c_str() );
}


void
Console::OnSetName()
{
    uint    len = strlen( _buffer );
    
    if ( len )
    {
        if ( (_buffer[len-1] != ' ') && (len < countof(_buffer)) )
        {
            _buffer[ len ] = ' ';
            _buffer[ len + 1 ] = 0;
        }
        strncpy( g_options._prefixName, _buffer, sizeof(g_options._prefixName) );
        g_options.Save();
    }
}


void
Console::OnSetSsid()
{
    if ( strlen(_buffer) )
    {
        strncpy( g_options._ssid, _buffer, sizeof(g_options._ssid) );
        g_options.Save();
        system_restart();
    }
}


void
Console::OnAccessPointOn()
{
    g_options._accessPointLifespan = 255;
    g_wifiAp.UpdateNextTime();
}


void
Console::OnAccessPointOff()
{
    g_options._accessPointLifespan = 0;
    g_wifiAp.UpdateNextTime();
}


void
Console::OnWiFiDisconnect()
{
    WiFi.disconnect( true );
}


void
Console::OnWiFiScan()
{
    WiFi.scanNetworks( false );     // sync scan

    const int   max = WiFi.scanComplete();
    for( int index=0; index < max; ++index )
    {
        Out( F("  %d. %s\n"), index, WiFi.SSID(index).c_str() );
    }

    WiFi.scanDelete();      // disard any pending results
}


void
Console::OnSyncNtp()
{
    g_ntp.ForceSync();
}


void
Console::OnWiFiBegin()
{
    g_wifiAp.ConnectNow();
}


void
Console::OnFlipX()
{
    g_animationManager.OnFlipX();
}


void
Console::OnRun()
{
    uint    a = atoi( _buffer );

    g_animationManager.OnRun( Animation(a) );
}


void
Console::OnForceDone()
{
    g_animationManager.Done();
}


void
Console::OnSetDebug()
{
    g_debug = !g_debug;
}


void
Console::OnReboot()
{
    system_restart();
}


void
Console::OnPasswordReset()
{
    if ( g_options._password[0] )
    {
        g_options._password[ 0 ] = 0;
        g_options.Save();
    }
}


void
Console::OnFactoryReset()
{
    char    prefixName[ sizeof( g_options._prefixName ) ];

    Out( F("Console: RESET\n") );
    g_options.Reset();          // clear our settings
    WiFi.disconnect( true );    // clear wifi settings
    system_restart();
    Out( F("Console: Failed?\n") );
    for (; ; );
}


void 
Console::OnAbout()
{
    MiscAbout( nullptr );
}


void
Console::TextOut( HtmlOut const &htmlOut, char const *format, ... )
{
    StringBuffer    sb( htmlOut ? htmlOut : OutStr );
    bool            link    = false;
    va_list         args;

    va_start( args, format );

    for ( char c = *format; c; ++format, c=*format)
    {
        switch ( c )
        {
        case '\r':
            sb.Add( htmlOut ? ' ' : '\n' );
            break;
        
        case '\n':
            sb.Add( htmlOut ? "<br>" : "\n" );
            break;
        
        case '*':
            if ( htmlOut )
            {
                sb.AddF( "<a href='https://%s'>", va_arg(args, char const *) );
                link = true;
            }
            break;
        
        case ' ':
            if ( link )
            {
                sb.Add( "</a>" );
                link = false;
            }
            sb.Add( c );
            break;
        
        default:
            sb.Add( c );
            break;
        }
    }

    va_end( args );
}


void
Console::MiscAbout( HtmlOut const & htmlOut )
{
    // // bake this into readonly html data? (to avoid to large heap blocks it creates)
    // TextOut( 
    //     htmlOut,
    //     "\n"
    //     "The *EleksTube clock from *Kickstarter brings RGB addressable LEDs in a Nixie tube\r"
    //     "format powered by a USB source. This project replaces the controller that comes\r"
    //     "with the clock with an *ESP8266 that syncs the time to an NTP time source over\r"
    //     "wifi, and includes a small webserver for configuration.\n\n",
    //         "www.banggood.com/EleksMaker-EleksTube-Bamboo-6-Bit-Kit-Time-Electronic-Glow-Tube-Clock-Time-Flies-Lapse-p-1297292.html",
    //         "www.kickstarter.com/projects/938509544/elekstube-a-time-machine",
    //         "www.esp8266.com"
    //     );
    // 
    // TextOut( htmlOut, "The following web servers are used:\n" );
    // {
    //     Columns     out( 2, "  ", '-', htmlOut );
    // 
    //     for (uint pass = 0; pass < 2; ++pass)
    //     {
    //         out.SetPass( pass );
    //         out.Row2( "time.google.com",    "NTP time service" );
    //         out.Row2( "ip-api.com",         "GeoLocation for timezone" );
    //         out.Row2( "timezonedb.com",     "DST times for timezone" );
    //     }
    // }
    // 
    // TextOut( htmlOut, "The following GIT project provided reference material:\n" );
    // {
    //     Columns     out( 2, ". ", ':', htmlOut );
    // 
    //     for (uint pass = 0; pass < 2; ++pass)
    //     {
    //         out.SetPass( pass );
    //         out.Row3a( "NTP Client", "github.com/arduino-libraries/NTPClient" );
    //         out.Row3a( "ElkesTube",  "github.com/ib134866/EleksTube" );
    //     }
    // }
    // 
    // TextOut( htmlOut, "The following GIT projects are used directly:\n" );
    // {
    //     Columns     out( 2, ". ", ':', htmlOut );
    // 
    //     for (uint pass = 0; pass < 2; ++pass)
    //     {
    //         out.SetPass( pass );
    //         out.Row3a( "esp8266",                           "github.com/esp8266/Arduino" );
    //         out.Row3a( "FastLED",                           "github.com/FastLED/FastLED" );
    //         out.Row3a( "Color Picker",  "Taufik Nurrohman", "tovic.github.io/color-picker" );  
    //         out.Row3a( "Time Library",  "Paul Stoffregen",  "github.com/PaulStoffregen/Time" );
    //         out.Row3a( "This Firmware", "Ken Reneris",      "github.com/KenReneris/EleksTubeClock" );  
    //     }
    // }
    // 
    // TextOut( htmlOut, "Many thanks to all of those who have contibuted in making this unique clock.\n\nKen Reneris\n12/25/2018\n\n" );
}


void
Console::OnRunStats()
{
    MiscRunStats( nullptr );
}


void
Console::MiscRunStats( HtmlOut const & htmlOut )
{
    using Fill = Columns::Fill;

    Columns         out( 2, ". ", ':', htmlOut );

    for ( uint pass = 0; pass < 2; ++pass )
    {
        out.SetPass( pass );

        for (Animation index=Animation(1); index < Animation::RandomMax; index=Animation(uint(index)+1) )
        {
            out.Row2( AnimationManager::Name(index), g_animationManager.AnimationCount(index) );
        }
    }
}


void
Console::OnStats()
{
    MiscStats( nullptr );
}


void
Console::MiscStats( HtmlOut const & htmlOut )
{
    using Fill = Columns::Fill;

    uint            wifiStatus = WiFi.status();
    Columns         out( 2, ". ", ':', htmlOut );
    char            version[ 8 ];

    SPrintF( version, F("1.%d"), g_options._version );
    for ( uint pass = 0; pass < 2; ++pass )
    {
        out.SetPass( pass );

        out.Row2( F("Version"),                 version );
        out.Row2( F("Prefix"),                  g_options._prefixName );
        out.Row2( F("SSID"),                    g_options._ssid );
        out.Row2( F("Password"),                g_options._password[0] ? "yes" : "no" );
        out.Row2( F("Time"),                    TimeStr() );
        out.Row2( F("RunTime"), Fill::Right,    Duration( g_poweredOnTime ) );
        out.Row2( F("Frames"),                  u64str( g_frameCount ) );
        out.Row2( F("g_flipX"),                 g_flipX ? F("TRUE") : F("FALSE") );
        out.Row2( F("Web Requests"),            g_wifiAp.Server().ResponsesSent() );
        out.Row2( F("WifiMode"),                WiFiAp::WlMode2Str(WiFi.getMode()) );
        out.Row2( F("WifiStatus"),              WiFiAp::WlStatus2Str(wifiStatus) );
        if ( wifiStatus & WIFI_AP )
        {
            out.Row2( F("Wifi.AP.ip"),          WiFi.softAPIP().toString() );
        }
        if ( wifiStatus & WIFI_STA )
        {
            out.Row2( F("Wifi.STA.ip"),         WiFi.localIP().toString() );
        }
        out.Row2( F("MAC address"),             WiFi.macAddress() );
        out.Row2( F("NtpState"),                g_ntp.GetStateStr() );
        out.Row2( F("NtpSync"), Fill::Right,    g_ntp.LastSync() );
        out.Row2( F("TzState"),                 g_timeZone.GetStateStr() );
        out.Row2( F("GmtOffset"),               g_options._gmtOffset );
        out.Row2( F("free_heap"),               ESP.getFreeHeap() );
    }
}