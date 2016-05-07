//
// Created by ashish on 5/5/16.
//

#ifndef MYGAME_RENDERMODULE_HPP
#define MYGAME_RENDERMODULE_HPP

#include "../../PlayModule.hpp"
class TestActor;
class RenderModule :public PlayModule{
public:
    CREATE_MODULE(RenderModule)

    virtual ~RenderModule();

    virtual void onEnter() override;
    bool init(const staticInfo &info, B2PhysicsSystem *system, MainCamera *cam, const b2Vec2 &offset) override;
    void prePhysicsUpdate(float delta) override;
    void postPhysicsUpdate(float delta) override;


    void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform,
                       uint32_t parentFlags) override;
protected:
    void onCoordsStable() override;




private:





    cocos2d::RenderTexture * canvas = nullptr;
    cocos2d::Sprite * renderSprite = nullptr;



    cocos2d::Sprite * testSprite = nullptr;

    TestActor *newActor = nullptr;

};



#endif //MYGAME_RENDERMODULE_HPP
