//
// Created by ashish on 4/10/16.
//

#ifndef MYGAME_LOADINGSCREEN_HPP
#define MYGAME_LOADINGSCREEN_HPP

#include "cocos2d.h"




class GameManager;




//TODO add support for loading stuff
//TODO per module based
class LoadingScreen :public cocos2d::Layer{
private:

    struct Loader{

        //graphics here
        std::vector<std::string> spriteSheets;
        std::vector<std::pair<std::string,cocos2d::Texture2D::PixelFormat> >textures;
        //audio effects
        std::vector<std::string> backgroundeffects;
        std::vector<std::string> effects;
        //other vectors here
        std::vector<std::pair<std::string,std::string> > jsonRubeStuff;







        bool step();

    private:

        bool done = false;
        size_t remainingSpriteSheets;
        size_t remainingTextures;
        size_t remainingBackgroundEffects;
        size_t remainingEffects;

    };


public:
    static cocos2d::Scene * createScene(GameManager * manager);
    static LoadingScreen  * create(GameManager * manager);
    virtual bool init (GameManager* manager);






private:
    void loadData(float delta);
    void onDone();

private:
    GameManager * manager = nullptr;
    Loader loader;






};







#endif //MYGAME_LOADINGSCREEN_HPP
