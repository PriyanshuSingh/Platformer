//
// Created by Priyanshu Singh on 06/05/16.
//

#ifndef PLATFORMER_DYNAMICLIGHT_HPP
#define PLATFORMER_DYNAMICLIGHT_HPP


#include "PhysicsActor.hpp"
#include "b2PhysicsSystem.hpp"

USING_NS_CC;

#define NUMRAYS 1000

class DynamicLight : public PhysicsActor, public b2RayCastCallback {

private:
    bool lightEnable;
    CustomCommand _customCommand;
    GLfloat _m[(NUMRAYS+1)*2];
    GLfloat _ss[NUMRAYS*2*2];
    Color4F _cr[(NUMRAYS)*2];
    Color4F _cmr[NUMRAYS+1];
    GLfloat _f[(NUMRAYS+1)];
    GLfloat _sf[(NUMRAYS)*2];
    Vec2 _end[NUMRAYS];
    b2Vec2 _lightPos;
    float _sin[NUMRAYS];
    float _cos[NUMRAYS];
    float _dist;
    int m_index;
    int s_index;

    float _shadowLen;
    RenderTexture *canvas;

public:


    virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;

    virtual cocos2d::Vec2 getDeltaMovement();

    virtual float32 ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) override;

    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;

    virtual ~DynamicLight() {}

    virtual void onEnter() override;

    static DynamicLight *create(B2PhysicsSystem * system,const b2Vec2 & initPosition);

    bool init(B2PhysicsSystem *system,const b2Vec2 & initPosition);

    virtual void postPhysicsUpdate(float delta) override;

    virtual void prePhysicsUpdate(float delta) override;

    void onDraw(const Mat4 &transform, uint32_t flags);

    Sprite *getRenderSprite(){
        return canvas->getSprite();
    }


};


#endif //PLATFORMER_DYNAMICLIGHT_HPP
