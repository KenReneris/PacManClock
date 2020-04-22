/*
 * PacMan.cpp
 *  PacMan animations
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

namespace PacMan
{
    class ChasePacMan
        : public IStoryboard
    {
    public:
        ChasePacMan();

        StartResult     Start() override;
        void            Loop() override;

    private:
        Sprite          _pacMan;
        Sprite          _ghost[ 4 ];
    };


    /////////////////////////////////
    //
    //
    //


    class ChaseGhost
        : public IStoryboard
    {
    public:
        ChaseGhost();

        StartResult     Start() override;
        void            Loop() override;

    private:
        Sprite          _pacMan;
        Sprite          _ghost[ 4 ];
    };



    /////////////////////////////////
    //
    //
    //


    class CatchPacMan
        : public IStoryboard
    {
    public:
        CatchPacMan();

        StartResult     Start() override;
        void            Loop() override;

    private:
        uint8           _step;
        Sprite          _pacMan;
        Sprite          _pacManCaught;
        Sprite          _ghost[ 2 ];
    };



    /////////////////////////////////
    //
    //


    class CatchGhost
        : public IStoryboard
    {
    public:
        CatchGhost();

        StartResult     Start() override;
        void            Loop() override;

    private:
        uint8           _step;
        uint8           _frozen;
        uint16          _start;
        Sprite          _pacMan;
        Sprite          _ghost[ 4 ];
        Sprite          _ghostEyes[ 4 ];
    };



    /////////////////////////////////
    //
    //


    class GhostEyes
        : public IStoryboard
    {
    public:
        GhostEyes();

        StartResult     Start() override;
        void            Loop() override;

    private:
        Sprite          _ghostEyes[ 2 ];
    };
}


