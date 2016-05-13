//
// Created by Priyanshu Singh on 10/05/16.
//

#ifndef PLATFORMER_LIGHTHANDLER_HPP
#define PLATFORMER_LIGHTHANDLER_HPP


#include "Box2D/Box2D.h"
#include "LightMap.hpp"
#include "Light.hpp"


USING_NS_CC;

namespace box2dLight{

    class LightHandler : public cocos2d::Node {

        friend class Light;
        friend class LightMap;
        friend class PositionalLight;

    private:

        bool isDiffuse;

        Color4F ambientLight;
        static const BlendFunc diffuseBlendFunc;
        static const BlendFunc shadowBlend;
        static const BlendFunc simpleBlendFunc;
        static const char *LIGHTMAP_NAME;

        GLProgram *customLightShader;
        GLProgram *lightShader;
        LightMap *lightMap;

        bool shadows;
        bool blur;


        int blurNum;
        int viewPortX;
        int viewPortY;
        int viewPortWidth;
        int viewPortHeight;

        int lightMapCount;
        int lightRenderedLastFrame;

        float x1, x2, y1, y2;
        bool culling;
        b2World *world;

        Mat4 box2dToWorldMat;

    public:
        static const float GAMMA_COR;
        static bool gammaCorrection;
        static float gammaCorrectionParameter;

        LightHandler() : Node(),
                         box2dToWorldMat(Mat4::IDENTITY),
                         ambientLight(Light::ZeroColor),
                         lightMapCount(0),
                         customLightShader(nullptr),
                         culling(true),
                         shadows(true),
                         blur(true),
                         blurNum(1),
                         viewPortX(0),
                         viewPortY(0),
                         viewPortWidth((int) Director::getInstance()->getWinSize().width),
                         viewPortHeight((int) Director::getInstance()->getWinSize().height),
                         lightRenderedLastFrame(0)
        {}

        void resizeRenderTexture(int fboWidth, int fboHeight);

        virtual void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;


        virtual bool init(b2World *world);

        virtual bool init(b2World *world, int textureHeight, int textureWidth);

        virtual void commonInit(b2World *world);

        virtual ~LightHandler();

        void setLightMap(LightMap *lightMap);

        static LightHandler *create(b2World *world);

        void setBox2dToWorldMat(Mat4 matrix);

        Mat4 getBox2dToWorldMat();

        bool intersect(float x, float y, float radius);

        virtual void update(float delta) override;

        virtual void setScreenCorners(const Camera *pCamera);

        // Just to ensure that Light and lightMap are the only children of LightHandler
        virtual void addChild(Node *child) override;

        virtual void addChild(Node *child, int localZOrder) override;

        virtual void addChild(Node *child, int localZOrder, int tag) override;

        virtual void addChild(Node *child, int localZOrder, const std::string &name) override;


    };
}


#endif //PLATFORMER_LIGHTHANDLER_HPP
