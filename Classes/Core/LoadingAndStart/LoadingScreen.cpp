//
// Created by ashish on 4/10/16.
//



#include "GameManager.hpp"
#include "LoadingScreen.hpp"
USING_NS_CC;


cocos2d::Scene *LoadingScreen::createScene(GameManager *manager) {

    auto scene = Scene::create();
    auto loadingScene = LoadingScreen::create(manager);
    scene->addChild(loadingScene);
    return scene;
}



LoadingScreen  * LoadingScreen::create(GameManager * manager){
    auto loadingLayer = new (std::nothrow) LoadingScreen();
    if (loadingLayer && loadingLayer->init(manager))
    {
        loadingLayer->autorelease();
        return loadingLayer;
    }
    CC_SAFE_DELETE(loadingLayer);
    return nullptr;
}


bool LoadingScreen::init(GameManager *manager) {
    if(!Layer::init()) {
        return false;
    }
    this->manager = manager;








//TODO add stuff to loader



//    loader.effects.push_back();
//    loader.backgroundeffects.push_back();
//    loader.spriteSheets.push_back();
//    loader.textures.push_back();














    //schedule data loader
    float freqeuncy = 1/20.0f;


    //schedule at frequency
    this->schedule(schedule_selector(LoadingScreen::loadData),freqeuncy);





    return true;

}





void LoadingScreen::loadData(float delta) {
    bool done = loader.step();
    if(done) {
        unschedule(schedule_selector(LoadingScreen::loadData));
        onDone();
    }
}


void LoadingScreen::onDone() {

    manager->switchToPlayScene();

}





bool LoadingScreen::Loader::step() {









    static bool once = true;

    if(once){
        once = false;
        remainingSpriteSheets = spriteSheets.size();
        remainingTextures = textures.size();
        remainingBackgroundEffects = backgroundeffects.size();
        remainingEffects = effects.size();

    }




    //please refer this http://www.learn-cocos2d.com/2012/11/optimize-memory-usage-bundle-size-cocos2d-app/
    //TODO load textures here

    //set texture format here before laoding texture here


    //TODO plist into spriteframe cache here

//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cjtexset_01.plist");
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("cjtexset_02.plist");

    //TODO preload sound effects here

//    auto audioEngine = SimpleAudioEngine::getInstance();
//    audioEngine->preloadBackgroundMusic();
//    audioEngine->preloadBackgroundMusic(fileUtils->fullPathForFilename("background.mp3").c_str());



    //set done
    done = (remainingSpriteSheets == 0)&&(remainingTextures)&&(remainingBackgroundEffects == 0)&&(remainingEffects ==0);

    return done;







}

