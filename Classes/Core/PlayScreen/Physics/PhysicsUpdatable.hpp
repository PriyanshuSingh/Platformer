//
// Created by ashish on 5/21/16.
//

#ifndef MYGAME_PHYSICSUPDATABLE_HPP
#define MYGAME_PHYSICSUPDATABLE_HPP


class B2PhysicsSystem;
class PhysicsUpdatable {
public:
    PhysicsUpdatable();
    virtual void prePhysicsUpdate(float delta)=0;
    virtual void postPhysicsUpdate(float delta)=0;
    virtual ~PhysicsUpdatable();

protected:
    B2PhysicsSystem * system = nullptr;


};


#endif //MYGAME_PHYSICSUPDATABLE_HPP
