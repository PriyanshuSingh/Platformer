//
// Created by ashish on 4/23/16.
//

#ifndef MYGAME_STATICACTOR_HPP
#define MYGAME_STATICACTOR_HPP

#include "PhysicsActor.hpp"

class StaticActor:public PhysicsActor {
public:

    //override not allowed!!! therefore deleted!
    bool init(B2PhysicsSystem * system,PlayModule * parent,const b2Vec2 & offset);
    virtual ~StaticActor();
    //return movement in Cocos Coordinates based on your physics Stuff
    //override not allowed!!! therefore deleted!
    cocos2d::Vec2 getDeltaMovement(){return cocos2d::Vec2::ZERO;}

protected:
    std::function<void(StaticActor *)> initCallback;
    std::function<void(StaticActor *)> cleanUpCallback;



private:

};

#endif //MYGAME_STATICACTOR_HPP
