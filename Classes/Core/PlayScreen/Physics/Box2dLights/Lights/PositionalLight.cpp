//
// Created by Priyanshu Singh on 10/05/16.
//

#include "PositionalLight.hpp"
#include "LightHandler.hpp"
#include "LightMap.hpp"

using namespace box2dLight;

bool PositionalLight::init(LightHandler *lightHandler, int rays, Color4F color, float distance, float directionDegree) {

    if(!Light::init(lightHandler, rays, color, distance, directionDegree))return false;

}

void PositionalLight::update(float delta) {

    updateBody(delta);

    if(cull()) return;
    if(staticLight && !dirty)return;

    dirty = false;
    updateMesh();
}

void PositionalLight::updateBody(float d) {

    if(body != nullptr || staticLight)return;

    //TODO Interpolation not taken into account
    //TODO --NOTE-- implement box2d to World coordinate
    Vec2 vec2 = Vec2(body->GetPosition().x, body->GetPosition().y);

    float angle = body->GetAngle();
    float cos = std::cosf(angle);
    float sin = std::sinf(angle);
    Vec2 change = Vec2(bodyOffset.x * cos - bodyOffset.y * sin,bodyOffset.x * sin + bodyOffset.y * cos);
    start = vec2 + change;
    setDirection(bodyAngleOffset + CC_RADIANS_TO_DEGREES(angle));

}

bool PositionalLight::cull() {
    culled = lightHandler->culling && !lightHandler->intersect(
            start.x, start.y, distance + softShadowLength);
    return culled;
}

void PositionalLight::updateMesh() {
    for(int i=0; i < rayNum; i++){
        m_index = i;
        raycastFraction[i] = 1.0f;
        tmpEnd = end[i] + start;
        raycastPoints[i] = tmpEnd;
        if(lightHandler->world != nullptr && !xray){
            lightHandler->world->RayCast(this, b2Vec2(start.x,start.y), b2Vec2(tmpEnd.x, tmpEnd.y));
        }
    }
    setMesh();
}

void PositionalLight::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {

    _customCommand.init(_globalZOrder, transform, flags);
    _customCommand.func = CC_CALLBACK_0(PositionalLight::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);

}

void PositionalLight::onDraw(const Mat4 &transform, uint32_t flags) {

    if(lightHandler->culling && culled)
        return;

    lightHandler->lightRenderedLastFrame++;
    glEnable(GL_BLEND);
    glBlendFunc(blendFunction.src, blendFunction.dst);

    GLProgramState *glProgramState = getGLProgramState();

    glProgramState->setVertexAttribPointer(GLProgram::ATTRIBUTE_NAME_POSITION,  2, GL_FLOAT, GL_FALSE, 0, &lightMeshPoints[0]);
    glProgramState->setVertexAttribPointer(GLProgram::ATTRIBUTE_NAME_COLOR, 4, GL_FLOAT, GL_FALSE, 0, &lightColorAttrib[0]);
    glProgramState->setVertexAttribPointer(LightMap::ATTRIBUTE_NAME_FRACTION, 1,  GL_FLOAT, GL_FALSE, 0, &lightFraction[0]);

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertexNum);

    if(soft && !xray) {
        glProgramState->setVertexAttribPointer(GLProgram::ATTRIBUTE_NAME_POSITION, 2, GL_FLOAT, GL_FALSE, 0, &softShadowMeshPoints[0]);
        glProgramState->setVertexAttribPointer(GLProgram::ATTRIBUTE_NAME_COLOR, 4, GL_FLOAT, GL_FALSE, 0, &softShadowColorAttrib[0]);
        glProgramState->setVertexAttribPointer(LightMap::ATTRIBUTE_NAME_FRACTION, 1, GL_FLOAT, GL_FALSE, 0, &softShadowFraction[0]);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, (vertexNum - 1)*2);
    }
    glDisable(GL_BLEND);
}

void PositionalLight::setMesh() {

    lightMeshPoints.clear();
    lightFraction.clear();
    lightColorAttrib.clear();

    lightMeshPoints.push_back(GVec2(start));
    lightColorAttrib.push_back(color);
    lightFraction.push_back(1.0f);

    for(int i=0; i < rayNum; i++){
        lightMeshPoints.push_back(GVec2(raycastPoints[i]));
        lightColorAttrib.push_back(color);
        lightFraction.push_back(1.0f - raycastFraction[i]);
    }

    if(!soft || xray) return;

    softShadowMeshPoints.clear();
    softShadowColorAttrib.clear();
    softShadowFraction.clear();

    for(int i=0; i<rayNum; i++){
        softShadowMeshPoints.push_back(GVec2(raycastPoints[i]));
        softShadowColorAttrib.push_back(color);
        softShadowFraction.push_back(1.0f - raycastFraction[i]);

        softShadowMeshPoints.push_back(GVec2(raycastPoints[i]));
        softShadowColorAttrib.push_back(ZeroColor);
        softShadowFraction.push_back(0.0f);
    }

}

void PositionalLight::setRayNum(int rays) {
    Light::setRayNum(rays);

    sin.reserve(rays);
    cos.reserve(rays);
    end.reserve(rays);

    lightMeshPoints.reserve(vertexNum);
    lightFraction.reserve(vertexNum);
    lightColorAttrib.reserve(vertexNum);

    softShadowMeshPoints.reserve((vertexNum - 1)*2);
    softShadowColorAttrib.reserve((vertexNum - 1)*2);
    softShadowFraction.reserve((vertexNum - 1)*2);

}

void PositionalLight::attachToBody(b2Body *body) {
    attachToBody(body, 0.0f, 0.0f, 0.0f);
}

void PositionalLight::attachToBody(b2Body *pBody, float bodyOffsetX, float bodyOffsetY, float bodyAngleOffset) {
    this->body;
    bodyOffset = Vec2(bodyOffsetX, bodyOffsetY);
    this->bodyAngleOffset = bodyAngleOffset;
    if(staticLight) dirty = true;
}

void PositionalLight::setStartPosition(float x, float y) {
    start.x = x;
    start.y = y;
    if(staticLight)dirty = true;
}

void PositionalLight::setStartPosition(Vec2 pos) {
    setStartPosition(pos.x, pos.y);
}

b2Body *PositionalLight::getBody() {
    return body;
}

bool PositionalLight::contains(float x, float y) {

    float x_d = start.x - x;
    float y_d = start.y - y;
    float dist = x_d * x_d + y_d * y_d;
    if(distance * distance <= dist) return false;

    bool oddNodes = false;
    Vec2 point2 = raycastPoints[rayNum] = start;
    Vec2 point1;
    for(int i=0; i <= rayNum; point2 = point1, ++i){
        point1 = raycastPoints[i];
        if(((point1.y < y) && (point2.y >= y)) || (point1.y >= y) && (point2.y < y)){
            if((y - point1.y) / (point2.y - point1.y) * (point2.x - point1.x) < (x - point1.x)) oddNodes = !oddNodes;
        }
    }
    return oddNodes;
}

Vec2 PositionalLight::getStartPosition() {
    tmpPosition = start;
    return tmpPosition;
}
