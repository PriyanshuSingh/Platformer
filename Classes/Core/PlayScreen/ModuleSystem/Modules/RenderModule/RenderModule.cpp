//
// Created by ashish on 5/5/16.
//

#include "RenderModule.hpp"
#include "../../../Physics/b2PhysicsSystem.hpp"
#include "../../../MainCamera.hpp"

USING_NS_CC;




bool RenderModule::init(const PlayModule::staticInfo &info,B2PhysicsSystem * system, MainCamera *cam,
                        const b2Vec2 &offset) {
    if(!PlayModule::init(info,system,cam, offset)){
        return false;
    }



    auto size = Director::getInstance()->getWinSize();

    // create a render texture, this is what we are going to draw into
//    canvas = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
//    canvas->retain();


//    renderSprite = Sprite::createWithTexture(canvas->getSprite()->getTexture());
//    renderSprite->setTextureRect(Rect(0, 0, renderSprite->getTexture()->getContentSize().width, renderSprite->getTexture()->getContentSize().height));
//    renderSprite->setPosition(size.width,size.height/2);
//    renderSprite->setCameraMask((unsigned short) CameraFlag::DEFAULT);
//    renderSprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
//    renderSprite->setFlippedY(true);
//    addChild(renderSprite);
//    renderSprite->setVisible(false);
//    renderSprite->setCameraMask((unsigned short)CameraFlag::DEFAULT | (unsigned short)CameraFlag::USER1);
//
//
//    renderSprite->setScale(0.25);
    testSprite = Sprite::create("HelloWorld.png");
    testSprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    testSprite->setPosition(size);
    addChild(testSprite);




    return true;
}

RenderModule::~RenderModule() {

    CC_SAFE_RELEASE(canvas);
}


void RenderModule::preUpdate(float delta) {
    //cocos2d::log("here render module");
    PlayModule::preUpdate(delta);
    lt->prePhysicsUpdate(delta);
    renderSprite->setTexture(lt->getRenderSprite()->getTexture());
}

void RenderModule::postUpdate(float delta) {
    PlayModule::postUpdate(delta);
}

/*
void RenderModule::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) {

    canvas->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    if (!_visible)
    {
        return;
    }


    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    _director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    _director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);

    bool visibleByCamera = isVisitableByVisitingCamera();

    int i = 0;

    //visit everyone except render sprite
    if(!_children.empty())
    {
        sortAllChildren();
        // draw children zOrder < 0
        for( ; i < _children.size(); i++ )
        {
            auto node = _children.at(i);

            if (node && node->getLocalZOrder() < 0){
                if(node != renderSprite)
                    node->visit(renderer, _modelViewTransform, flags);
            }
            else
                break;
        }
        // self draw
        if (visibleByCamera)
            this->draw(renderer, _modelViewTransform, flags);

        for(auto it=_children.cbegin()+i; it != _children.cend(); ++it){
            if(*it != renderSprite)
                (*it)->visit(renderer, _modelViewTransform, flags);

        }
    }
    else if (visibleByCamera)
    {
        this->draw(renderer, _modelViewTransform, flags);
    }

    _director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);


    canvas->end();

    renderSprite->setTexture(canvas->getSprite()->getTexture());
    //render sprite in end
    renderSprite->visit(renderer,_modelViewTransform,flags);





}
*/
void RenderModule::onCoordsStable() {
    PlayModule::onCoordsStable();
    auto size = Director::getInstance()->getWinSize();
    lt = DynamicLight::create(system, system->screenToBox2D(Vec2(size.width*0.8f , size.height * 0.8f)));
    if(lt == nullptr)CCLOG("FUCK");
    addChild(lt,14);
    renderSprite = Sprite::createWithTexture(lt->getRenderSprite()->getTexture());
    addChild(renderSprite);
    renderSprite->setCameraMask((unsigned short) CameraFlag::DEFAULT,true);
    renderSprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    renderSprite->setFlippedY(true);
    renderSprite->setScale(2.0f);
    renderSprite->setPosition(size);
}

void RenderModule::onEnter() {
    Node::onEnter();
    renderSprite->setCameraMask((unsigned short) CameraFlag::DEFAULT);
    //auto glProgramState = renderSprite->getGLProgramState();
    std::string frag = "\n#ifdef GL_ES\n"
            "precision lowp float;\n"
            "\n#endif\n"
            "varying vec4 v_fragmentColor;\n"
            "varying vec2 v_texCoord;\n"
            "uniform vec4 ambient;\n"
            "uniform vec2 resolution;\n"
            "uniform float blurRadius;\n"
            "uniform float sampleNum;\n"
            "vec4 blur(vec2 p)\n"
            "{\n"
            "    if (blurRadius > 0.0 && sampleNum > 1.0)\n"
            "    {\n"
            "        vec4 col = vec4(0);\n"
            "        vec2 unit = 1.0 / resolution.xy;\n"
            "        \n"
            "        float r = blurRadius;\n"
            "        float sampleStep = r / sampleNum;\n"
            "        \n"
            "        float count = 0.0;\n"
            "        \n"
            "        for(float x = -r; x < r; x += sampleStep)\n"
            "        {\n"
            "            for(float y = -r; y < r; y += sampleStep)\n"
            "            {\n"
            "                float weight = (r - abs(x)) * (r - abs(y));\n"
            "                col += texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y)) * weight;\n"
            "                count += weight;\n"
            "            }\n"
            "        }\n"
            "        \n"
            "        return col / count;\n"
            "    }\n"
            "}\n"
            "void main()\n"
            "{\n"
            "    vec4 c = v_fragmentColor * blur(v_texCoord);\n"
            "    gl_FragColor.rgb = c.rgb * c.a + ambient.rgb;\n"
            "    gl_FragColor.a = ambient.a - c.a;\n"
            "}";

    auto glProgram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, frag.c_str());
    //glProgramState->setGLProgram(glProgram);
    auto glProgramState = GLProgramState::getOrCreateWithGLProgram(glProgram);
    auto ambientColor = Vec4(0.1f, 0.1f, 0.1f, 0.9f);
    glProgramState->setUniformVec4("ambient", ambientColor);
    glProgramState->setUniformVec2("resolution", renderSprite->getTexture()->getContentSizeInPixels());
    glProgramState->setUniformFloat("blurRadius", 8.0f);
    glProgramState->setUniformFloat("sampleNum", 8.0f);
    renderSprite->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    renderSprite->setGLProgramState(glProgramState);
}
