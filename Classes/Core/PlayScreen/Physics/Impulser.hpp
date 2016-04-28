//
// Created by ashish on 4/28/16.
//

#ifndef MYGAME_IMPULSER_HPP
#define MYGAME_IMPULSER_HPP

class B2PhysicsSystem;

class Impulser{

public:
    Impulser();
    ~Impulser();
    virtual void updateImpulse()=0;

private:
    B2PhysicsSystem * system = nullptr;
};


#endif //MYGAME_IMPULSER_HPP
