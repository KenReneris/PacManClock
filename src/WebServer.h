/*
 * WebServer.h
 *  Small web page server
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

class WebServer : public ESP8266WebServer
{
public:
    WebServer();

    uint32              ResponsesSent() const;

    void                on( char const *uri, THandlerFunction handler );
    void                on( char const *uri, bool ap, THandlerFunction handler );
    void                begin();
    String              arg( char name );

    // Construct html response
    void                ResetPageState();
    void                CheckClient();
    void                SetTitle( FSTR const *title );
    void                SetTitle( FSTR const *title, uint8 scale );
    void                SetTitle( FSTR const *title, FSTR const *pageName, uint8 scale );
    void                AddBr();
    void                Add( char const *html );
    void                Add( FSTR const *html );
    void                Add( String const &html );
    void                AddF( FSTR const *format, ... );
    void                AddF_br( FSTR const *format, ... );
    void                AddForm( FSTR const *action );
    void                EndForm();
    void                AddClientForm( FSTR const *action );
    void                AddInput( char id, int8 length, FSTR const *value );
    void                AddInput_br( char id, int8 length, FSTR const *value );
    void                AddInput_br( char id, int8 length, FSTR const *value, FSTR const *desc );
    void                AddInput2_br( char id, int8 length, FSTR const *placeholder );
    void                AddInputPassword_br( char id, int8 length, FSTR const *placeholder );
    void                AddRadio( char id, FSTR const *value, FSTR const *desc, bool selected );
    void                AddCheckbox( char id, char const *desc, bool selected );
    void                AddCheckbox( char id, FSTR const *desc, bool selected );
    void                AddCheckbox_br( char id, FSTR const *desc, bool selected );
    void                AddOption( uint value );
    void                AddOption( uint value, FSTR const *name );
    void                AddOption( uint value, bool selected, FSTR const *name );
    void                AddTime( char id, FSTR const *desc, HourMinute value );
    void                AddDateTime( FSTR const *desc, time_t value );
    void                AddLinkButton( FSTR const *uri );
    void                AddLinkButton( FSTR const *uri, FSTR const *desc );
    void                AddOnOff( const OnOff & onOff );
    void                AddOnOffEdit( char os, FSTR const *desc, const OnOff &onOff );
    void                ParseOnOff( const char os, OnOff *onOff );
    void                AddLinkDiv( FSTR const *str );
    void                AddLinkDiv( FSTR const *uri, FSTR const *text );

    void                AddCachingAllowed();
    void                SendPage();

private:
    static bool         IsIp( String str );

private:
    void                SendHeader( FSTR const *name, const String &value );
    void                SendHeader( FSTR const *name, const char *value );
    void                SendHeader( FSTR const *name, const FSTR *value );
    void                Send( int code, char const * contentType, const String& content );
    void                Send( int code, char const * contentType, char const *content );
    void                Redirect( String const &uri, bool sendSessionId );
    void                Redirect( FSTR const *uri );
    void                Redirect( FSTR const *uri, bool sendSessionId );

    void                OnNotFound();
    void                EndDiv();

private:
    void                SnapOptions( FSTR const *redir );

    // misc page handlers
    void                OnRoot();
    void                OnRoot2();
    void                OnGetPassword();
    void                OnSetPassword();
    void                OnSettings();
    void                OnSetSettings();
    void                OnTimeOnOff();
    void                OnSetTimeOnOff();
    void                OnDimOnOff();
    void                OnSetDimOnOff();
    void                OnTopOfHour();
    void                OnSetTopOfHour();
    void                OnQuarterOfHour();
    void                OnSetQuarterOfHour();
    void                OnShowDate();
    void                OnSetShowDate();
    void                OnNtp();
    void                OnSetNtp();
    void                OnTimeZone();
    void                OnSetTimeZone();
    void                OnMicroAdjust();
    void                OnSetMicroAdjust();
    void                OnWebReq();
    void                OnSetWebReq();
    void                OnForceDim();
    void                OnForceOn();
    void                OnTime();
    void                OnStats();
    void                OnAbout();
    void                OnSyncNtp();

private:
    static char const   k_daysOfWeek3[];

private:
    uint                _responses;                 // misc stat
    uint32              _clientId;                  // if login required
                     
    // per-request
    StringBuffer        _buffer;                    // place to build some output before reallocating the content string
    bool                _responseSent;              // only send 1 response per request
    bool                _pingColorState;            // ping g_globalColor that a web request arrived
    uint8               _titleScale;                // non-zero if title has been set
    bool                _formStarted;               // true if a form was started (and </form> is needed)
    bool                _divStarted;                
    bool                _isText;                    // _content is a text response
    String              _content;                   // the response (right now is 1 big string?)
    Options           * _snappedOptions;            // if processing a form, this is the before state of the options
    FSTR const        * _snapRedir;                 // if processing a form, this is the page to redirect the client too
    uint                _selectionValue;            // used for making selection values easier
};



