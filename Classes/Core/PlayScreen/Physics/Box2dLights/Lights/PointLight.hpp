//
// Created by Priyanshu Singh on 11/05/16.
//

#ifndef PLATFORMER_POINTLIGHT_HPP
#define PLATFORMER_POINTLIGHT_HPP

#include "PositionalLight.hpp"

namespace box2dLight {
    class PointLight: public PositionalLight {

    public:
        virtual void setDistance(float dist) override;

        virtual void setDirection(float directionDegree) override;

        virtual void update(float delta) override;

        void setEndPoints();
    };
}


#endif //PLATFORMER_POINTLIGHT_HPP
