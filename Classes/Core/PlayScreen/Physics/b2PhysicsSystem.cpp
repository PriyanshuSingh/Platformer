//
// Created by ashish on 3/27/16.
//

#include "b2PhysicsSystem.hpp"
#include "../PlatformerGlobals.hpp"
#include "Impulser.hpp"
#include "PhysicsUpdatable.hpp"



USING_NS_CC;

bool B2PhysicsSystem::made = false;
B2PhysicsSystem * B2PhysicsSystem::currentInstance = nullptr;

b2dJson B2PhysicsSystem::addJsonObject(const std::string &filename) {
    //TODO replace this with preloaded strings to avoid loading data at runtime
    std::string path = FileUtils::getInstance()->fullPathForFilename(filename.c_str());
    b2dJson json;
    std::string errMsg;
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(path);
#ifdef DEBUGGING_APP
    cocos2d::log("Full path is: %s", path.c_str());
#endif

    json.readFromString(jsonContent, errMsg,this->getWorld());

#ifdef DEBUGGING_APP

    if(errMsg.empty()){
        cocos2d::log("no error occured json loaded ok");
    }
    else{
        cocos2d::log("Error occured : %s",errMsg.c_str());
    }
#endif
    return json;

}


bool B2PhysicsSystem::isSystemActive() {
    return B2PhysicsSystem::made;
}




B2PhysicsSystem::B2PhysicsSystem(float32 ptmRatio,const b2Vec2 & gravity):ptmRatio(ptmRatio),paused(false),updated(false),worldSettings() {






    CCASSERT(!B2PhysicsSystem::made,"One instance of Physics World Exists");
    B2PhysicsSystem::made = true;
    currentInstance = this;
    phyWorld = new b2World(gravity);


#ifdef FIXED_TIMESTEP
    phyWorld->SetAutoClearForces(false);
#endif

}

B2PhysicsSystem::~B2PhysicsSystem() {

    delete phyWorld;
    phyWorld = nullptr;
    B2PhysicsSystem::made = false;
    currentInstance = nullptr;


}


void B2PhysicsSystem::setUpdated(bool update) {
    updated = update;
}



b2World *B2PhysicsSystem::getWorld() {
    return phyWorld;
}


float32 B2PhysicsSystem::getInterpolationFactor() const {
#ifdef FIXED_TIMESTEP
    return interpolationFactor;
#else
    return 1.0f;
#endif

}


void B2PhysicsSystem::addOffset(b2Body *body,const b2Vec2 & offset) {

    body->SetTransform(body->GetPosition()+offset,body->GetAngle());
}

void B2PhysicsSystem::addOffset(b2Joint *joint,const b2Vec2 & offset) {

    joint->ShiftOrigin(-offset);
}


float32 B2PhysicsSystem::screenToBox2D(float32 in) {
    return in/ptmRatio;
}


float32 B2PhysicsSystem::box2DToScreen(float32 in) {
    return in*ptmRatio;

}

cocos2d::Vec2 B2PhysicsSystem::box2DToScreen(const b2Vec2 &in) {
    return cocos2d::Vec2(box2DToScreen(in.x),box2DToScreen(in.y));

}

b2Vec2 B2PhysicsSystem::screenToBox2D(const cocos2d::Vec2 &in) {
    return b2Vec2(screenToBox2D(in.x),screenToBox2D(in.y));
}


void B2PhysicsSystem::DestroyJoints(const std::vector<b2Joint *> &joints) {

    if(B2PhysicsSystem::made) {
        for (auto &j :joints) {
            phyWorld->DestroyJoint(j);

        }
    }
}

void B2PhysicsSystem::DestroyBodies(const std::vector<b2Body *> &bodies) {

    if(B2PhysicsSystem::made) {
        for (auto &b :bodies) {
            phyWorld->DestroyBody(b);

        }
    }
}


b2Vec2 B2PhysicsSystem::getB2VisibleOrigin() {
    return screenToBox2D(Director::getInstance()->getVisibleOrigin());
}


void B2PhysicsSystem::update(float deltaTime) {


//temporary for testing only


#ifndef FIXED_TIMESTEP

    phyWorld->Step(deltaTime,worldSettings.velocityIterations,worldSettings.positionIterations);

#else










    //TODO refer these links for implementation
    //http://www.unagames.com/blog/daniele/2010/06/fixed-time-step-implementation-box2d
    //http://saltares.com/blog/games/fixing-your-timestep-in-libgdx-and-box2d/
    //http://plaincode.blogspot.in/2012/05/fixed-timestep-in-cocos2d-x-with-box2d.html


    accumulator+=deltaTime;


//TODO  TOGETHER take care if simulation paused


    if(paused){
        accumulator = 0;
    }






    const int MAX_STEPS = worldSettings.maxSteps;


    const int nSteps = static_cast<int>(std::floor(accumulator / worldSettings.timeStep));


    if (nSteps > 0)
    {
        accumulator -= nSteps * worldSettings.timeStep;
    }


    CCASSERT(accumulator < worldSettings.timeStep + FLT_EPSILON,"Accumulator must have a value lesser than the fixed time step");
    const float32 accumulatorRatio = accumulator / worldSettings.timeStep;




    //avoid spiral of death
    const int nStepsClamped = std::min(nSteps, MAX_STEPS);
    bool simulated = false;
    if(nStepsClamped>0){
        for(auto & updatable:updatables){
            updatable->prePhysicsUpdate(deltaTime);
        }

    }

    for (int i = 0; i < nStepsClamped; ++i)
    {
        if(i == nStepsClamped-1){
            auto list = phyWorld->GetBodyList();
            for(auto bod = list;bod;bod = bod->GetNext()){
                bod->SetPrevPositionAndAngle(bod->GetPosition(),bod->GetAngle());
            }
        }
        for(auto & impulser: impulsers){
            impulser->updateImpulse();
        }
        phyWorld->Step(worldSettings.timeStep, worldSettings.velocityIterations, worldSettings.positionIterations);
        // In singleStep_() the CollisionManager could fire custom
        // callbacks that uses the smoothed states. So we must be sure
        // to reset them correctly before firing the callbacks.
//        resetSmoothStates_ ();
//        singleStep_ (FIXED_TIMESTEP);

        simulated = true;

    }
    //fixed updates in Box2D World

    if(simulated) {
        phyWorld->ClearForces();

        for(auto & updatable:updatables){
            updatable->postPhysicsUpdate(deltaTime);
        }

    }


    interpolationFactor =  accumulatorRatio;
//#ifdef  DEBUGGING_APP
//    int execCount  = 0;
//#endif
//    while(accumulator >= worldSettings.timeStep){
//
//        phyWorld->Step(worldSettings.timeStep, worldSettings.velocityIterations, worldSettings.positionIterations);
//        accumulator -= worldSettings.timeStep;
//        simulated = true;
//
//#ifdef  DEBUGGING_APP
//        execCount++;
//#endif
//
//    }
//    if(simulated)
//        phyWorld->ClearForces();
//
//#ifdef  DEBUGGING_APP
//    cocos2d::log("exec count is %d",execCount);
//#endif



    //TODO may be add interpolation??
    //TODO interpolation in last two time steps of world only












//TODO add this to delete bodies after time step stuff done
// refer http://gamedev.stackexchange.com/questions/16943/how-to-remove-a-box2d-body-when-collision-happens
//    for (Entity entity : manager.entitiesToRemove)
//    {
//        world.destroyBody(entity.body); //this might be all you need -- adapt to your own purposes. but you will still need a list such that you remove only at the end of each tick.
//    }
//
//    manager.entitiesToRemove.clear();


#endif
}

void B2PhysicsSystem::pause() {

    paused = true;

}

void B2PhysicsSystem::resume() {

    paused = false;

}


void B2PhysicsSystem::setSimulationSpeed(float scaleFactor) {


    //TODO see what min value of scale factor you can have
    //and limit it here
    this->scaleFactor = scaleFactor;
}


void B2PhysicsSystem::addImpulser(Impulser *i) {

    if(isSystemActive()) {

        CCASSERT(std::find(impulsers.begin(), impulsers.end(), i) == impulsers.end(),
                 "Re Adding a already added impulser");
        impulsers.push_back(i);
    }

}

void B2PhysicsSystem::removeImpulser(Impulser *i) {

    if(isSystemActive()) {
        auto it = std::find(impulsers.begin(), impulsers.end(), i);

        CCASSERT(it != impulsers.end(), "Deleting a not added impulser");


        using std::swap;
        // swap the one to be removed with the last element
        swap(*it, impulsers.back());
        impulsers.pop_back();

    }
}



void B2PhysicsSystem::addPhysicsUpdatable(PhysicsUpdatable *pUpdatable) {

    if(B2PhysicsSystem::made) {
        CCASSERT(std::find(updatables.begin(), updatables.end(), pUpdatable) == updatables.end(), "Re Adding a already added actor");
        updatables.push_back(pUpdatable);

    }
}

void B2PhysicsSystem::removePhysicsUpdatable(PhysicsUpdatable *pUpdatable ) {
    if(B2PhysicsSystem::made) {
        auto it = std::find(updatables.begin(), updatables.end(), pUpdatable);

        CCASSERT(it != updatables.end(), "Deleting a not added actor");
            using std::swap;
    //     swap the one to be removed with the last element
        swap(*it, updatables.back());
        updatables.pop_back();


    }


}


