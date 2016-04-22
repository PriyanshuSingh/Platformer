//
// Created by ashish on 4/15/16.
//

#include "PlayerCommand.hpp"
#include "Player.hpp"
#include "../b2PhysicsSystem.hpp"
#include "../PlatformerGlobals.hpp"



USING_NS_CC;

float  MoveCommand::total = 0;
std::vector<MoveCommand*> MoveCommand::contributors;

/*
 * PlayerCommand
 */


bool PlayerCommand::init(Player *target) {

    this->target = target;

    return true;
}






PlayerCommand::~PlayerCommand() {




}







/*
 * JumpCommand
 */

JumpCommand *JumpCommand::create(Player *target) {
    auto jC = new(std::nothrow)JumpCommand();
    if (jC && jC->init(target)) {
        jC->autorelease();
        return jC;
    }
    CC_SAFE_DELETE(jC);
    return nullptr;
}

bool JumpCommand::init(Player *target) {

    return PlayerCommand::init(target);


}

void JumpCommand::execute() {

    //check if possible to execute command
    if(target->dead || !target->grounded)
        return;


    target->grounded = false;
//    cocos2d::log("applied impulse");
    auto body = target->mainBody;
    //TODO get world gravity
    //and apply impulse in the reverse direction of it
    //This Allows for jump to work even when gravity reversed.
    b2Vec2 impulse(0,10);
    impulse*=body->GetMass();

    body->ApplyLinearImpulse(impulse,body->GetWorldCenter(),true);







}


/*
 * MoveCommand
 */


MoveCommand *MoveCommand::create(Player *target, float percent) {
    auto mC = new(std::nothrow)MoveCommand();
    if (mC && mC->init(target, percent)) {
        mC->autorelease();
        return mC;
    }
    CC_SAFE_DELETE(mC);
    return nullptr;

}

bool MoveCommand::init(Player *target, float percent) {
    PlayerCommand::init(target);
    this->myContribution = percent;
    right = true;
    MoveCommand::contributors.push_back(this);
    this->setContribution(percent);

    return true;
}


void MoveCommand::setContribution(float c) {

    MoveCommand::total+=c;
    myContribution = c;
    if(MoveCommand::total <=1)return;
    else{
       if(MoveCommand::contributors.size()<=1)return;
       auto diff = (MoveCommand::total-1)/(MoveCommand::contributors.size()-1);
       for(auto & p :MoveCommand::contributors){
           p->myContribution  = std::max(0.0f,p->myContribution-diff);
       }
       myContribution = c;


    }

}


void MoveCommand::execute() {

    if(target->dead || !target->grounded)
        return;



    auto body = target->mainBody;
    auto vel = Vec2(body->GetLinearVelocity().x,body->GetLinearVelocity().y);

    //if velocity 0 apply horizontal impulse to get to minSpeed
    if(vel.equals(Vec2::ZERO)){

        body->ApplyLinearImpulse((right?1:-1)*body->GetMass()*myContribution*(b2Vec2(minSpeed,0)),body->GetWorldCenter(),true);
        return;
    }



    float n = vel.lengthSquared();
    n = sqrt(n);
    float currentSpeed = n;
    // Too close to zero.
    if (n >= MATH_TOLERANCE) {
        n = 1.0f / n;
        vel.x *= n;
        vel.y *= n;
    }



    float delta = std::min(std::abs(maxSpeed-currentSpeed),increment);

    //if you exceed speed or move in other direction
    if(currentSpeed > maxSpeed ||right != (vel.x>=0) ){
        delta*=-1;
    }


    vel*=delta;

    body->ApplyLinearImpulse(body->GetMass()*myContribution*b2Vec2(vel.x,vel.y),body->GetWorldCenter(),true);




}


void MoveCommand::setDirection(bool right) {

    this->right = right;
}




MoveCommand::~MoveCommand() {

    auto it = std::find(MoveCommand::contributors.begin(), MoveCommand::contributors.end(),this);

    using std::swap;

    // swap the one to be removed with the last element
    swap(*it, MoveCommand::contributors.back());
    MoveCommand::contributors.pop_back();
    if(MoveCommand::contributors.empty())
        return;
    MoveCommand::total-=myContribution;
    float dis = myContribution/MoveCommand::contributors.size();
    for(auto & p:MoveCommand::contributors){
        p->myContribution+=dis;
    }


}



KillCommand *KillCommand::create(Player *target) {
    auto kC = new(std::nothrow)KillCommand();
    if (kC && kC->init(target)) {
        kC->autorelease();
        return kC;
    }
    CC_SAFE_DELETE(kC);
    return nullptr;

}

bool KillCommand::init(Player *target) {
    return PlayerCommand::init(target);

}

void KillCommand::execute() {


    if(target->dead)
        return;
    target->killMe();





}


AliveCommand * AliveCommand::create(Player *target,const b2Vec2 &pos, float degrees) {
    auto aC = new(std::nothrow)AliveCommand();
    if (aC && aC->init(target,pos,degrees)) {
        aC->autorelease();
        return aC;
    }
    CC_SAFE_DELETE(aC);
    return nullptr;

}

bool AliveCommand::init(Player *target,const b2Vec2 &pos, float degrees) {

    PlayerCommand::init(target);
    this->pos = pos;
    this->degrees = degrees;
    return true;
}

void AliveCommand::execute() {

    if(!target->dead)
        return;

    target->bringToLife(pos,degrees);
}


