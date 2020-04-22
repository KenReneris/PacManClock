/*
 * GlobalState.cpp
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


#include "platform.h"


GlobalState::GlobalState()
    : _states       ( 0 )
{
}


void
GlobalState::EnableState( State state, bool enabled )
{
    if ( enabled )
    {
        EnableState( state );
    }
    else
    {
        ClearState( state );
    }
}



void  
GlobalState::EnableState( State state )
{
    _states |= ( 1 << state );
}


void 
GlobalState::ClearState( State state )
{
    _states &= ~( 1 << state );
}


GlobalState::State
GlobalState::IsDisplaying() const
{
    return State( FirstSetBit8( _states ) );
}


uint32
GlobalState::IsPending( State state ) const
{
    return (_states & (1 << uint( state )));
}




