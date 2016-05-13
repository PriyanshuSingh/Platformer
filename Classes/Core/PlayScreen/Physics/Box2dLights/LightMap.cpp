//
// Created by Priyanshu Singh on 10/05/16.
//

#include "LightMap.hpp"

using namespace box2dLight;

//Attribute Name
const char *LightMap::ATTRIBUTE_NAME_FRACTION = "u_fraction";
const char *LightMap::UNIFORM_NAME_BLUR_AXIS = "u_blurAxis";
const char *LightMap::UNIFORM_NAME_VIEWPROJECTION_MATRIX = "u_viewProjection";

void LightMap::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {

    bool needed = _lightHandler->lightRenderedLastFrame > 0;

    if(lightMapDrawingDisabled)return;

    setTexture(mainCanvas->getSprite()->getTexture());

    GLProgramState *glProgramState = nullptr;

    if(_lightHandler->shadows){
        Color4F c = _lightHandler->ambientLight;
        if(_lightHandler->isDiffuse){
            glProgramState = GLProgramState::getOrCreateWithGLProgram(_diffuseShader);
            glProgramState->setUniformVec4(GLProgram::UNIFORM_NAME_AMBIENT_COLOR, Vec4(c.r, c.g, c.b, c.a));
            setGLProgramState(glProgramState);
            setBlendFunc(_lightHandler->diffuseBlendFunc);
        }else{
            glProgramState = GLProgramState::getOrCreateWithGLProgram(_shadowShader);
            glProgramState->setUniformVec4(GLProgram::UNIFORM_NAME_AMBIENT_COLOR, Vec4(c.r * c.a, c.g * c.a, c.b * c.a, 1.0f - c.a));
            setGLProgramState(glProgramState);
            setBlendFunc(_lightHandler->shadowBlend);
        }
    }else if (needed){
        glProgramState = GLProgramState::getOrCreateWithGLProgram(_withoutShadowShader);
        setGLProgramState(glProgramState);
        setBlendFunc(_lightHandler->simpleBlendFunc);
    }
    //TODO setting only one time will be enough But for that glProgramState must be cached and saved!
    if(glProgramState)glProgramState->setUniformMat4(UNIFORM_NAME_VIEWPROJECTION_MATRIX,viewProjectionMatrix);
    //Triangle Command to draw the sprite
    pushTriangleCommand(renderer,transform,flags);

}

void LightMap::gaussianBlur(Renderer *renderer, const Mat4 &transform, uint32_t flags) {

    auto glProgramState = GLProgramState::getOrCreateWithGLProgram(_blurShader);
    //TODO setting only one time will be enough But for that glProgramState must be cached and saved!
    glProgramState->setUniformMat4(UNIFORM_NAME_VIEWPROJECTION_MATRIX, viewProjectionMatrix);
    setBlendFunc(BlendFunc::DISABLE);
    for(int i=0; i<_lightHandler->blurNum; ++i){
        setTexture(mainCanvas->getSprite()->getTexture());
        mainCanvas->beginWithClear(0.0f ,0.0f, 0.0f, 0.0f);
        glProgramState->setUniformVec2(UNIFORM_NAME_BLUR_AXIS, Vec2(1, 0));

        setGLProgramState(glProgramState);
        pushTriangleCommand(renderer,transform,flags);
        mainCanvas->end();

        setTexture(mainCanvas->getSprite()->getTexture());
        mainCanvas->beginWithClear(0.0f ,0.0f, 0.0f, 0.0f);
        glProgramState->setUniformVec2(UNIFORM_NAME_BLUR_AXIS, Vec2(0, 1));
        setGLProgramState(glProgramState);
        pushTriangleCommand(renderer, transform, flags);
        mainCanvas->end();

    }

}


bool LightMap::init(LightHandler *lightHandler, int width, int height) {
    if(Sprite::init())return false;

    _lightHandler = lightHandler;

    _textureHeight = height;
    _textureWidth = width;

    //TODO No support for viewport change or screen resolution change

    auto director = Director::getInstance();
    Size size = director->getWinSize();

    Mat4::createOrthographicOffCenter(0, size.width, 0, size.height, -1024, 1024, &viewProjectionMatrix);

    mainCanvas = RenderTexture::create(_textureWidth, _textureHeight, Texture2D::PixelFormat::RGBA8888);
    CC_SAFE_RETAIN(mainCanvas);
    //TODO assuming Aspect Ratio is same
    setScale(size.height/_textureHeight);

    //TODO files for shader
    GLProgramState *glProgramState;

    Color4F c = _lightHandler->ambientLight;

    _shadowShader = GLProgram::createWithFilenames("","");
//    GLProgramCache::getInstance()->addGLProgram(_shadowShader, "shadowShader");

    glProgramState = GLProgramState::getOrCreateWithGLProgram(_shadowShader);
    glProgramState->setUniformVec4(GLProgram::UNIFORM_NAME_AMBIENT_COLOR, Vec4(c.r * c.a, c.g * c.a, c.b * c.a , 1.0f - c.a));


    _diffuseShader = GLProgram::createWithFilenames("","");
    glProgramState = GLProgramState::getOrCreateWithGLProgram(_diffuseShader);
    glProgramState->setUniformVec4(GLProgram::UNIFORM_NAME_AMBIENT_COLOR, Vec4(c.r, c.g, c.b ,c.a));


    _withoutShadowShader = GLProgram::createWithFilenames("","");
    glProgramState = GLProgramState::getOrCreateWithGLProgram(_withoutShadowShader);


    _blurShader = GLProgram::createWithFilenames("","");
    glProgramState = GLProgramState::getOrCreateWithGLProgram(_blurShader);
    glProgramState->setUniformVec2(UNIFORM_NAME_BLUR_AXIS, Vec2(1.0f, 0.0f));

    setFlippedY(true);
    setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    setPosition(size);

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

            if (node && node->_localZOrder < 0)
                node->visit(renderer, _modelViewTransform, flags);
            else
                break;
        }
        //LightMap must be drawn everytime for all Cameras
        this->draw(renderer, _modelViewTransform, flags);

        for(auto it=_children.cbegin()+i; it != _children.cend(); ++it)
            (*it)->visit(renderer, _modelViewTransform, flags);
    }
    else
    {
        this->draw(renderer, _modelViewTransform, flags);
    }

    _director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

}


void LightMap::pushTriangleCommand(Renderer *renderer, const Mat4 &transform, uint32_t flags) {

    _trianglesCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, _polyInfo.triangles, transform, flags);
    renderer->addCommand(&_trianglesCommand);
}
