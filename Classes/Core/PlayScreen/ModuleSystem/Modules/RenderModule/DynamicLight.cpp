//
// Created by Priyanshu Singh on 06/05/16.
//

#include "DynamicLight.hpp"

void DynamicLight::prePhysicsUpdate(float delta) {
    PhysicsActor::prePhysicsUpdate(delta);
    Color4F color = Color4F(1.0f,1.0f,1.0f, 1.0f);
    //CCLOG("prephysics for dynamic light");
    _m[0] = system->box2DToScreen(_lightPos.x);
    _m[1] = system->box2DToScreen(_lightPos.y);
    _f[0] = 1.0f;
    _cmr[0] = color;
    for(int i=1; i< NUMRAYS+1; i++){
        m_index = i*2;
        s_index = (i-1)*4;
        b2Vec2 tempEnd;
        tempEnd.x = _end[i-1].x + _lightPos.x;
        tempEnd.y = _end[i-1].y + _lightPos.y;
        _f[m_index/2] = _sf[s_index/2] = _sf[s_index/2 + 1] = 0.0f;

        _m[m_index] = _ss[s_index] = system->box2DToScreen(tempEnd.x);
        _m[m_index+1] = _ss[s_index+1] = system->box2DToScreen(tempEnd.y);
        _ss[s_index+2] = _m[m_index] + _sf[s_index/2] * _shadowLen * _cos[s_index/4];
        _ss[s_index+3]  = _m[m_index+1] + _sf[s_index/2] * _shadowLen * _sin[s_index/4];
        _cr[s_index/2] = color;
        _cmr[m_index/2] = color;
        _cr[s_index/2+1] = Color4F(0.0f,0.0f,0.0f,0.0f);
        system->getWorld()->RayCast(this, _lightPos, tempEnd);
    }

}


void DynamicLight::postPhysicsUpdate(float delta) {
    PhysicsActor::postPhysicsUpdate(delta);
}

bool DynamicLight::init(B2PhysicsSystem *system, const b2Vec2 &initPosition) {
    bVec bods;
    jVec jos;
    if(!PhysicsActor::init(system,ActorType::Interactive,initPosition,bods,jos)){
        return false;
    }
    _lightPos = initPosition;
    _dist = system->screenToBox2D(1000.0f);
    float angleNum = 360.0f / (NUMRAYS-1);
    for(int i=0; i<NUMRAYS; i++){
        float angle = angleNum * i;
        CCLOG("%f", angle);
        _sin[i] = sinf(CC_DEGREES_TO_RADIANS(angle));
        _cos[i] = cosf(CC_DEGREES_TO_RADIANS(angle));
        _end[i].x = _dist*_cos[i];
        _end[i].y = _dist*_sin[i];
    }
    _shadowLen = 100.0f;
    std::string frag =
            "\n#ifdef GL_ES\n"
            "precision lowp float;"
            "\n#endif\n"
            "varying vec4 v_fragmentColor;"
            "void main()"
            "{"
            "    gl_FragColor = v_fragmentColor;"
            "}";
    std::string vert =
            "attribute vec4 a_position;"
            "attribute vec4 a_color;"
            "attribute float s;"
            "\n#ifdef GL_ES\n"
            "varying lowp vec4 v_fragmentColor;"
            "\n#else\n"
            "varying vec4 v_fragmentColor;"
            "\n#endif\n"
            "void main()"
            "{"
            "    gl_Position = CC_PMatrix * a_position;"
            "    v_fragmentColor = s * a_color;"
            "}";

    auto glprogram = GLProgram::createWithByteArrays(vert.c_str(),frag.c_str());
    auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
    setGLProgramState(glprogramstate);
    auto s = Director::getInstance()->getWinSize();
    setPosition(Vec2::ZERO);
    auto size = Director::getInstance()->getWinSize();
    canvas = RenderTexture::create(size.width/2, size.height/2, Texture2D::PixelFormat::RGBA8888);
    canvas->setKeepMatrix(true);
    canvas->retain();
    return true;
}

DynamicLight *DynamicLight::create(B2PhysicsSystem *system, const b2Vec2 &initPosition) {
    auto tActor = new(std::nothrow)DynamicLight();
    if(tActor && tActor->init(system,initPosition)){
        tActor->autorelease();
        return tActor;
    }
    CC_SAFE_DELETE(tActor);
    return nullptr;
}

void DynamicLight::onEnter() {
    PhysicsActor::onEnter();
}

void DynamicLight::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) {

    _customCommand.init(_globalZOrder, transform, flags);
    _customCommand.func = CC_CALLBACK_0(DynamicLight::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void DynamicLight::onDraw(const Mat4 &transform, uint32_t flags) {
    GLProgramState *glProgramState = getGLProgramState();
    //glProgramState->setUniformVec4("u_color",Vec4(1.0f,1.0f,1.0f,0.25f));
    //glProgramState->setUniformFloat("u_pointSize", 100.0f);
    glProgramState->setVertexAttribPointer("a_color", 4, GL_FLOAT, GL_FALSE, 0,_cmr);
    glProgramState->setVertexAttribPointer("a_position",  2, GL_FLOAT, GL_FALSE, 0, _m);
    glProgramState->setVertexAttribPointer("s", 1, GL_FLOAT, GL_TRUE, 0, _f);
    glProgramState->apply(transform);
    glDrawArrays(GL_TRIANGLE_FAN, 0, NUMRAYS+1);
    glProgramState->setVertexAttribPointer("a_position",  2, GL_FLOAT, GL_FALSE, 0, _ss);
    glProgramState->setVertexAttribPointer("s", 1, GL_FLOAT, GL_FALSE, 0, _sf);
    glProgramState->setVertexAttribPointer("a_color", 4, GL_FLOAT, GL_FALSE, 0,_cr);
    glProgramState->apply(transform);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, NUMRAYS*2);
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,NUMRAYS+1);
}

float32 DynamicLight::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) {
    _m[m_index] = system->box2DToScreen(point.x);
    _m[m_index+1] = system->box2DToScreen(point.y);
    _f[m_index/2] = 1.0f - fraction;
    _sf[s_index/2] = 1.0f - fraction;
    _ss[s_index] = _m[m_index];
    _ss[s_index+1] = _m[m_index+1];
    _ss[s_index+2] = _m[m_index] + _sf[s_index/2] * _shadowLen * _cos[s_index/4];
    _ss[s_index+3]  = _m[m_index+1] + _sf[s_index/2] * _shadowLen * _sin[s_index/4];
    return fraction;
}

cocos2d::Vec2 DynamicLight::getDeltaMovement() {
    return cocos2d::Vec2();
}

void DynamicLight::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) {
    //Node::visit(renderer, parentTransform, parentFlags);
    if(Camera::getDefaultCamera() == Camera::getVisitingCamera()){

    }else {
        canvas->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        bool visibleByCamera = isVisitableByVisitingCamera();
        if (visibleByCamera)
            this->draw(renderer, _modelViewTransform, parentFlags);
        canvas->end();
    }
}
