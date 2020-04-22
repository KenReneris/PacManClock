/*
 * platform.h
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

 /*
 * Platform.h
 *  Global includes
 *  Global macros
 *  Global globals...
 *
 */


#include <Arduino.h> 

#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <TimeLib.h>            // https://github.com/PaulStoffregen/Time

//#define double_buffer
#include <PxMatrix.h>
#include <Ticker.h>

#define PANEL_WIDTH     32
#define PANEL_HEIGHT    16
    
#define GPIO_FORCE_TIMEON       0       // 5       // D1    (0 == disabled)
#define GPIO_FORCE_TIMEOFF      0       // 4       // D2

#define FASTLED_INTERNAL        // remove various compile time debug output
#include <FastLED.h>            // https://github.com/FastLED/FastLED

// simple types
using   int8    = int8_t;
using   int16   = int16_t;
using   int64   = int64_t;

using   uint8   = uint8_t;
using   uint16  = uint16_t;

using   uint    = unsigned int;

using   FSTR    = __FlashStringHelper;

// simple macros
#define countof(a)      ( (int) ( sizeof(a)/sizeof(a[0]) ) )
#define ABS(a)          ( ((a) < 0) ? (-a) : (a) )
#define MAX(a,b)        ( (a) > (b) ? (a) : (b) )
#define MIN(a,b)        ( (a) < (b) ? (a) : (b) )

template<typename TYPE>    
TYPE
Sign( const TYPE &v )
{     
    return ( v ? ( v < 0 ? -1 : +1 ) : 0 );     
}


using   std::function;
extern void OutStr( char const *str );
extern void MemCpy( void *dst, void const *src, size_t size );


// our common headers
#include "Debug.h"
#include "Time.h"
#include "Bits.h"
#include "Point.h"
#include "Range.h"
#include "EnumStr.h"
#include "ZString.h"
#include "Log.h"
#include "Argb.h"
#include "HourMinute.h"
#include "OnOff.h"
#include "TimeOnOff.h"
#include "DimOnOff.h"
#include "Smooth.h"
#include "GlobalState.h"

#include "IStoryboard.h"
#include "Options.h"

#include "Draw.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "Particles.h"
#include "Digit.h"
#include "Clock.h"
#include "Marquee.h"
#include "AnimationManager.h"

// globals
extern volatile uint64  g_frameCount;           // number of frames (at 60fps)
extern bool             g_wifiIsConnected;

extern uint16           g_black;

extern bool             IsInit();
extern void             PrintTime();
extern void             OutNl();
extern void             UpdateWaitTimes();
extern String           TimeStr();

