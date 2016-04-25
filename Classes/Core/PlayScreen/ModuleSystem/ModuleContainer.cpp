//
// Created by ashish on 4/11/16.
//

#include "ModuleContainer.hpp"
#include "ModuleEntries.hpp"
#include "../Physics/b2PhysicsSystem.hpp"
#include "../MainCamera.hpp"
#include "../Player/Player.hpp"
#include "../../PlayScreen/Player/PlayerController.hpp"

USING_NS_CC;


ModuleContainer * ModuleContainer::create(B2PhysicsSystem *system,MainCamera * cam) {

    auto module = new (std::nothrow) ModuleContainer();
    if (module && module->init(system,cam))
    {
        module->autorelease();
        return module;
    }
    CC_SAFE_DELETE(module);
    return nullptr;
}
bool ModuleContainer::init(B2PhysicsSystem *system,MainCamera * cam) {
    if(!Node::init()){
        return false;
    }

    //self properties
    {
        //TODO see this position
        setPosition(Vec2::ZERO);
        setAnchorPoint(Vec2::ZERO);
        setContentSize(Director::getInstance()->getWinSize());


        schedule(schedule_selector(ModuleContainer::checkIsCurrentSafelyDone),checkInterval);

        this->system = system;
        this->cam = cam;
        this->mainPlayer = Player::create(this,system,b2Vec2(3,10));
        this->mainPlayer->retain();
    }









    //TODO use save info here to know which module to start game from
    currentLevel = 0;

    loadCurrentModule();
    loadNextModule();




    //input controller test
    {
        gameController = InputController::create(mainPlayer);
        controller2 = TestAiController::create(mainPlayer);

        this->addChild(gameController,10);
        this->addChild(controller2,10);





    }








    return true;
}


ModuleContainer::~ModuleContainer() {


    CC_SAFE_RELEASE_NULL(mainPlayer);


}


void ModuleContainer::move(float xDisplacement,float zoomFactor) {
#ifdef DEBUGGING_APP
    cocos2d::log("x displacement is %f",xDisplacement);
#endif





//    this->setPosition(getPosition()+Vec2(xDisplacement,0));
//
//
//    auto winSize  = Director::getInstance()->getWinSize();
//    Vec2 mid(winSize/2);
//
//
//
//    mid = convertToNodeSpace(mid)/winSize.width;
//    this->setAnchorPoint(Vec2(mid.x,_anchorPoint.y));
//    cocos2d::log("anchor pos is %f",_anchorPoint.x);
//    this->setScale(zoomFactor);







}
//public interface
void ModuleContainer::onEnterLastModule() {



    cocos2d::log("entered last module");
}
void ModuleContainer::addSwitchCurrentCallback(const std::function<void()> &callback) {

    switchCurrentCallbacks.push_back(callback);

}
//private methods
void ModuleContainer::switchCurrentModule() {




    currentLevel++;
    if(currentLevel < ModuleEntries::getTotal()){




        cocos2d::log("loading next module and swapping");


        PlayModule* prev = current;
        current = next;





        //shift Box2D bodies
        auto getOffset  = b2Vec2(system->screenToBox2D(prev->getContentSize()).x,0);
        system->getWorld()->ShiftOrigin(getOffset);


        //Box2D world callbacks
        //like b2ContactListener,b2ContactFilter,b2DestructionListener
        system->getWorld()->SetContactListener(current);
        system->getWorld()->SetDestructionListener(current);


        //move camera back
        cam->setPositionX(current->convertToNodeSpace(cam->getPosition()).x);


        //move yourself back
        current->setPosition(Vec2::ZERO);
        current->setCoordinatesStabilized(true);


        //pass on player
        prev->removePlayer();
        current->addPlayer(mainPlayer);

        //delete prev guy
        removeChild(prev);
        loadNextModule();
        cocos2d::log("removed prev");

        if(next == nullptr){

            onEnterLastModule();


        }


    }
    else{
        cocos2d::log("can't enter danger");
        //unschedule the checker
        unschedule(schedule_selector(ModuleContainer::checkIsCurrentSafelyDone));




        //save the fact user completed game

        //switch the scene back to start screen
//        Director::getInstance()->replaceScene()
        //TODO end game here
        //application control breaker
        //roll credits


    }
}





void ModuleContainer::loadCurrentModule() {

    current = ModuleEntries::getNextModule(currentLevel,system,cam,b2Vec2(0.0f,0.0f));
    current->setPosition(Vec2::ZERO);
    current->setCoordinatesStabilized(true);
    //add player
    current->addPlayer(mainPlayer);

    //register contact listeners
    system->getWorld()->SetContactListener(current);
    system->getWorld()->SetDestructionListener(current);

    this->addChild(current,DRAWORDER::MIDDLEGROUND);






}

void ModuleContainer::loadNextModule() {


    Vec2 offset(current->getContentSize().width,0);




    if(currentLevel== ModuleEntries::getTotal()-1){
        next = nullptr;
        return;
    }
    next = ModuleEntries::getNextModule(currentLevel+1,system,cam,system->screenToBox2D(offset));
    next->setPosition(offset);
    this->addChild(next,DRAWORDER::MIDDLEGROUND);



}

void ModuleContainer::checkIsCurrentSafelyDone(float delta) {

    //see if the right most corner is past screen

    auto right = convertToWorldSpace(Vec2(current->getContentSize().width,0));
    auto hold = cam->projectGL(Vec3(right.x,right.y,0));





    if(hold.x+current->getOffset()<=0){
        switchCurrentModule();
        for(auto f:switchCurrentCallbacks){f();}


    }


}

PlayModule *ModuleContainer::getCurrentModule() {

    CCASSERT(current,"FATAL ERROR Current is level is NULL");
    return current;
}

void ModuleContainer::prePhysicsUpdate(float delta) {



    CCASSERT(getNodeToWorldTransform().isIdentity(),"Don't tranform Container");




    //TODO move these to the current prePhysicsUpdate for
    //more fine grained control
    gameController->prePhysicsUpdate(delta);
    mainPlayer->prePhysicsUpdate(delta);
    current->prePhysicsUpdate(delta);

}


void ModuleContainer::postPhysicsUpdate(float delta){



    gameController->postPhysicsUpdate(delta);
    mainPlayer->postPhysicsUpdate(delta);
    current->postPhysicsUpdate(delta);





}







