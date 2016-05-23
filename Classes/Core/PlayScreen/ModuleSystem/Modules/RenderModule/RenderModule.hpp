//
// Created by ashish on 5/5/16.
//

#ifndef MYGAME_RENDERMODULE_HPP
#define MYGAME_RENDERMODULE_HPP

#include "../../PlayModule.hpp"
#include "DynamicLight.hpp"

class RenderModule :public PlayModule{
public:
    CREATE_MODULE(RenderModule)

    virtual void onEnter() override;


    virtual ~RenderModule();

    bool init(const staticInfo &info,B2PhysicsSystem * system,MainCamera *cam, const b2Vec2 &offset) override;
    void preUpdate(float delta) override;
    void postUpdate(float delta) override;

/*
    void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform,
                       uint32_t parentFlags) override;
*/
protected:




private:
    void onCoordsStable() override;
    cocos2d::RenderTexture * canvas = nullptr;
    cocos2d::Sprite * renderSprite = nullptr;

    DynamicLight * lt;

    cocos2d::Sprite * testSprite = nullptr;

};



#endif //MYGAME_RENDERMODULE_HPP
