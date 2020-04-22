/*
 * Columns.h
 *  Output column/table data
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */


class Columns
{
public: 
    static const uint       k_maxCols = 3;

    enum class Fill : uint8
    {
        None,
        Left,
        Right,
    };

public:
    Columns( uint indent, char const *fill, char const sep, std::function<void( char const * )> const &htmlTable );
    ~Columns();

    void        SetPass( uint pass );
    void        Row( FSTR const *str );
    void        Row2( FSTR const *c0, char const * c1 );
    void        Row2( FSTR const *c0, FSTR const * c1 );
    void        Row2( FSTR const *c0, String const &c1 );
    void        Row2( FSTR const *c0, uint c1 );
    void        Row2( char const *c0, Fill fill, FSTR const * c1 );
    void        Row2( FSTR const *c0, Fill fill, FSTR const * c1 );
    void        Row2( FSTR const *c0, Fill fill, String const & c1 );
    void        Row2Flag( FSTR const *c0, bool c1 );

  //void        Row3a( FSTR const *c0, char const *link );
  //void        Row3a( FSTR const *c0, char const *c1, char const *link );
  //void        Cella( uint col, char const *link );            // html link

    void        Cell( uint col, Fill fill, FSTR const *txt );

private:
    void        NewLine();

private:
    char const              * const _fill;
    char const                      _sep;
    uint                            _pass;
    uint                            _indent;
    uint                            _width[ k_maxCols ];
    FSTR const                    * _htmlRowEnd;
    bool                            _isHtml;
    uint8                           _row;
    StringBuffer                    _buffer;
};


