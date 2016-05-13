//
// Created by Priyanshu Singh on 10/05/16.
//

#include "Light.hpp"
#include "LightHandler.hpp"

using namespace box2dLight;

void Light::setColor(Color4F color) {

    if (staticLight && color != this->color) dirty = true;
    this->color = color;
}

float32 Light::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) {

    if(!globalContactFilter(fixture))return -1;
    if(!contactFilter(fixture))return -1;
    if(ignoreBody && fixture->GetBody() == getBody())return -1;

    //TODO --NOTE-- implement box2d to World coordinate
    raycastPoints[m_index] = Vec2(point.x, point.y);
    raycastFraction[m_index] = fraction;

    return fraction;
}

bool Light::init(LightHandler *lightHandler, int rays, Color4F color, float distance, float directionDegree) {

    setLightHandler(lightHandler);
    setRayNum(rays);
    setColor(color);
    setDistance(distance);
    setSoftnessLength(distance * 0.1f);
    setDirection(directionDegree);

    return true;
}

void Light::setColor(float r, float g, float b, float a) {

    setColor(Color4F(r,g,b,a));
}

void Light::setLightHandler(LightHandler *lightHandler) {

    if(this->lightHandler == nullptr){
        this->lightHandler = lightHandler;
        this->lightHandler->addChild(this);
    }else{
        removeLightHandler();
        this->lightHandler = lightHandler;
        if(lightHandler != nullptr)this->lightHandler->addChild(this);
    }

}

void Light::removeLightHandler() {
    if(this->lightHandler != nullptr){
        this->lightHandler->removeChild(this, true);
    }
}

bool Light::isXray() {
    return xray;
}

void Light::setXray(bool xray) {
    this->xray = xray;
}

bool Light::isStaticLight() {
    return staticLight;
}

void Light::setStaticLight(bool staticLight) {
    this->staticLight = staticLight;
    if(staticLight)dirty = true;
}

bool Light::isSoft() {
    return soft;
}

void Light::setSoft(bool soft) {
    this->soft = soft;
    if(staticLight)dirty = true;
}

float Light::getSoftShadowLength() {
    return softShadowLength;
}

void Light::setSoftnessLength(float softShadowLength) {
    this->softShadowLength = softShadowLength;
    if(staticLight) dirty = true;
}

Color4F Light::getColor() {
    return color;
}

float Light::getDistance() {
    return distance / LightHandler::gammaCorrectionParameter;
}


float Light::getDirection() {
    return direction;
}

bool Light::contains(float x, float y) {
    return false;
}

void Light::setIgnoreAttachedBody(bool flag) {
    ignoreBody = flag;
}

bool Light::isIgnoreAttachedBody() {
    return ignoreBody;
}

void Light::setRayNum(int rays) {
    if(rays < MIN_RAYS) rays = MIN_RAYS;

    rayNum = rays;
    vertexNum = rays + 1;

    if(raycastPoints != nullptr){
        CC_SAFE_DELETE(raycastPoints);
    }
    if(raycastFraction != nullptr){
        CC_SAFE_DELETE(raycastFraction);
    }
    raycastFraction = new float[vertexNum];
    raycastPoints = new Vec2[vertexNum];


}

int Light::getRayNum() {
    return rayNum;
}

void Light::setContactFilter(b2Filter filter) {
    filterA = filter;
}

void Light::setContactFilter(uint16 categoryBits, uint16 groupIndex, uint16 maskBits) {
    b2Filter filter;
    filter.categoryBits = categoryBits;
    filter.groupIndex = groupIndex;
    filter.maskBits = maskBits;
    setContactFilter(filter);
}

bool Light::contactFilter(b2Fixture *fixtureB) {
    const b2Filter& filterB = fixtureB->GetFilterData();

    if (filterA.groupIndex == filterB.groupIndex && filterA.groupIndex != 0)
    {
        return filterA.groupIndex > 0;
    }

    bool collide = (filterA.maskBits & filterB.categoryBits) != 0 && (filterA.categoryBits & filterB.maskBits) != 0;
    return collide;
}

bool Light::globalContactFilter(b2Fixture *fixtureB) {
    const b2Filter& filterB = fixtureB->GetFilterData();

    if (globalFilterA.groupIndex == filterB.groupIndex && globalFilterA.groupIndex != 0)
    {
        return globalFilterA.groupIndex > 0;
    }

    bool collide = (globalFilterA.maskBits & filterB.categoryBits) != 0 && (globalFilterA.categoryBits & filterB.maskBits) != 0;
    return collide;
}

void Light::setGlobalContactFilter(b2Filter filter) {
    globalFilterA = filter;

}

void Light::setGlobalContactFilter(uint16 categoryBits, uint16 groupIndex, uint16 maskBits) {
    b2Filter filter;
    filter.categoryBits = categoryBits;
    filter.groupIndex = groupIndex;
    filter.maskBits = maskBits;
    setGlobalContactFilter(filter);
}

Vec2 Light::getStartPosition() {
    return tmpPosition;
}

void Light::setBlendFunc(const BlendFunc func) {
    blendFunction = func;
}

void Light::setUniformsForCustomLightShader() {

}
