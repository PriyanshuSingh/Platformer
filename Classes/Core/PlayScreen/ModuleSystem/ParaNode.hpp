//
// Created by ashish on 5/24/16.
//

#ifndef MYGAME_PARANODE_HPP
#define MYGAME_PARANODE_HPP

#include "cocos2d.h"
#include "PlayModule.hpp"
class MainCamera;
class ParaNode: public cocos2d::Node,public PlayModule::StableListener{

public:
    enum class Dir{
        X,Y
    };

public:
    static ParaNode* create(MainCamera * cam,const cocos2d::Size & size,const Dir & dir);
    bool init(MainCamera * cam,const cocos2d::Size & size,const Dir & dir);
    void onEnter() override;
    void update(float delta)override;
    void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform,uint32_t parentFlags) override;




    virtual void onStable() override;

private:
    cocos2d::Vec2 factor = cocos2d::Vec2::ONE;
    Dir dir = Dir::X;
    PlayModule * parentModule = nullptr;
    MainCamera * cam = nullptr;
    cocos2d::Vec2 camPrevPos;

};


#endif //MYGAME_PARANODE_HPP
