//
// Created by ashish on 5/5/16.
//

#include "../../../Physics/PhysicsActor.hpp"
#include "RenderModule.hpp"
USING_NS_CC;


void RenderModule::onEnter() {
    Node::onEnter();





}
bool RenderModule::init(const PlayModule::staticInfo &info, B2PhysicsSystem *system, MainCamera *cam,
                        const b2Vec2 &offset) {
    if(!PlayModule::init(info, system, cam, offset)){
        return false;
    }


    const auto & size = _director->getWinSize();
    //render texture setup
    {
        // create a render texture, this is what we are going to draw into
        canvas = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
        canvas->setKeepMatrix(true);
        canvas->retain();

        //sprite setup
        renderSprite = Sprite::createWithTexture(canvas->getSprite()->getTexture());
        renderSprite->setPosition(Vec2::ZERO);
        renderSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        renderSprite->setFlippedY(true);

        addChild(renderSprite);
        renderSprite->setCameraMask((unsigned short)CameraFlag::USER1);



        //TODO see if this shader added to cache
        auto vShaderFilename = "Shaders/MatrixShader.vert";
        auto fileUtils = FileUtils::getInstance();
        std::string vertexSource = fileUtils->getStringFromFile(FileUtils::getInstance()->fullPathForFilename(vShaderFilename));

        auto renderSpriteGlProgram = GLProgram::createWithByteArrays(vertexSource.c_str(),ccPositionTextureColor_noMVP_frag);
        renderSprite->setGLProgramState(GLProgramState::getOrCreateWithGLProgram(renderSpriteGlProgram));


    }


    {
        testSprite = Sprite::create("HelloWorld.png");
        testSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        testSprite->setPosition(Vec2(0, size.height));
        addChild(testSprite);


        //node filter
        setNodeFilter([this](Node *n) -> bool {
            return !(n == testSprite);

        });

    }









    return true;
}

RenderModule::~RenderModule() {

    CC_SAFE_RELEASE(canvas);

}


void RenderModule::prePhysicsUpdate(float delta) {

    PlayModule::prePhysicsUpdate(delta);
    newActor->prePhysicsUpdate(delta);
}

void RenderModule::postPhysicsUpdate(float delta) {
    PlayModule::postPhysicsUpdate(delta);
    newActor->postPhysicsUpdate(delta);
}


void RenderModule::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) {





    auto visCam = Camera::getVisitingCamera();
    if(visCam->getCameraFlag() == CameraFlag::USER1)
    {

        //update matrix and make flags
        uint32_t flags = processParentFlags(parentTransform,parentFlags);
        std::vector<Node *>leftNodes;
        //draw stuff into render texture
        {
            canvas->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
            internalVisit(renderer, flags, leftNodes);
            canvas->end();
        }
        //sprite shader stuff
        {
            setupSprite.init(_globalZOrder);
            setupSprite.func = [this]() {
                //setup sprite
                const Size &size = _director->getWinSize();
                //load orthographic matrix
                //TODO save this matrix and avoid calculating again
                Mat4 orthoMatrix;
                Mat4::createOrthographicOffCenter(0, size.width, 0, size.height, -1024, 1024, &orthoMatrix);
                renderSprite->getGLProgramState()->setUniformMat4("mvpMatrix", orthoMatrix);


            };
            renderer->addCommand(&setupSprite);
        }

        if(show && renderBefore){
            renderSprite->visit(renderer, Mat4::IDENTITY, 0);
        }

        for(size_t i = 0;i<leftNodes.size();++i){
            leftNodes[i]->visit(renderer,_modelViewTransform,flags);

        }
        leftNodes.clear();


        if(show &&!renderBefore){
            renderSprite->visit(renderer,Mat4::IDENTITY,0);
        }

    }

}

void RenderModule::internalVisit(cocos2d::Renderer *renderer,uint32_t flags,std::vector<Node *> & filteredNodes) {




    if (!_visible)
    {
        return;
    }


    _director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    _director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);

    bool visibleByCamera = isVisitableByVisitingCamera();

    int i = 0;

    if(!_children.empty()) {
        sortAllChildren();
        // draw children zOrder < 0
        for (; i < _children.size(); i++) {
            auto node = _children.at(i);

            if (node && node->getLocalZOrder() < 0 && node != renderSprite) {
                if (filter(node)) {
                    node->visit(renderer, _modelViewTransform, flags);
                }
                else {
                    filteredNodes.push_back(node);
                }
            }
            else
                break;
        }
        // self draw
        if (visibleByCamera) {
            if (filter(this))draw(renderer, _modelViewTransform, flags);
            else filteredNodes.push_back(this);
        }
        for (auto it = _children.cbegin() + i; it != _children.cend(); ++it){
            if (*it != renderSprite) {
                if (filter(*it)) {
                    (*it)->visit(renderer, _modelViewTransform, flags);
                }
                else {
                    filteredNodes.push_back(*it);
                }
            }
        }
    }
    else if (visibleByCamera)
    {

        if (filter(this))draw(renderer, _modelViewTransform, flags);
        else filteredNodes.push_back(this);
    }

    _director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);


}


bool RenderModule::filter(Node *n) {

    return (!filterFunc || filterFunc(n));
}


void RenderModule::onCoordsStable() {
    PlayModule::onCoordsStable();

    newActor = TestActor::create(system,boxInitOffset+b2Vec2(2,10));
    addChild(newActor);


}

void RenderModule::setShow(bool val) {
    show=val;
}


void RenderModule::setRenderBefore(bool should) {
    renderBefore = should;

}


void RenderModule::setNodeFilter(const std::function<bool(cocos2d::Node *)> & filterFunc) {
    this->filterFunc = filterFunc;
}


