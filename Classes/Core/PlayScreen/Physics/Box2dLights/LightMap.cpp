//
// Created by Priyanshu Singh on 10/05/16.
//

#include "LightMap.hpp"
#include "LightHandler.hpp"
using namespace box2dLight;

//Attribute Name
const char *LightMap::ATTRIBUTE_NAME_FRACTION = "a_fraction";
const char *LightMap::UNIFORM_NAME_BLUR_AXIS = "u_blurAxis";
const char *LightMap::UNIFORM_NAME_VIEWPROJECTION_MATRIX = "Proj";
const char *LightMap::UNIFORM_NAME_WINSIZE = "u_winsize";
const char *LightMap::UNIFORM_NAME_ISDIFFUSE = "u_isDiffuse";
const char *LightMap::UNIFORM_NAME_ISGAMMA = "u_isGamma";

void LightMap::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {

    bool needed = _lightHandler->lightRenderedLastFrame > 0;
    CCLOG("%d <-lightHandler",_lightHandler->lightRenderedLastFrame);
    if(lightMapDrawingDisabled)return;

//    setTexture(mainCanvas->getSprite()->getTexture());

//    renderer->addCommand(&_preBlurCommand);

    GLProgramState *glProgramState = nullptr;

    if(_lightHandler->shadows){
        Color4F c = _lightHandler->ambientLight;
        if(_lightHandler->isDiffuse){
//            CCLOG("HEre in diffuse shader");

            glProgramState = GLProgramState::getOrCreateWithGLProgram(_diffuseShader);
            glProgramState->setUniformVec4("AmbientColor", Vec4(c.r, c.g, c.b, c.a));
            setGLProgramState(glProgramState);
            setBlendFunc(_lightHandler->diffuseBlendFunc);
        }else{
//            CCLOG("here in shadow shader %d",_lightHandler->lightRenderedLastFrame);
//            c = Color4F(0.0f, 0.0f, 0.0f, 0.0f);
            glProgramState = GLProgramState::getOrCreateWithGLProgram(_shadowShader);
//            glProgramState->setUniformMat4(UNIFORM_NAME_VIEWPROJECTION_MATRIX,viewProjectionMatrix);
//            glProgramState->setUniformVec4(GLProgram::UNIFORM_NAME_AMBIENT_COLOR, Vec4(c.r * c.a, c.g * c.a, c.b * c.a, 1.0f - c.a));
            glProgramState->setUniformVec4("AmbientColor", Vec4(c.r * c.a, c.g * c.a, c.b * c.a, 1.0f - c.a));
//            glProgramState->setUniformVec4("AmbientColor", Vec4(c.r, c.g, c.b, c.a));
            setGLProgramState(glProgramState);
            setBlendFunc(_lightHandler->shadowBlend);
        }
    }else if(needed){
        glProgramState = GLProgramState::getOrCreateWithGLProgram(_withoutShadowShader);
        setGLProgramState(glProgramState);
        setBlendFunc(_lightHandler->simpleBlendFunc);
    }
    //TODO setting only one time will be enough But for that glProgramState must be cached and saved!
    if(glProgramState)glProgramState->setUniformMat4(UNIFORM_NAME_VIEWPROJECTION_MATRIX,viewProjectionMatrix);
    //Triangle Command to draw the sprite
//    pushTriangleCommand(renderer,transform,flags);
//    renderer->addCommand(&_blur1);
    Sprite::draw(renderer, _modelViewTransform, flags);
}

void LightMap::gaussianBlur(Renderer *renderer, const Mat4 &transform, uint32_t flags) {

    auto glProgramState = GLProgramState::getOrCreateWithGLProgram(_blurShader);
    //TODO setting only one time will be enough But for that glProgramState must be cached and saved!
    glProgramState->setUniformMat4(UNIFORM_NAME_VIEWPROJECTION_MATRIX, viewProjectionMatrix);
    glProgramState->setUniformVec2(UNIFORM_NAME_WINSIZE,Vec2(_textureWidth ,_textureHeight));
    _preBlurCommand.init(_globalZOrder, transform, flags);
    _preBlurCommand.func = CC_CALLBACK_0(LightMap::onPreBlurDraw, this, transform, flags, &sprite, &texture2D);
//    _postBlurCommand.init(_globalZOrder, transform, flags);
//    _postBlurCommand.func = CC_CALLBACK_0(LightMap::onPreBlurDraw, this, transform, flags, false);

    if(_lightHandler->isDiffuse){
        glProgramState->setUniformFloat(UNIFORM_NAME_ISDIFFUSE,1.0f);
    }else{
        glProgramState->setUniformFloat(UNIFORM_NAME_ISDIFFUSE,0.0f);
    }
//    setBlendFunc(BlendFunc::DISABLE);

    sprite = Sprite::createWithTexture(mainCanvas->getSprite()->getTexture());
    sprite->setFlippedY(true);

    for(int i=0; i<_lightHandler->blurNum; ++i){
//        setTexture(mainCanvas->getSprite()->getTexture());
        //TODO Ashish as of now the loop only runs once so there is no problem but as the blurNum > 1 then i am not able to set the texture of sprite to pinpong2's texture at the end of the loop. (see the TODO below)
        auto pingPongCanvas1 = RenderTexture::create(_textureWidth, _textureHeight, Texture2D::PixelFormat::RGBA8888);
        auto pingPongCanvas2 = RenderTexture::create(_textureWidth, _textureHeight, Texture2D::PixelFormat::RGBA8888);
        CCLOG("%d %d in loop",pingPongCanvas1->getSprite()->getTexture()->getName(), pingPongCanvas2->getSprite()->getTexture()->getName());


        Sprite *spritePingPong1 = pingPongCanvas1->getSprite();
        Sprite *spritePingPong2 = pingPongCanvas2->getSprite();
//        spriteMain->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
//        spritePingPong->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
//        spritePingPong->setPosition(Vec2::ZERO);
//        spriteMain->setPosition(Vec2::ZERO);
//        spriteMain->setTextureRect(Rect(0, 0, getTexture()->getContentSize().width, getTexture()->getContentSize().height));
//        spritePingPong->setTextureRect(Rect(0, 0, getTexture()->getContentSize().width, getTexture()->getContentSize().height));
//        spritePingPong->setScale(4.0f);
//        spriteMain->setScale(4.0f);
//        spriteMain->setTextureRect(Rect(0, getTexture()->getContentSize().width, 0, getTexture()->getContentSize().height));
//        spritePingPong->setTextureRect(Rect(0, getTexture()->getContentSize().width, 0, getTexture()->getContentSize().height));
//        spriteMain->setFlippedY(true);
//        spritePingPong->setFlippedY(true);
//        spritePingPong2->setFlippedY(true);
//        CCLOG("spriteMain = %f spritePingPong =  %f spritePingPong2 %f",spriteMain->getGlobalZOrder(), spritePingPong->getGlobalZOrder(), spritePingPong2->getGlobalZOrder());

        sprite->setBlendFunc(BlendFunc::DISABLE);
        spritePingPong1->setBlendFunc(BlendFunc::DISABLE);
//        renderer->addCommand(&_preBlurCommand);
//        pingPongCanvas->setGlobalZOrder(0);
        pingPongCanvas1->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        glProgramState->setUniformVec2(UNIFORM_NAME_BLUR_AXIS, Vec2(1, 0));
        //setGLProgramState(glProgramState);
//        Sprite::draw(renderer,transform,flags);
        sprite->setGLProgramState(glProgramState);
//        Mat4 a = spriteMain->getParentToNodeTransform();
//        a.multiply(_modelViewTransform);
        sprite->draw(renderer, transform, flags);
        //pushTriangleCommand(renderer,transform, flags, &_blur1);
//        renderer->addCommand(&_blur1);
        pingPongCanvas1->end();




//        CCLOG(">>>%d %d",spriteMain->getTexture()->getName(), spritePingPong->getTexture()->getName());
//        mainCanvas->beginWithClear(0.2, 0.2, 0.2, 1);
//        glProgramState->setUniformVec2(UNIFORM_NAME_BLUR_AXIS, Vec2(0, 1));
//////        setGLProgramState(glProgramState);
//////        Sprite::draw(renderer, transform, flags);
//////        renderer->addCommand(&_blur2);
//        spritePingPong->setGLProgramState(glProgramState);
////            pushTriangleCommand(renderer, transform, flags, &_blur2);
//        spritePingPong->draw(renderer, transform, flags);
//        mainCanvas->end();
//        CCLOG("%d %d<<<",spriteMain->getTexture()->getName(), spritePingPong->getTexture()->getName());


        pingPongCanvas2->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
        glProgramState->setUniformVec2(UNIFORM_NAME_BLUR_AXIS, Vec2(1, 0));
        spritePingPong1->setGLProgramState(glProgramState);
        spritePingPong1->draw(renderer,transform, flags);
        pingPongCanvas2->end();


        //texture2D = spritePingPong2->getTexture();
        //TODO Ashish this line sets the texture of sprite before rendering hence nothing will get render and texture with (0,0,0,0) will get rendered at the end.
        // sprite->setTexture(spritePingPong2->getTexture());
        // this works




////        renderer->addCommand(&_postBlurCommand);
////        setTexture(pingPongCanvas->getSprite()->getTexture());
//        mainCanvas->beginWithClear(0.0f ,0.0f, 0.0f, 0.0f);
//        glProgramState->setUniformVec2(UNIFORM_NAME_BLUR_AXIS, Vec2(0, 1));
//////        setGLProgramState(glProgramState);
//////        Sprite::draw(renderer, transform, flags);
//////        renderer->addCommand(&_blur2);
//        spritePingPong->setGLProgramState(glProgramState);
//        a = spritePingPong->getParentToNodeTransform();
//        a.multiply(_modelViewTransform);
//////        pushTriangleCommand(renderer, transform, flags, &_blur2);
//        spritePingPong->draw(renderer, a, flags);
//        mainCanvas->end();
//
//        glProgramState = GLProgramState::getOrCreateWithGLProgram(_withoutShadowShader);
//        glProgramState->setUniformMat4(UNIFORM_NAME_VIEWPROJECTION_MATRIX, viewProjectionMatrix);
//        a = spriteMain->getParentToNodeTransform();
//        a.multiply(_modelViewTransform);
//        spriteMain->draw(renderer,a, flags);

    }
    Color4F c = _lightHandler->ambientLight;
    glProgramState = GLProgramState::getOrCreateWithGLProgram(_shadowShader);
//        glProgramState->setUniformVec4("AmbientColor", Vec4(c.r, c.g, c.b, c.a));
    //setGLProgramState(glProgramState);
//        spriteMain->setBlendFunc(_lightHandler->diffuseBlendFunc);
//        glProgramState = GLProgramState::getOrCreateWithGLProgram(_withoutShadowShader);
    glProgramState->setUniformVec4("AmbientColor", Vec4(c.r * c.a, c.g * c.a, c.b * c.a, 1.0f - c.a));
    glProgramState->setUniformMat4(UNIFORM_NAME_VIEWPROJECTION_MATRIX, viewProjectionMatrix);
    sprite->setGLProgramState(glProgramState);
    sprite->setBlendFunc(_lightHandler->shadowBlend);
//    sprite->setBlendFunc(_lightHandler->diffuseBlendFunc);
    sprite->draw(renderer,transform, flags);


//    glProgramState->setUniformVec2(UNIFORM_NAME_BLUR_AXIS, Vec2(1, 0));
//    setGLProgramState(glProgramState);
//    setScale(4.0f);
//    setScale(8.0f);
//    setTexture(mainCanvas->getSprite()->getTexture());
//    Sprite::draw(renderer, transform, flags);
}


bool LightMap::init(LightHandler *lightHandler, int width, int height) {
    if(!Sprite::init())return false;

    _lightHandler = lightHandler;

    _textureHeight = height;
    _textureWidth = width;

    //TODO No support for viewport change or screen resolution change

    auto director = Director::getInstance();
    Size size = director->getWinSize();

    Mat4::createOrthographicOffCenter(0, size.width, 0, size.height, -1024, 1024, &viewProjectionMatrix);
//    float zeye = Director::getInstance()->getZEye();
//    Mat4::createPerspective(60, (GLfloat)size.width / size.height, 10,  zeye + size.height / 2.0f, &viewProjectionMatrix);
    mainCanvas = RenderTexture::create(_textureWidth, _textureHeight, Texture2D::PixelFormat::RGBA8888);
    CC_SAFE_RETAIN(mainCanvas);
//    pingPongCanvas = RenderTexture::create(_textureWidth, _textureHeight, Texture2D::PixelFormat::RGBA8888);
//    CC_SAFE_RETAIN(pingPongCanvas);
//    pingPongCanvas2 = RenderTexture::create(_textureWidth, _textureHeight, Texture2D::PixelFormat::RGBA8888);
//    CC_SAFE_RETAIN(pingPongCanvas2);
    //TODO assuming Aspect Ratio is same
    setScale(size.height/_textureHeight);
    setTexture(mainCanvas->getSprite()->getTexture());
    setTextureRect(Rect(0, 0, getTexture()->getContentSize().width, getTexture()->getContentSize().height));
    //TODO files for shader

    _shadowShader = GLProgram::createWithFilenames("shaders/shadowShader.vert","shaders/shadowShader.frag");
    //CCLOG("shadowShader %s",_shadowShader->getProgramLog().c_str());
    CCASSERT(_shadowShader,"shadowShader null");
    CC_SAFE_RETAIN(_shadowShader);
    //GLProgramCache::getInstance()->addGLProgram(_shadowShader, "shadowShader");


    _diffuseShader = GLProgram::createWithFilenames("shaders/diffuseShader.vert","shaders/diffuseShader.frag");
    CCASSERT(_diffuseShader,"diffuseshader null");
    CC_SAFE_RETAIN(_diffuseShader);

    _withoutShadowShader = GLProgram::createWithFilenames("shaders/withoutShadowShader.vert", "shaders/withoutShadowShader.frag");
    CCASSERT(_withoutShadowShader,"withoutShadowShader null");
    CC_SAFE_RETAIN(_withoutShadowShader);

    _blurShader = GLProgram::createWithFilenames("shaders/blurShader.vert","shaders/blurShader.frag");
    CCASSERT(_blurShader,"blurShader null");
    CC_SAFE_RETAIN(_blurShader);

    setFlippedY(true);
    setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    setPosition(Vec2::ZERO);




    return true;
}

LightMap *LightMap::create(LightHandler *lightHandler, int width, int height) {

    LightMap *ret = new(std::nothrow) LightMap();
    if(ret && ret->init(lightHandler, width, height)){
        ret->autorelease();
        return ret;
    }else{
        delete(ret);
        ret = nullptr;
        return nullptr;
    }
}



void LightMap::resizeRenderTexture(int width, int height) {
    CC_SAFE_RELEASE(mainCanvas);
    mainCanvas = RenderTexture::create(_textureWidth, _textureHeight, Texture2D::PixelFormat::RGBA8888);
    _textureWidth = width;
    _textureHeight = height;

    Size size = Director::getInstance()->getWinSize();
    //TODO assuming Aspect Ratio is same
    setScale(size.height/_textureHeight);

    CC_SAFE_RETAIN(mainCanvas);
}

LightMap::~LightMap() {
    CC_SAFE_RELEASE(mainCanvas);
//    CC_SAFE_RELEASE(pingPongCanvas);
    CC_SAFE_RELEASE(_blurShader);
    CC_SAFE_RELEASE(_diffuseShader);
    CC_SAFE_RELEASE(_withoutShadowShader);
    CC_SAFE_RELEASE(_shadowShader);
//    CC_SAFE_RELEASE(pingPongCanvas2);

}

void LightMap::beginWithClearRenderTexture() {
    mainCanvas->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
}

void LightMap::endRenderTexture() {
    mainCanvas->end();
}

void LightMap::beginRenderTexture() {
    mainCanvas->begin();
}

void LightMap::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) {
    // quick return if not visible. children won't be drawn.
    if (!_visible)
    {
        return;
    }

    uint32_t flags = processParentFlags(parentTransform, parentFlags);

    // IMPORTANT:
    // To ease the migration to v3.0, we still support the Mat4 stack,
    // but it is deprecated and your code should not rely on it
    _director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    _director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);

    bool visibleByCamera = isVisitableByVisitingCamera();

    int i = 0;

    if(!_children.empty())
    {
        sortAllChildren();
        // draw children zOrder < 0
        for( ; i < _children.size(); i++ )
        {
            auto node = _children.at(i);

            if (node && node->getLocalZOrder() < 0)
                node->visit(renderer, _modelViewTransform, flags);
            else
                break;
        }
        //LightMap must be drawn everytime for all Cameras
        if(visibleByCamera){
            if(_lightHandler->blur){gaussianBlur(renderer,_modelViewTransform, flags);}
            this->draw(renderer, _modelViewTransform, flags);
        }

        for(auto it=_children.cbegin()+i; it != _children.cend(); ++it)
            (*it)->visit(renderer, _modelViewTransform, flags);
    }
    else
    {
        if(visibleByCamera){
            if(_lightHandler->blur){gaussianBlur(renderer,_modelViewTransform, flags);}
//            this->draw(renderer, _modelViewTransform, flags);
        }
    }

    _director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

}


void LightMap::pushTriangleCommand(Renderer *renderer, const Mat4 &transform, uint32_t flags, TrianglesCommand *cmd) {

    CCLOG("%d",_texture->getName());
    cmd->init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, _polyInfo.triangles, transform, flags);
    _trianglesCommand.setSkipBatching(true);
    renderer->addCommand(cmd);
}

void LightMap::onPreBlurDraw(const Mat4 &transform, uint32_t flags, Sprite **sprite, Texture2D **texture) {
    (*sprite)->setTexture(*texture);
}

void LightMap::onPostBlurDraw(const Mat4 &transform, uint32_t flags) {

}

