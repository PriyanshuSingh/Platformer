//
// Created by Priyanshu Singh on 10/05/16.
//

#include "LightHandler.hpp"
#include "LightMap.hpp"


using namespace box2dLight;

const float LightHandler::GAMMA_COR = 0.625f;
bool LightHandler::gammaCorrection = false;
float LightHandler::gammaCorrectionParameter = 1.0f;

BlendFunc LightHandler::diffuseBlendFunc = {GL_DST_COLOR, GL_ZERO};
BlendFunc LightHandler::shadowBlend = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
BlendFunc LightHandler::simpleBlendFunc = {GL_SRC_ALPHA, GL_ONE};
const char *LightHandler::LIGHTMAP_NAME = "LightMapTag";

void LightHandler::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) {

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

    const Camera *cam = Camera::getVisitingCamera();



    setScreenCorners(cam);

    int i = 0;

    if(!_children.empty())
    {
        sortAllChildren();

        CCASSERT((lightMap != nullptr), "Cannot Render without lightMap");

        // set lightShaders and uniform MVP matrix
        GLProgramState *glProgramState;
        if(customLightShader != nullptr){
            CCLOG("not in here");
            glProgramState = GLProgramState::getOrCreateWithGLProgram(customLightShader);
        }
        else {
            glProgramState = GLProgramState::getOrCreateWithGLProgram(lightShader);
        }
        Mat4 u_MVP;
        Mat4::multiply(cam->getViewProjectionMatrix(), box2dToWorldMat, &u_MVP);
        glProgramState->setUniformMat4(LightMap::UNIFORM_NAME_VIEWPROJECTION_MATRIX, u_MVP);

//        Vec3 a( 665.107544f,665.107544f, 1024);
//        Vec3 ans;
//        Camera::getDefaultCamera()->getViewProjectionMatrix().transformPoint(Vec3(0,768, 1), &ans);

//        Vec3 aa = Camera::getDefaultCamera()->unproject(a);
//        CCLOG("viewProjection of defaultCamera ans ==> %f %f ",aa.x, aa.y);
//        box2dToWorldMat.transformPoint(a,&ans);
//        cam->getViewProjectionMatrix().transformPoint(ans, &a);
//        CCLOG("View Projection Matrix ans ==> %f %f ",a.x, a.y);
//        u_MVP.transformPoint(a,&ans);
//        CCLOG("UMVP ans ==> %f %f ",ans.x, ans.y);

//        glProgramState->setUniformMat4("u_viewProjection", u_MVP);
        glProgramState->setUniformFloat(LightMap::UNIFORM_NAME_ISGAMMA, (gammaCorrection)?1.0f:0.0f );
//        CCLOG("%d = uniform count in visit of lightHandler",glProgramState->getUniformCount());

        lightRenderedLastFrame = 0;

        bool useLightMap = ((shadows || blur) && ((lightMap->getCameraMask() & (unsigned short)Camera::getVisitingCamera()->getCameraFlag()) != 0));
        if(useLightMap){
            //Render to texture begin
            lightMap->beginWithClearRenderTexture();
        }

        Node * lightMapNode = getChildByName(LIGHTMAP_NAME);

        for( ; i < _children.size(); i++ )
        {
            auto node = _children.at(i);

            if (node && node->getLocalZOrder() < 0) {
                if(node != lightMapNode) {
                    node->setGLProgramState(glProgramState);
                    if(customLightShader)((Light *)node)->setUniformsForCustomLightShader();
                    ((Light *)node)->setBlendFunc(simpleBlendFunc);
                    node->visit(renderer, _modelViewTransform, flags);
                }else{
//                    CCLOG("Light Map is skipped, everything is ok!");
                }
            }
            else
                break;
        }

        // self draw
//        if (visibleByCamera) {
//            if(useLightMap)lightMap->endRenderTexture();
//            this->draw(renderer, _modelViewTransform, flags);
//            if(useLightMap)lightMap->beginRenderTexture();
//        }

        for(auto it=_children.cbegin()+i; it != _children.cend(); ++it) {
            if(*it != lightMapNode) {
                (*it)->setGLProgramState(glProgramState);
                if(customLightShader)((Light *)*it)->setUniformsForCustomLightShader();
                //This will be light so no need to cast and check
                ((Light *)*it)->setBlendFunc(simpleBlendFunc);
                (*it)->visit(renderer, _modelViewTransform, flags);
            }else{
//                CCLOG("Light Map is skipped, everything is ok!");
            }
        }

        if(useLightMap){
            //Render to texture end
            lightMap->endRenderTexture();

//            bool needed = lightRenderedLastFrame > 0;
            if( blur){
//                lightMap->gaussianBlur(renderer, _modelViewTransform, flags);
            }
        }

        lightMap->visit(renderer, Mat4::IDENTITY, 0);

    }
    else if (visibleByCamera)
    {
        this->draw(renderer, _modelViewTransform, flags);
    }

    _director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

    // FIX ME: Why need to set _orderOfArrival to 0??
    // Please refer to https://github.com/cocos2d/cocos2d-x/pull/6920
    // reset for next frame
    // _orderOfArrival = 0;

}

bool LightHandler::init(b2World *world) {

    if(!Node::init())return false;
    if(world == nullptr)return false;

    Size size = Director::getInstance()->getWinSize();
    setLightMap(LightMap::create(this, (int) (size.width/4.0f), (int) (size.height/4.0f)));

    commonInit(world);

    return true;
}

bool LightHandler::init(b2World *world, int textureHeight, int textureWidth) {
    if(!Node::init())return false;
    if(world == nullptr)return false;

    setLightMap(LightMap::create(this,  textureWidth, textureHeight));

    commonInit(world);

}


LightHandler *LightHandler::create(b2World *world) {

    LightHandler *ret = new (std::nothrow)LightHandler();
    if(ret && ret->init(world)){
        ret->autorelease();
        return ret;
    }else{
        delete(ret);
        ret = nullptr;
        return nullptr;
    }
}

void LightHandler::addChild(Node *child, int localZOrder, const std::string &name) {

#if COCOS2D_DEBUG >= 1
    Light *lightChild = dynamic_cast<Light *>(child);
    LightMap *lightMapChild = dynamic_cast<LightMap *>(child);

    if(!lightChild && !lightMapChild){
        CCASSERT(false, "LightHandler can have Light or LightMap as Children");
    }

    if(lightMapChild && lightMapCount >= 1){
        CCASSERT(false, "LightHandler cannot have more than one LightMap as child");
    }

    if(lightMapChild){
        lightMapCount++;
        CCASSERT(name == LIGHTMAP_NAME, "LightMap tag mismatched. Use static const defined in LightHandler class");
    }

#endif
    Node::addChild(child, localZOrder, name);
}

void LightHandler::addChild(Node *child, int localZOrder, int tag) {

#if COCOS2D_DEBUG >= 1
    Light *lightChild = dynamic_cast<Light *>(child);
    LightMap *lightMapChild = dynamic_cast<LightMap *>(child);

    if(!lightChild && !lightMapChild){
        CCASSERT(false, "LightHandler can have Light or LightMap as Children");
    }
    if(lightMapChild)CCASSERT(false, "integer tag for LightMap is not allowed, use string tag instead");

#endif
    Node::addChild(child, localZOrder, tag);
}

void LightHandler::addChild(Node *child) {
    Node::addChild(child);
}

void LightHandler::addChild(Node *child, int localZOrder) {
    Node::addChild(child, localZOrder);
}


void LightHandler::resizeRenderTexture(int fboWidth, int fboHeight) {
    if(lightMap != nullptr){
        lightMap->resizeRenderTexture(fboWidth, fboHeight);
    }
}

void LightHandler::setLightMap(LightMap *lightMap) {

    if(this->lightMap != nullptr){
        removeChild(this->lightMap, true);
        lightMapCount--;
    }
    this->lightMap = lightMap;
    if(this->lightMap != nullptr){
        addChild((Node *)this->lightMap, 1000 , LIGHTMAP_NAME);
    }
//    lightMap->setCameraMask((unsigned short) CameraFlag::USER1);
}

void LightHandler::setBox2dToWorldMat(Mat4 matrix) {
    box2dToWorldMat = matrix;
}

Mat4 LightHandler::getBox2dToWorldMat() {
    return box2dToWorldMat;
}



bool LightHandler::intersect(float x, float y, float radius) {
    return (x1 < (x+radius)) && (x2 > (x - radius)) && (y1 < (y + radius)) && (y2 > (y - radius));
}

void LightHandler::update(float delta) {

    for(Node *child: getChildren()){
        child->update(delta);
    }
}


void LightHandler::setScreenCorners(const Camera *pCamera) {
    auto size = Director::getInstance()->getWinSize();
    Vec3 topLeft = pCamera->unproject(Vec3(0, 0, 0.5));
    Vec3 bottomRight = pCamera->unproject(Vec3(size.width, size.height, 0.5));
//    Mat4 inv = box2dToWorldMat.getInversed();
//    inv.transformPoint(&topLeft);
//    inv.transformPoint(&bottomRight);

    x1 = topLeft.x;
    x2 = bottomRight.x;
    y1 = topLeft.y;
    y2 = bottomRight.y;
//    CCLOG("%f = x1 , %f = y1, %f = x2, %f = y2",x1,y1,x2,y2);
}

LightHandler::~LightHandler() {
    CC_SAFE_RELEASE(lightShader);
}

void LightHandler::commonInit(b2World *world) {
    lightShader = GLProgram::createWithFilenames("shaders/lightShader.vert", "shaders/lightShader.frag");
    CC_SAFE_RETAIN(lightShader);
    this->world = world;
    //TODO fix me (Hard Coded Values)
    box2dToWorldMat.scale(Vec3(64.0f, 64.0f, 1.0f));
}
