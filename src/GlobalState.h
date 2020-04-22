/*
 * GlobalState.h
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


class GlobalState
{
public:
    // stats are in order of priority.  e.g., Date will display before TopOfHour effect
    enum State : uint8
    {
        TimeNotSet          = 1,
        TimeOff             = 2,        // if in this state, displaying the time (& date) is surpressed.
        MaxState
    };

public:
    GlobalState();

    void            EnableState( State state, bool enabled );   // calls either Enable or Clear depending on "enabled"
    void            EnableState( State state );
    void            ClearState( State state );

    State           IsDisplaying() const;               // MSB state active
    uint32          IsPending( State state ) const;     // non-zero if the "state" is set.  Set but might not be displaying if a higher priority state is also set.

private:
    uint32          _states;                            // states being held on
};


extern GlobalState      g_globalState;


