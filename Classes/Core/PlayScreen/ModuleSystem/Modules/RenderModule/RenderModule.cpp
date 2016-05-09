//
// Created by ashish on 5/5/16.
//

#include "../../../Physics/PhysicsActor.hpp"
#include "RenderModule.hpp"
USING_NS_CC;


void RenderModule::onEnter() {
    Node::onEnter();


    //important to do this here(init of the very first module called after which playscreen resets the
    //the camera mask for all children)

    renderSprite->setCameraMask((unsigned short)CameraFlag::DEFAULT);



    cocos2d::log("here to");



}


bool RenderModule::init(const PlayModule::staticInfo &info, B2PhysicsSystem *system, MainCamera *cam,
                        const b2Vec2 &offset) {
    if(!PlayModule::init(info, system, cam, offset)){
        return false;
    }





    auto size = Director::getInstance()->getWinSize();

    // create a render texture, this is what we are going to draw into
    canvas = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
    canvas->setKeepMatrix(true);
    canvas->retain();



    renderSprite = Sprite::createWithTexture(canvas->getSprite()->getTexture());
    renderSprite->setTextureRect(Rect(0, 0, renderSprite->getTexture()->getContentSize().width, renderSprite->getTexture()->getContentSize().height));
    renderSprite->setPosition(Vec2::ZERO);
    renderSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    renderSprite->setFlippedY(true);
    renderSprite->setName("renderSprite");
    if(this->getCameraMask() == (unsigned short)CameraFlag::DEFAULT){
        cocos2d::log("fucked hard by this logic");
    }
    if(_running){
        cocos2d::log("i am running");
    }
    addChild(renderSprite);

    renderSprite->setOpacity(64);


    testSprite = Sprite::create("HelloWorld.png");
    testSprite->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    testSprite->setPosition(Vec2(0,size.height));
    addChild(testSprite);





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


    if(Camera::getDefaultCamera() == Camera::getVisitingCamera()) {



        renderSprite->visit(renderer,Mat4::IDENTITY, 0);

    }
    else {

        if (!_visible) {
            return;
        }


        canvas->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);


        _director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
        _director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);


        int i = 0;


        uint32_t flags = processParentFlags(parentTransform, parentFlags);

        //visit everyone except render sprite
        if (!_children.empty()) {
            sortAllChildren();
            // draw children zOrder < 0
            for (; i < _children.size(); i++) {
                auto node = _children.at(i);

                if (node && node != renderSprite)
                    node->visit(renderer, _modelViewTransform, flags);

            }

            _director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
            canvas->end();

        }


    }


}


void RenderModule::onCoordsStable() {
    PlayModule::onCoordsStable();

    newActor = TestActor::create(system,boxInitOffset+b2Vec2(2,10));
    addChild(newActor);


}

