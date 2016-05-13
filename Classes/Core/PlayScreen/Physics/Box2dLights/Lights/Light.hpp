//
// Created by Priyanshu Singh on 10/05/16.
//

#ifndef PLATFORMER_LIGHT_HPP
#define PLATFORMER_LIGHT_HPP

USING_NS_CC;

#include "Box2D/Box2D.h"

namespace box2dLight {

// OpenGL compatible Vec2
    struct GVec2{
        GLfloat x;
        GLfloat y;

        inline GVec2(GLfloat _x, GLfloat _y):
                x(_x),
                y(_y)
        {}

        inline GVec2(const GVec2 &other):
                x(other.x),
                y(other.y)
        {}

        inline GVec2(const Vec2 &other):
                x(other.x),
                y(other.y)
        {}

        inline GVec2 set(const GVec2 &other){
            this->x = other.x;
            this->y = other.y;
        }

        inline GVec2 set(const Vec2 &other){
            this->x = other.x;
            this->y = other.y;
        }

        inline GVec2 &operator = (const Vec2 &other){
            set(other);
            return *this;
        }

        inline GVec2 &operator = (const GVec2 &other){
            set(other);
            return *this;
        }

        inline const GVec2 operator * (const GVec2 &other){
            return GVec2( x * other.x , y * other.y);
        }

        inline const GVec2 operator * (const GLfloat s){
            return GVec2( x * s , y * s);
        }

        inline const GVec2 operator / (const GLfloat s){
            return GVec2( x / s, y / s);
        }

        inline const GVec2 operator + (const GVec2 &other){
            return GVec2(x + other.x, y + other.y);
        }

        inline const GVec2 operator - (const GVec2 &other){
            return GVec2(x - other.x, y - other.y);
        }

        inline GVec2 &operator *= (const GVec2 &other){
            x += other.x;
            y += other.y;
            return *this;
        }

        inline GVec2 &operator += (const GVec2 &other) {
            x += other.x;
            y += other.y;
            return *this;
        }

        inline GVec2 &operator -= (const GVec2 &other){
            x -= other.x;
            y -= other.y;
            return *this;
        }

        inline GVec2 &operator /= (const GLfloat s){
            x /= s;
            y /= s;
            return *this;
        }

    };


    class LightHandler;

    class Light : public cocos2d::Node, public b2RayCastCallback {

    protected:

        CustomCommand _customCommand;

        Color4F color;
        Vec2 tmpPosition;

        LightHandler *lightHandler;

        bool soft;
        bool xray;
        bool staticLight;
        bool culled;
        bool dirty;
        bool ignoreBody;

        int rayNum;
        int vertexNum;

        float distance;
        float direction;

        float softShadowLength;

        int m_index;
        Vec2 *raycastPoints;
        float *raycastFraction;

        BlendFunc blendFunction;

        std::vector<GVec2> lightMeshPoints;
        std::vector<GVec2> softShadowMeshPoints;
        std::vector<GLfloat> lightFraction; //Vertex attribute for light
        std::vector<GLfloat> softShadowFraction; //Vertex attribute for soft shadow;
        std::vector<Color4F> lightColorAttrib; // Vertex attribute color for light
        std::vector<Color4F> softShadowColorAttrib; //Vertex attribute color for soft shadow
        virtual float32 ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) override;

    public:
        static const Color4F DefaultColor;
        static const Color4F ZeroColor;
        static const int MIN_RAYS;

        Light() :
                color(ZeroColor),
                soft(true),
                xray(false),
                m_index(0),
                staticLight(false),
                culled(false),
                dirty(true),
                ignoreBody(false),
                softShadowLength(2.5f){
        }

        virtual ~Light() {
            if (raycastPoints != nullptr)CC_SAFE_DELETE(raycastPoints);
            if (raycastFraction != nullptr)CC_SAFE_DELETE(raycastFraction);

        }


        virtual bool init(LightHandler *lightHandler, int rays, Color4F color, float distance, float directionDegree);

        virtual void update(float delta) override {
        }


        virtual void setDistance(float dist) = 0;

        virtual void setDirection(float directionDegree) = 0;

        virtual void attachToBody(b2Body *body) = 0;

        virtual b2Body *getBody() = 0;

        virtual void setStartPosition(float x, float y) = 0;

        virtual void setStartPosition(Vec2 pos) = 0;

        virtual Vec2 getStartPosition();

        void setColor(Color4F color);

        void setColor(float r, float g, float b, float a);

        virtual void setLightHandler(LightHandler *lightHandler);

        virtual void removeLightHandler();

        bool isXray();

        void setXray(bool xray);

        bool isStaticLight();

        void setStaticLight(bool staticLight);

        bool isSoft();

        void setSoft(bool soft);

        float getSoftShadowLength();

        void setSoftnessLength(float softShadowLength);

        Color4F getColor();

        float getDistance();

        float getDirection();

        virtual bool contains(float x, float y);

        void setIgnoreAttachedBody(bool flag);

        bool isIgnoreAttachedBody();

        virtual void setRayNum(int rays);

        int getRayNum();

        void setContactFilter(b2Filter filter);

        void setContactFilter(uint16 categoryBits, uint16 groupIndex, uint16 maskBits);

        bool contactFilter(b2Fixture *fixtureB);

        bool globalContactFilter(b2Fixture *fixtureB);

        static void setGlobalContactFilter(b2Filter filter);

        static void setGlobalContactFilter(uint16 categoryBits, uint16 groupIndex, uint16 maskBits);


        void setBlendFunc(const BlendFunc func);

        virtual void setUniformsForCustomLightShader();

    private:
        static b2Filter globalFilterA;
        b2Filter filterA;

    };
}

#endif //PLATFORMER_LIGHT_HPP
