/*
 * Mario.h
 *  Mario animations
 *
 * Author: Ken Reneris <https://github.com/KenReneris>
 * MIT License
 * ----------------------------------------------------------
 */

namespace Mario
{
    class RunBy
        : public IStoryboard
    {
    public:
        RunBy();
    
        StartResult     Start() override;
        void            Loop() override;
    
    private:
        Sprite          _mario;
    };
    

    class RunBack
        : public IStoryboard
    {
    public:
        RunBack();
    
        StartResult     Start() override;
        void            Loop() override;
    
    private:
        Sprite          _mario;
    };
}


