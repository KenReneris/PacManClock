/*
 * WiFiAp
 *  WiFi Access Point
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"
#include "WebServer.h"
#include "WiFiAp.h"


void
WiFiAp::Setup()
{
    _status = wl_status_t(-1);
    _wlSurpress = 0xFF;

    WiFi.enableAP( false );
    WiFi.hostname( g_options._ssid );
    WiFi.softAP( g_options._ssid );
    Out( F("SoftAp: %s\n"), WiFi.softAPIP().toString().c_str() );

    _dnsServer.setErrorReplyCode( DNSReplyCode::NoError );

    _server.on( "c",        false, std::bind( &WiFiAp::OnRoot, this ) );
    _server.on( "SConnect", false, std::bind( &WiFiAp::OnConnect, this ) );
    _server.on( "Scan",     false, std::bind( &WiFiAp::OnScan, this ) );
    _server.begin(); 

    UpdateNextTime();

    WiFi.begin( "Kens iPhone", "PasswordGoesHere" );
}


WebServer & 
WiFiAp::Server()
{
    return _server;
}


void
WiFiAp::UpdateNextTime()
{
    if ( (WiFi.hostname() != g_options._ssid) && (_isOn) )
    {
       Out( F("SoftAp: Disable\n") );
        WiFi.enableAP( false );
        _isOn = false;
    }
    
    _offTime = 0;
    if ( g_options._accessPointLifespan != 255 )
    {
        _offTime = g_options._accessPointLifespan * 60 + 1;
        Out( F("SoftAp: off in %d seconds\n"), _offTime - g_poweredOnTime );
    }
}


void
WiFiAp::Loop()
{
    const bool      isOn    = ( (!_offTime) || (g_poweredOnTime < _offTime)  );

    yield();
    if ( isOn != _isOn )
    {
        _isOn = isOn; 
        if ( isOn )
        {
            //const ARGB      color = { ARGB::Flash | 3, 0xFF, 0xFF, 0xFF };
    
            WiFi.hostname( g_options._ssid );
            WiFi.softAP( g_options._ssid );
            WiFi.enableAP( true );
            _dnsServer.start( 53, "*", WiFi.softAPIP() );
            Out( F("SoftAp: Start: %s, %s\n"), g_options._ssid, WiFi.softAPIP().toString().c_str() );
        }
        else
        {
            // dns server stop?
            WiFi.enableAP( false );
            Out( F("SoftAp: Disable\n") );
        }
    }
   
    if ( (!g_wifiIsConnected) && (_reconnect) && (g_poweredOnTime > _reconnect) )
    {
        // retry at least every 15 minutes (autoreconnect seems to be slower)
        _reconnect = g_poweredOnTime + (15 * 60);
        WiFi.begin();
    }

    UpdateWiFiStatus();
    _dnsServer.processNextRequest();
    _server.handleClient();
}


void
WiFiAp::UpdateWiFiStatus()
{
    wl_status_t     status      = WiFi.status();
    bool            showStatus  = false;

    if ( _status != status )
    {
        if ( !(_wlSurpress & (1<<status)) )
        {
            showStatus = true;
            _wlSurpress |= (1 << status);
            Out( F("WiFi: %s\n"), WlStatus2Str(status) );
        }

        g_wifiIsConnected = false;
        switch ( status )
        {
        case WL_CONNECTED: 
            if ( !WiFi.getAutoConnect() )
            {
                WiFi.setAutoConnect( true );
            }
            
            Out( F( "WiFi: %s\n" ), WiFi.localIP().toString().c_str() );
            Marquee::Show( F("%s\n"), WiFi.localIP().toString().c_str() );
            showStatus = false;
            ScanDelete();
            g_wifiIsConnected = true;
            _wlSurpress = 0;
            _reconnect = 1;
            break;
        }

        if ( showStatus ) 
        {
            Marquee::Show( F("%s\n"), WlStatus2Str(status) );
        }

        _status = status;
    }
}


void
WiFiAp::ConnectNow()
{ 
    Out( F("WiFi: begin()\n") );
    _wlSurpress = 0;
    WiFi.begin();
}



FSTR const *
WiFiAp::WlMode2Str( uint8 mode )
{
    switch ( mode )
    {
    case WIFI_OFF:      return F("OFF");
    case WIFI_STA:      return F("STA");
    case WIFI_AP:       return F("AP");
    case WIFI_AP_STA:   return F("AP+STA");
    }

    return nullptr;
}


FSTR const * 
WiFiAp::WlStatus2Str( uint8 status )
{
    switch ( status )
    {
    case WL_IDLE_STATUS:        return F("idle");
    case WL_NO_SSID_AVAIL:      return F("No SSID");
    case WL_CONNECTED:          return F("Connected");
    case WL_CONNECT_FAILED:     return F("Connect Failed");
    case WL_CONNECTION_LOST:    return F("Connection Lost");
    case WL_DISCONNECTED:       return F("Disconnected");
    }

    return nullptr;
}


bool
WiFiAp::WiFiConnected()
{
    if ( g_wifiIsConnected )
    {
        _server.AddF( F("Connected: %s<br/>"), WiFi.SSID().c_str() );
        _server.AddF( F("IP: %s<br/>"), WiFi.localIP().toString().c_str() );
        return true;
    }

    return false;
}


void
WiFiAp::ScanStart()
{
    if ( !_isScanning )
    {
        ScanDelete();
        WiFi.scanNetworks( true );      // start scanning 
    }
}


void
WiFiAp::ScanDelete()
{
    _isScanning = false;
    WiFi.scanDelete();
}


void
WiFiAp::OnRoot()
{
    String          name = _server.arg( 'n' );

    _server.SetTitle( F(""), 2 );

    if ( !WiFiConnected() )
    {
        ScanStart();
        _server.Add( F("Connect to WiFi<br>") );
        _server.AddForm( F("Connect") );
        if ( name.length() )
        {
            _server.AddInput_br( 'n', 32, (FSTR const *) name.c_str() );
        }
        else
        {
            _server.AddInput2_br( 'n', 32, F("SSID") );
        }
        _server.AddInputPassword_br( 'p', 32, F("password") );
        _server.EndForm();
        _server.AddLinkButton( F("Scan"), F("scan") );
    }
}


void
WiFiAp::OnConnect()
{
    _server.SetTitle( F("Connect") );

    if ( !WiFiConnected() )
    {
        String  name        = _server.arg( 'n' );
        String  password    = _server.arg( 'p' );

        //Popup1x6( 1, color );

        _wlSurpress = 0;
        WiFi.begin( name.c_str(), password.c_str() );
        _server.AddF( F("Attempting to connect to: %s<br/>Check Clock"), name.c_str() );

        // sometimes the first connect doesn't work.. queue on a quick retry
        _reconnect = g_poweredOnTime + 10;
    }
}


void
WiFiAp::OnScan()
{
    auto addLink = 
        [this]( const String &ssid )
        {
            _server.AddF( F("<a href='c?n=%s'>%s</a><br>"), UrlEncode(ssid).c_str(), ssid.c_str() );
        };

    int         max;
    const int   start = millis();
    
    do
    {
        max = WiFi.scanComplete();
        if ( max >= 0 )
        {
            break;
        }
        delay( 1 );
    }
    while( (millis() - start) < 3*1000 );

    _server.SetTitle( F("Scan") );
    if ( max < 0 )
    {
        _server.AddF( F("still scanning") );
    } 
    else 
    {
        _server.AddF_br( F("Found %d WiFis:"), max );
        for ( int index=0; index < max; ++index )
        {
            String  ssid    = WiFi.SSID( index );
            bool    isDup   = false;

            for ( int j=0; j < index; ++j )
            {
                if ( ssid == WiFi.SSID(j) )
                {
                    isDup = true;
                    break;
                }
            }

            if ( !isDup )
            {
                addLink( ssid );
            }
        }
    }
}


    