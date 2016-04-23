//
// Created by ashish on 3/27/16.
//

#include "b2PhysicsSystem.hpp"




USING_NS_CC;

bool B2PhysicsSystem::made = false;

bool B2PhysicsSystem::isSystemActive() {
    return B2PhysicsSystem::made;
}




B2PhysicsSystem::B2PhysicsSystem(float32 ptmRatio,const b2Vec2 & gravity,bool interpolate):ptmRatio(ptmRatio),interpolate(interpolate),paused(false),worldSettings() {






    CCASSERT(!B2PhysicsSystem::made,"One instance of Physics World Exists");
    B2PhysicsSystem::made = true;
    phyWorld = new b2World(gravity);


}

B2PhysicsSystem::~B2PhysicsSystem() {

    delete phyWorld;
    phyWorld = nullptr;
    B2PhysicsSystem::made = false;


}


void B2PhysicsSystem::interpolation(bool enable) {

    interpolate = enable;
}


b2World *B2PhysicsSystem::getWorld() {
    return phyWorld;
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


b2Vec2 B2PhysicsSystem::getB2VisibleOrigin() {
    return screenToBox2D(Director::getInstance()->getVisibleOrigin());
}


void B2PhysicsSystem::update(float deltaTime) {




    //temporary for testing only
    phyWorld->Step(deltaTime,worldSettings.velocityIterations,worldSettings.positionIterations);



    //TODO take care if simulation paused
    //refer the testbed pause for example
//    if(paused){
//
//    }





//TODO avoid spiral of death as well



    //TODO refer these links for implementation
    //http://www.unagames.com/blog/daniele/2010/06/fixed-time-step-implementation-box2d
    //http://saltares.com/blog/games/fixing-your-timestep-in-libgdx-and-box2d/
    //http://plaincode.blogspot.in/2012/05/fixed-timestep-in-cocos2d-x-with-box2d.html

    //TODO interpolation in last two time steps of world only






//TODO add this to delete bodies after time step stuff done
// refer http://gamedev.stackexchange.com/questions/16943/how-to-remove-a-box2d-body-when-collision-happens
//    for (Entity entity : manager.entitiesToRemove)
//    {
//        world.destroyBody(entity.body); //this might be all you need -- adapt to your own purposes. but you will still need a list such that you remove only at the end of each tick.
//    }
//
//    manager.entitiesToRemove.clear();

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



