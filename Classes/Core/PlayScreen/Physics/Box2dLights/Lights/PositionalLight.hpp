//
// Created by Priyanshu Singh on 10/05/16.
//

#ifndef PLATFORMER_POSITIONALLIGHT_HPP
#define PLATFORMER_POSITIONALLIGHT_HPP


#include "Light.hpp"
namespace box2dLight {
    class PositionalLight : public Light {

    protected:
        Vec2 tmpEnd;
        Vec2 start;

        b2Body *body;
        Vec2 bodyOffset;
        float bodyAngleOffset;

        std::vector<float> sin;
        std::vector<float> cos;

        std::vector<Vec2> end;

    public:
        virtual Vec2 getStartPosition() override;

        virtual void setStartPosition(float x, float y) override;

        virtual void setStartPosition(Vec2 pos) override;

        virtual void attachToBody(b2Body *body) override;

        virtual void setRayNum(int rays) override;

        void updateBody(float d);

        bool cull();

        void updateMesh();

        void setMesh();

        void onDraw(const Mat4 &transform, uint32_t flags);


    public:
        virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

        virtual void update(float delta) override;

        virtual bool init(LightHandler *lightHandler, int rays, Color4F color, float distance, float directionDegree) override;


        void attachToBody(b2Body *pBody, float bodyOffsetX, float bodyOffsetY, float bodyAngleOffset);

        virtual bool contains(float x, float y) override;

        virtual b2Body *getBody() override;
    };
}


#endif //PLATFORMER_POSITIONALLIGHT_HPP
