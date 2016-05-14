//
// Created by Priyanshu Singh on 11/05/16.
//

#include "PointLight.hpp"
#include "LightHandler.hpp"

//PointLight name collision with cocos2d::PointLight

void box2dLight::PointLight::update(float delta) {
    updateBody(delta);
    if(dirty) setEndPoints();

    if(cull()) return;
    if(staticLight && !dirty) return;

    dirty = false;
    updateMesh();
}

void box2dLight::PointLight::setDistance(float dist) {

    dist *= LightHandler::gammaCorrectionParameter;
    distance = dist < 0.01f ? 0.01f : dist;
    dirty = true;
}

void box2dLight::PointLight::setDirection(float directionDegree) {

}

void box2dLight::PointLight::setEndPoints() {
    sin.clear();
    cos.clear();
    end.clear();
    float angleNum = 360.0f / (rayNum - 1);
    for(int i = 0; i < rayNum; i++){
        float angle = angleNum * i;
        sin.push_back(std::sinf(CC_DEGREES_TO_RADIANS(angle)));
        cos.push_back(std::cosf(CC_DEGREES_TO_RADIANS(angle)));
        end.push_back(Vec2(distance * cos[i], distance * sin[i]));
    }
}

box2dLight::PointLight *box2dLight::PointLight::create(box2dLight::LightHandler *lightHandler, int rays, Color4F color, float distance, float directionDegree) {
    PointLight *ret = new (std::nothrow)PointLight();
    if(ret && ret->init(lightHandler, rays, color, distance, directionDegree)){
        ret->autorelease();
        return ret;
    }else{
        delete(ret);
        ret = nullptr;
        return nullptr;
    }
}
