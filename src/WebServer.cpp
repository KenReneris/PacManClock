/*
 * WebServer
 *  Local WebServer for EleksTubeClock
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

#include "platform.h"
#include "WebServer.h"

#include "NtpClient.h"
#include "TimeZone.h"
#include "Console.h"


char const PROGMEM  WebServer::k_daysOfWeek3[] = "Sun\0Mon\0Tue\0Wed\0Thu\0Fri\0Sat";


WebServer::WebServer()
    : _responses            ( 0 )
    , _snappedOptions       ( nullptr )
    , _buffer               ( [this](char const *txt){ _content += txt; } )
{
}


uint32
WebServer::ResponsesSent() const
{
    return _responses;
}


void
WebServer::on( char const *uri, THandlerFunction handler )
{
    on( uri, true, handler );
}


void
WebServer::on( char const *uri, bool sta, THandlerFunction handler )
{
    WiFi.scanNetworksAsync( nullptr );

    ESP8266WebServer::on( 
        PrintF( F("/%s"), uri ),
        [this, sta, handler]()
        {           
            const bool    isLocal = ( this->client().localIP() == WiFi.localIP() );

            Out( F("WebServer: '%s'\n"), this->uri().c_str() );
            ResetPageState();

            if ( sta != isLocal )
            {
                OnNotFound();
            }
           
            handler();
            SendPage();
        } );
}


void
WebServer::begin()
{
    // todo - could save some ram here 

    on( "",             std::bind( &WebServer::OnRoot, this ) );
    on( "r2",           std::bind( &WebServer::OnRoot2, this ) );
    on( "p",            std::bind( &WebServer::OnGetPassword, this ) );
    on( "Sp",           std::bind( &WebServer::OnSetPassword, this ) );

    // forms
    on( "Settings",     std::bind( &WebServer::OnSettings, this ) );
    on( "SSettings",    std::bind( &WebServer::OnSetSettings, this ) );
    on( "TOnOff",       std::bind( &WebServer::OnTimeOnOff, this ) );
    on( "STOnOff",      std::bind( &WebServer::OnSetTimeOnOff, this ) );

    on( "NTP",          std::bind( &WebServer::OnNtp, this ) );
    on( "SNTP",         std::bind( &WebServer::OnSetNtp, this ) );
    on( "TimeZone",     std::bind( &WebServer::OnTimeZone, this ) );
    on( "STimeZone",    std::bind( &WebServer::OnSetTimeZone, this ) );
    on( "MicroAdjust",  std::bind( &WebServer::OnMicroAdjust, this ) );
    on( "SMicroAdjust", std::bind( &WebServer::OnSetMicroAdjust, this ) );
    on( "WebReq",       std::bind( &WebServer::OnWebReq, this ) );
    on( "SWebReq",      std::bind( &WebServer::OnSetWebReq, this ) );
    
    // buttons
    on( "BNtp",         std::bind( &WebServer::OnSyncNtp, this ) );
    
    // actions
    //on( "popup",        std::bind( &WebServer::OnPopup, this ) );
    on( "forcedim",     std::bind( &WebServer::OnForceDim, this ) );
    on( "forceon",      std::bind( &WebServer::OnForceOn, this ) );
    on( "time",         std::bind( &WebServer::OnTime, this ) );

    // misc
    on( "stats",        std::bind( &WebServer::OnStats, this ) );
    on( "about",        std::bind( &WebServer::OnAbout, this ) );
    
    ESP8266WebServer::onNotFound ( std::bind( &WebServer::OnNotFound, this ) );

    static const char * headerkeys[] = { "Cookie" };
    ESP8266WebServer::collectHeaders( headerkeys, countof(headerkeys) );
    ESP8266WebServer::begin();
}


String
WebServer::arg( char name )
{
    char        str[ 2 ];

    str[ 0 ] = name;
    str[ 1 ] = 0;
    return ESP8266WebServer::arg( str );
}


bool
WebServer::IsIp( String str )
{
    for ( char c : str )
    {
        if (c != '.' && (c < '0' || c > '9'))
        {
            return false;
        }
    }

    return true;
}


void
WebServer::Redirect( String const &uri, bool sendSessionId )
{
    Redirect( (FSTR const *) uri.c_str(), sendSessionId );
}


void
WebServer::Redirect( FSTR const *uri )
{
    Redirect( uri, false );
}


void
WebServer::Redirect( FSTR const *uri, bool sendSessionId )
{
    if ( !_responseSent )
    {
        String  fullUri = PrintF( F("http://%s%s/"), client().localIP().toString().c_str(), uri );

        Out( F("WebServer: Redirect '%s' -> '%s'\n"), hostHeader().c_str(), fullUri.c_str() );
        sendHeader( "Location", fullUri, true );
        if ( sendSessionId )
        {
            sendHeader( "Set-Cookie", PrintF( F("ESPID=%d"), _clientId) );
        }

        Send( 302, "text/plain", "" );
        client().stop();
     }
}


void
WebServer::OnNotFound()
{
    ResetPageState();
    if ( !IsIp(hostHeader()) )
    {
        Redirect( F("/c") );
    }
    else
    {
        _content = PrintF( F("File Not Found\n\nURI: %s"), uri().c_str() );  

        SendHeader( F("Cache-Control"), F("no-cache, no-store, must-revalidate") );
        SendHeader( F("Pragma"), F("no-cache") );
        SendHeader( F("Expires"), F("-1") );
        SendHeader( F("Content-Length"), String( _content.length()).c_str() );
        Send ( 404, "text/plain", _content );
    }
}


void
WebServer::ResetPageState()
{
    _responseSent   = false;
    _content        = "";
    _titleScale     = 0;
    _formStarted    = false;
    _divStarted     = false;
    _isText         = false;
    _pingColorState = true;
    _buffer.Reset();
}


void
WebServer::SetTitle( FSTR const *title )
{
    SetTitle( title, 1 );
}


void
WebServer::SetTitle( FSTR const *title, uint8 scale )
{
    SetTitle( title, title, scale );
}


void
WebServer::SetTitle( FSTR const *title, FSTR const *pageName, uint8 scale )
{
    _titleScale = scale;

    AddF( F(
        "<!DOCTYPE html><html lang=\"en\">"
        "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=%d,\"/>"
        "<title>%s%s%s</title></head>"
        "<body onload='LX();LX2();'>"
            "<script>function LX(){}function LX2(){}</script>\n"
            "<h2>"
                "<font color='#2E9AFE'>%s%s%s%s</font>"
            "</h2>\n" ),
        scale,
        g_options._ssid, (ReadChar(title) ? F(": ") : F("")), title,
        g_options._prefixName, g_options._ssid, (ReadChar(pageName) ? F(": ") : F("")), pageName
    );
}


void
WebServer::AddBr()
{
    Add( "<br/>" );
}


void
WebServer::Add( String const &html )
{
    _buffer.Add( html.c_str() );
}


void 
WebServer::Add( char const *html )
{
    _buffer.Add( html );
}


void
WebServer::Add( FSTR const *html )
{
    _buffer.Add( html );
    yield();
}


void
WebServer::AddF( FSTR const *format, ... )
{
    va_list     args;
    
    va_start ( args, format );
    _buffer.VAddF( format, args );
    va_end( args );
    yield();
}


void
WebServer::AddF_br( FSTR const *format, ... )
{
    va_list     args;

    va_start ( args, format );
    _buffer.VAddF( format, args );
    va_end( args );
    AddBr();
    yield();
}


void
WebServer::AddClientForm( FSTR const *action )
{
    CheckClient();
    AddForm( action );
}


void
WebServer::AddForm( FSTR const *action )
{
    if ( !_titleScale )
    {
        SetTitle( action );
    }

    AddF( F("<form method='get' action='S%s'><p style='line-height:%d%%'>"), action, (_titleScale > 1 ? 100 : 200) );
    _formStarted = true;
}


void
WebServer::AddInput2_br( char id, int8 length, FSTR const *placeholder )
{
    AddF_br( F("<input id='%c' name='%c' length=%d placeholder='%s'/>"), id, id, length, placeholder );
}


void
WebServer::AddInputPassword_br( char id, int8 length, FSTR const *placeholder )
{
    AddF_br( F("<input type='password' id='%c' name='%c' length=%d placeholder='%s'/>"), id, id, length, placeholder );
}


void
WebServer::AddInput_br( char id, int8 length, FSTR const *value, FSTR const *desc )
{
    AddF( desc );
    AddInput_br( id, length, value );
}


void
WebServer::AddInput_br( char id, int8 length, FSTR const *value )
{
    AddInput( id, length, value );
    AddBr();
}


void
WebServer::AddInput( char id, int8 length, FSTR const *value )
{
    AddF( F("<input id='%c' name='%c' length=%d value='%s'/>"), id, id, length, value );
}


void 
WebServer::AddRadio( char id, FSTR const *value, FSTR const *desc, bool selected )
{
    AddF( F("<input name='%c' type='radio' value='%s' %s/>%s"), id, value, (selected ? "checked" : ""), desc );
}


void
WebServer::AddCheckbox( char id, char const *desc, bool selected )
{
    AddCheckbox( id, (FSTR const *) desc, selected );
}


void
WebServer::AddCheckbox( char id, FSTR const *desc, bool selected )
{
    AddF( F("<input name='%c' type='checkbox' %s/>%s"), id, (selected ? "checked" : ""), desc );
}


void
WebServer::AddCheckbox_br( char id, FSTR const *desc, bool selected )
{
    AddCheckbox( id, desc, selected );
    AddBr();
}


void
WebServer::AddOption( uint value )
{
    char        buffer[ 32 ];

    SPrintF( buffer, "%d", value );
    AddOption( value, (FSTR const *) buffer );
}


void
WebServer::AddOption( uint value, FSTR const *name )
{
    AddOption( value, (value == _selectionValue), name );
}


void
WebServer::AddOption( uint value, bool selected, FSTR const *name )
{
    AddF( F("<option value='%d' %s>%s</option>"), value, (selected ? F("selected") : F("")), name );
}


void
WebServer::AddTime( char id, FSTR const *desc, HourMinute hhmm )
{
    AddF( F("%s<input name='%c' type='time' value='%02d:%02d'/>"), desc, id, hhmm._hour, hhmm._minute );
}


void
WebServer::AddDateTime( FSTR const *desc, time_t value )
{
    tmElements_t            tm;

    breakTime( value, tm );
    AddF( F("<font id='f'>%s<input name='d' type='date' value='%04d-%02d-%02d'/>"), desc, tm.Year+1970, tm.Month, tm.Day );
    AddF( F("Time: <input name='t' type='time' value='%02d:%02d'/></font>"), tm.Hour, tm.Minute );
}


void
WebServer::AddLinkButton( FSTR const *uri )
{
    AddLinkButton( uri, uri );
}


void
WebServer::AddLinkButton( FSTR const *uri, FSTR const *desc )
{
    AddF( F("<button onclick='window.location.href=\"/%s\"'>%s</button> "), uri, desc );
}


void
WebServer::AddLinkDiv( FSTR const *text )
{
    AddLinkDiv( text, text );
}


void
WebServer::AddLinkDiv( FSTR const *uri, FSTR const *text )
{
    EndDiv();
    AddF( F("<a href='/%s'>%s:</a><div style='margin-left: 20px'>"), uri, text );
    _divStarted = true;
}


void
WebServer::EndDiv()
{
    if ( _divStarted )
    {
        AddF_br( F("</div>\n") );
        _divStarted = false;
    }
}


void
WebServer::EndForm()
{
    if ( _formStarted )
    {
        Add( F("<br/><button type='submit'>submit</button></form>") );
        _formStarted = false;
    }
}
    

void
WebServer::AddOnOff( const OnOff & onOff )
{
    if ( onOff._onDays )
    {
        if ( onOff._onDays == 0x7F )
        {
            Add( F("Everyday ") );
        }
        else
        {
            char const  * sep = "";

            for ( int dayOfWeek=0; dayOfWeek < 7; ++dayOfWeek )
            {
                if ( onOff._onDays & (1 << dayOfWeek) )
                {
                    AddF( F("%s%s"), sep, &k_daysOfWeek3[dayOfWeek*4] );
                    sep = ", ";
                }
            }
        }

        if ( onOff._onTime != onOff._offTime )
        {
            AddF( F(" between %s - %s"), onOff._onTime.toString().c_str(), onOff._offTime.toString().c_str() );
        }
    }
    else
    {
        Add( F("Disabled") );
    }
    AddBr();
    AddF_br( F("Currently is %s"), onOff.Desc( onOff.IsOn() ) );
}


void
WebServer::AddOnOffEdit( char os, FSTR const *desc, const OnOff &onOff )
{
    AddTime( os+0, desc, g_options._timeOnOff._onTime );
    AddTime( os+1, F(" and "), g_options._timeOnOff._offTime );
    AddBr();
    Add( F("On: ") );
    for (int index = 0; index < 7; ++index)
    {
        AddCheckbox( os+2+index, &k_daysOfWeek3[index*4], !!(onOff._onDays & (1 << index)) );
    }
    AddBr();
}


void
WebServer::ParseOnOff( const char os, OnOff *onOff )
{
    onOff->_onTime   = HourMinute( arg(os+0) );
    onOff->_offTime  = HourMinute( arg(os+1) );

    onOff->_onDays = 0;
    for ( int index = 0; index < 7; ++index )
    {
        onOff->_onDays |= arg( os+2+index ).length() ? (1 << index) : 0;
    }
}



void
WebServer::SendPage()
{
    if ( _snappedOptions )
    { 
        if ( memcmp(_snappedOptions, &g_options, sizeof(g_options)) )
        { 
            g_options.Save();
        }

        delete _snappedOptions;
        _snappedOptions = nullptr;

        Redirect( _snapRedir );
    }
    else if ( !_isText )
    {
        EndDiv();
        EndForm();
        
        if ( _titleScale )
        {
            Add( F("</body></html>") );
        }

        _buffer.Flush();
        SendHeader( F("Content-Length"), String( _content.length() ) );
        Send( 200, "text/html", _content );
    }
    else
    {
        _buffer.Flush();
        SendHeader( F("Content-Length"), String( _content.length() ) );
        Send( 200, "text/plain", _content );
    }

    // if ( _pingColorState )           << REMOVE
    // {
    //     g_globalColor.PingState( GlobalColor::ClientRequest );
    // }
}


void 
WebServer::SendHeader( FSTR const *name, const String &value )
{
    if ( !_responseSent )
    {
        ESP8266WebServer::sendHeader( name, value );
    }
}


void
WebServer::Send( int code, char const * contentType, const String& content )
{
    Send( code, contentType, content.c_str() );
}


void
WebServer::SendHeader( FSTR const *name, const char *value )
{
    SendHeader( name, PrintF( "%s", value ) );
}


void
WebServer::SendHeader( FSTR const *name, const FSTR *value )
{
    SendHeader( name, (char const *) value );
}


void 
WebServer::Send( int code, char const * contentType, char const *content )
{
    if ( !_responseSent )
    {
        ESP8266WebServer::send_P( code, contentType, content );
        _responseSent = true;
        _responses += 1;
    }
}


void
WebServer::SnapOptions( FSTR const *redir )
{
    CheckClient();
    _snapRedir = redir;
    if ( !_snappedOptions )
    {
        _snappedOptions = new Options();
    }

    memcpy( _snappedOptions, &g_options, sizeof(g_options) ); 
}


void
WebServer::CheckClient()
{
    if ( g_options._password[0] )
    {
        bool    redirect    = true;

        if ( _clientId )
        {
            String      cookie      = ESP8266WebServer::header( "Cookie" );
            int const   pos         = cookie.indexOf( "ESPID=" );

            if ( pos >= 0 )           
            {
                uint const  id      = atoi( cookie.c_str() + 6 );

                if ( id == _clientId )
                {
                    redirect = false;
                }
            }
        }

        if ( redirect )
        {
            Redirect( PrintF(F("/p?u=%s"), UrlEncode(uri()).c_str()), false );
        }
    }
}


void
WebServer::OnGetPassword()
{
    _clientId = 0;

    SetTitle( F("Login") );
    AddForm( F("p") );
    AddInput_br( 'p', 8, F(""), F("password: ") );
    AddF( F("<input name='u' value='%s' hidden/>"), arg('u').c_str() );
}


void
WebServer::OnSetPassword()
{
    String      p = arg( 'p' );

    if ( p == g_options._password )
    {
        randomSeed( g_ms ^ random(0x7FFFFFFF) );
        _clientId = random( 0x7FFFFFFF );
        Redirect( arg('u'), true );
    }
    else
    {
        Redirect( F("") );
    }
}


///////////////////////////////////////
//
//
//


void
WebServer::OnRoot()
{
    // any warnings
    {
        char   * addBr = "";

        auto add =
            [this, &addBr]( FSTR const *text )
            {
                AddF_br( text );
                addBr = "<br/>";
            };

        Add( F("<font color='red'><b>") );
        if ( g_ntp.GetState() != NtpClient::WaitingForSyncTime )
        {
            add( F("NTP time not synced") );
        }
            
        if ( g_timeZone.GetState() != TimeZone::WaitingForSyncTime )
        {
            add( F("TimeZone not synced") );
        }

        if ( g_globalState.IsDisplaying() == GlobalState::TimeOff )
        {
            add( F("Time is off") );
        }

        AddF( F("</b>%s</font>"), addBr );
    }

    {
        AddLinkDiv( F("Settings") );
    }

    {
        AddLinkDiv( F("TOnOff"), F("Display Time") );
        AddOnOff( g_options._timeOnOff );
    }


    EndDiv();
    Add( F("<a href='r2'>More Settings</a><br><br>") );

    AddLinkButton( F("stats") );
    AddBr();
    AddBr();
}


void
WebServer::OnRoot2()
{
    SetTitle( F("More") );
   
    {
        AddLinkDiv( F("NTP") );
        // AddColorEffect( g_options._ntpColor );
        AddF_br( F("Sync at %s then every %s"), g_options._ntpSync.toString().c_str(), Duration(g_options._ntpFrequency*10*60).c_str() );
        AddF_br( F("Last sync: %s"), g_ntp.LastSync().c_str() );
    
        AddLinkDiv( F("TimeZone") );
        if ( g_options._tzKey[0] )
        {
            // AddColorEffect( g_options._tzColor );
        }
        else
        {
            AddF_br( F("Manual") );
        }
    }
    
    {
        AddLinkDiv( F("MicroAdjust") );            // link.. enable/disable.  smoothing data.  history data
        if ( g_options._madjEnable )
        {
            int32   madj = g_options._madjFreq * g_options._madjDir;
    
            AddF_br( F("%s seconds per day"), MadjSecondsPerDay( madj ).c_str() );
            AddF_br( F("Last adjust: %s"), LastMicroAdjust().c_str() );
        }
        else
        {
            AddF_br( F("disabled") );
        }
    }
    
    {
        AddLinkDiv( F("WebReq"), F("Web Request") );
        // AddColorEffect( g_options._httpClient );
        AddF( F("AP %srunning"), (WiFi.getMode() & WIFI_AP) ? F("") : F("not-") );
    
        if ( (g_options._accessPointLifespan != 0) && (g_options._accessPointLifespan != 255) )
        {
            AddF_br( F(", turn off after %s"), Duration(g_options._accessPointLifespan*60).c_str() );
        }
        if ( WiFi.getMode() & WIFI_AP )
        {
            AddF_br( F("IP: %s"), WiFi.softAPIP().toString().c_str() );
        }
    
        AddBr();
    }

    EndDiv();
    AddLinkButton( F("BNtp"), F("Sync Ntp") );
    AddLinkButton( F("about") );

}


void
WebServer::OnSyncNtp()
{
    SnapOptions( F("/r2") );
    g_ntp.ForceSync();
}


void
WebServer::OnSettings()
{
    auto addSlider =
        [this]( FSTR const *desc, char id, uint8 level )
        {
            AddF_br( F("%s: <input type='range' min='0' max='255' value='%d' name='%c'/>"), desc, level, id );
        };

    AddClientForm( F("Settings") );

    addSlider( F("Bright"), 't', g_options._bright );
    addSlider( F("Dim"), 'u', g_options._dim );
}


void
WebServer::OnSetSettings()
{
    SnapOptions( F("") );
    g_options._bright   = arg('t').toInt();
    g_options._dim      = arg('u').toInt();

    Out( F("Dim: %d, Bright: %d\n"), g_options._dim, g_options._bright );
}


void
WebServer::OnTimeOnOff()
{
    SetTitle( F("Display Time") );
    AddClientForm( F("TOnOff") );

    // 0=on time, 1=off time, 2,3,4,5,6,7,8,9 = sun-sat
    AddOnOffEdit( '0', F("On between"), g_options._timeOnOff );
}


void
WebServer::OnSetTimeOnOff()
{
    SnapOptions( F("") );
    ParseOnOff( '0', &g_options._timeOnOff );
}



void       
WebServer::OnNtp()
{
    AddClientForm( F("NTP") );

    // s - ntp server
    // t - sync time
    // f - frequency
    // e - highlight errors
    // (e,h,z - in AddColorEffectEdit)

    AddInput_br     ( 's', sizeof(g_options._ntpServer), (FSTR const *) g_options._ntpServer, F("NTP Server: ") );

    AddTime( 't', F("Sync at "), g_options._ntpSync );
    AddF( F(" every <select name='f'>") );
    _selectionValue = g_options._ntpFrequency;
    AddOption(   1, F("10 minutes") );
    AddOption(   3, F("30 minutes") );
    AddOption(   6, F("1 hour") );
    AddOption(  24, F("4 hours") );
    AddOption(  72, F("12 hours") );
    AddOption( 144, F("day") );
    AddF_br( F("</select>") );
    AddBr();

    AddBr();
}


void       
WebServer::OnSetNtp()
{
    SnapOptions( F("/r2") );
    strncpy( g_options._ntpServer, arg( 's' ).c_str(), sizeof(g_options._ntpServer) );
    g_options._ntpSync = HourMinute( arg( 't' ) );
    g_options._ntpFrequency = arg( 'f' ).toInt();
  //g_options._ntpColor = GetColorEffect();
}


void       
WebServer::OnTimeZone()
{
    AddClientForm( F("TimeZone") );

    Add(F(
        "<script>"
            "function LX2(){"
                "m=D('u');"
                "if(m.checked){a='visible';b='hidden';}else{a='hidden';b='visible';}"
                "V('ba',a);V('bb',b);"
            "}"
        "</script>" ));

    AddF( F("<input name='u' id='u' type='checkbox' onchange='LX2()' %s/>Enable (p-api.com &amp; api.timezonedb.com)"), (g_options._tzKey[ 0 ] ? "checked" : "") );
    Add ( F("<div id='ba'>") );
    AddInput_br( 'k', sizeof(g_options._tzKey), (FSTR const *) g_options._tzKey, F("timezonedb api key: ") );

    Add( F("</div><div id='bb'>") );
        AddDateTime( F("Enter current date &amp; time for manual gmt offset<br>"), g_now );     // adds 'f', 't' and 'd' (font, time, date)
    Add( F("</div><br>") );

    AddBr();
}


void       
WebServer::OnSetTimeZone()
{
    SnapOptions( F("/r2") );
    const bool      enabled     = !!arg('u').length();
    String          dateStr     = arg('d');
    char const    * date        = dateStr.c_str();
    HourMinute      hhmm ( HourMinute(arg('t')) );
    tmElements_t    tm;

    strncpy( g_options._tzKey, arg( 'k' ).c_str(), sizeof( g_options._tzKey ) );
  //g_options._tzColor = GetColorEffect();

    if ( !enabled )
    {
        g_options._tzKey[ 0 ] = 0;
        g_options._gmtOffset = 0;

        // 1234-67-89
        if ( dateStr.length() == 10 )
        {
            tm.Year     = atoi( date ) - 1970;
            tm.Month    = atoi( date+5 );
            tm.Day      = atoi( date+8 );
            tm.Hour     = hhmm._hour;
            tm.Minute   = hhmm._minute;
            tm.Second   = 0;

            g_options._gmtOffset = ( makeTime(tm) - g_gmtTime );
            Out( F("WebServer: TzManual: gmtOffset %d\n"), g_options._gmtOffset );
        }
    }
}


void
WebServer::OnMicroAdjust()
{
    const int32  madj0 = g_options._madjFreq * g_options._madjDir;

    AddClientForm( F("MicroAdjust") );

    // todo.. make manual allow setting of time
    AddCheckbox_br  ( 'e', F("Enable"), g_options._madjEnable );
    AddF_br( F("Adjust %s seocnds per day (%dms per %d)"), MadjSecondsPerDay(madj0).c_str(), g_options._madjDir, g_options._madjFreq );
    EndForm();
    AddF_br( F("Log:") );
    LogToHtml( [this]( char const *txt ) { Add( txt ); } );
    AddBr();
}


void
WebServer::OnSetMicroAdjust()
{
    SnapOptions( F("/r2") );
    g_options._madjEnable = !!arg( 'e' ).length();
}


void       
WebServer::OnWebReq()
{
    SetTitle( F("Web Request") );
    AddClientForm( F("WebReq") );

    AddF( F("Turn Access Point off in:  <select name='l'>") );
    _selectionValue = g_options._accessPointLifespan;
    AddOption(   0, F("0 minutes") );
    AddOption(   1, F("1 minute") );
    AddOption(   5, F("5 minutes") );
    AddOption(  60, F("60 minutes") );
    AddOption( 255, F("Never") );
    AddF_br( F("</select>") );
    AddBr();

    AddInput_br( 'w', sizeof( g_options._password ), (FSTR const *) g_options._password, F("Set new password for updating settings: ") );
    AddBr();
}


void       
WebServer::OnSetWebReq()
{
    SnapOptions( F("/r2") );
    g_options._accessPointLifespan  = arg( 'l' ).toInt();
    strncpy( g_options._password, arg('w').c_str(), sizeof( g_options._password ) );
}


// void
// WebServer::OnCreatePopup()
// {
//     ARGB        argb = { (ARGB::BlendInOut | 3), 0xFF, 0xA5, 0x00 };
// 
//     AddClientForm( "Popup" );
//     AddInput_br( 'd', NUM_DIGITS, "", "Digits: " );
//     AddColorEffectEdit( argb );
// }
// 
// 
// void
// WebServer::OnSetPopup()
// {
//     const ARGB      argb    = GetColorEffect();
//     int             pos     = 0;
//     char            digits[ NUM_DIGITS + 1 ];
//     String          url;
// 
//     SetTitle( "PopupUrl" );
//     
//     for ( char d : arg('d') )
//     {
//         if ( d < '0' || d > '9' )
//         {
//             d = 'x';
//         }
// 
//         digits[ pos ] = d;
//         pos += 1;
//         if ( pos >= NUM_DIGITS )
//         {
//             break;
//         }
//     }
//     digits[ pos ] = 0;
// 
//     url = PrintF(
//         "http://%s/popup?d=%s&e=%s&h=%s&c=%s",
//             client().localIP().toString().c_str(),
//             digits,
//             arg( 'e' ).c_str(),
//             arg( 'h' ).c_str(),
//             argb.toRgbString().c_str()
//         );
//         
//     AddF( "URL: <a href='%s'>%s</a>", url.c_str(), url.c_str() );
// 
//     _pingColorState = false;
//     Popup( digits, argb );
// }
// 
// 
// void
// WebServer::OnPopup()
// {
//     String          digits  = arg( 'd' );
//     const ARGB      argb    = GetColorEffect();
// 
//     _pingColorState = false;
//     _isText = true;
//     if ( g_options._allowPopupUrl )
//     { 
//         Popup( digits.c_str(), argb );
//         AddF( "Popup( %s, %s )\n", digits.c_str(), argb.toString().c_str() );
//     }
//     else
//     {
//         Add( "disabled\n" );
//     }
// }


void
WebServer::OnForceOn()
{
    _pingColorState = false;
    _isText = true;
    Add( g_options._timeOnOff.ForceOnOff( arg('s') ) );
}


void
WebServer::OnForceDim()
{
    _pingColorState = false;
    _isText = true;
    Add( g_options._dimOnOff.ForceOnOff( arg('s') ) );
}


void
WebServer::OnTime()
{
    _pingColorState = false;
    _isText = true;
    Add( TimeStr().c_str() );
}


void
WebServer::OnStats()
{
    SetTitle( F("Stats") );

    Console::MiscStats(
        [this]( char const *line )
        {
            Add( line );
        } );
}


void
WebServer::OnAbout()
{
    SetTitle( F("About") );

    Console::MiscAbout(
        [this]( char const *line )
        {
            Add( line );
        } );
    AddBr();
    AddBr();
}


