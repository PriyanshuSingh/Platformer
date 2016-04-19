//
// Created by ashish on 4/15/16.
//

#ifndef MYGAME_PLAYERCOMMAND_HPP
#define MYGAME_PLAYERCOMMAND_HPP


#include "Box2D/Box2D.h"
#include "cocos2d.h"

//
//bool isEqual(const pair<string, int> element)
//{
//    return element.first ==  User.name;
//}
//it = find_if( sortList.begin(), sortList.end(), isEqual );





//TODO see if the implementation of this
//can be changed,because this replies on exponential behaviour

class Player;
class PlayerCommand;






class PlayerCommand{
public:
    virtual bool init(Player * target);
    //main command here
    virtual void execute()=0;
    virtual ~PlayerCommand();

    //TODO add callback method here




protected:
    Player * target = nullptr;
private:

};

//relying on cocos2d-x memory management
class JumpCommand:public PlayerCommand,public cocos2d::Ref{


public:
    static JumpCommand * create(Player * target);
    bool init(Player * target);
    void execute()override;


    virtual ~JumpCommand(){}
private:
;
};


class MoveCommand:public PlayerCommand,public cocos2d::Ref{


public:
    static MoveCommand * create(Player * target,float percent);
    bool init(Player * target,float percent);
    void execute()override;
    void setDirection(bool right);

    float getContribution(){ return myContribution;}
    void setContribution(float c);

    virtual ~MoveCommand();

private:
    bool right = false;
    float minSpeed = 0.27f;
    float maxSpeed = 2.22f;


    float myContribution = 0.0f;

    static std::vector<MoveCommand*> contributors;
    static float total;

};


class KillCommand:public PlayerCommand,public cocos2d::Ref{


public:
    static KillCommand * create(Player * target);
    bool init(Player * target);
    void execute()override;
    virtual ~KillCommand(){}

private:

};




class AliveCommand:public PlayerCommand,public cocos2d::Ref{


public:
    static AliveCommand * create(Player * target,const b2Vec2 &pos,float degrees);
    bool init(Player * target,const b2Vec2 &pos,float degrees);
    void execute()override;
    virtual ~AliveCommand(){}

private:

    b2Vec2 pos;
    float degrees;
};

#endif //MYGAME_PLAYERCOMMAND_HPP
