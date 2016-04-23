//
// Created by ashish on 3/27/16.
//

#ifndef B2PHYSICSSYSTEM_HPP
#define B2PHYSICSSYSTEM_HPP



#include "Box2D/Box2D.h"
#include "cocos2d.h"

class B2PhysicsSystem {
public:


    //check if an instance of world exists
    static bool isSystemActive();


    B2PhysicsSystem(float32 ptmRatio,const b2Vec2 & gravity,bool interpolate);
    ~B2PhysicsSystem();


    void setUpdated(bool update);
    //returns whether the physics update has happened this tick or not
    bool hasUpdated(){return updated;}
    b2World* getWorld();



    void addOffset(b2Body * body,const b2Vec2 & offset);
    void addOffset(b2Joint * joint,const b2Vec2 & offset);




    float32 getPtmRatio(){return ptmRatio;}

    //utility methods
    float32 screenToBox2D(float32 in);
    float32 box2DToScreen(float32 in);

    b2Vec2 screenToBox2D(const cocos2d::Vec2 & in);
    cocos2d::Vec2 box2DToScreen(const b2Vec2 & in);


    //get visible origin in bo2d world
    b2Vec2 getB2VisibleOrigin();








    void interpolation(bool enable);

    //makes sure that the simulation updates in fixed
    //discrete time steps only
    void update(float deltaTime);





    //methods to pause/resume Physics Simulation
    void pause();
    void resume();



    //for slow motion effects and stuff
    void setSimulationSpeed(float scaleFactor);



    //TODO
    //add other support methods here
    //support for interpolation
    //support for deleting entities after a step
    //support for pausing the simulation(see testbed example)




//wrapper methods





public:
    //settings from Box2D TestBed :)
    struct Settings{

        //default settings
        Settings()
        {

            timeStep = (1/60.0f);
            velocityIterations = 8;
            positionIterations = 3;

        }




    public:
        float32 timeStep;
        int32 velocityIterations;
        int32 positionIterations;



    };

private:
    b2World * phyWorld = nullptr;
    bool interpolate;
    float32 ptmRatio;
    bool paused;
    bool updated;
    float scaleFactor = 1.0f;

    Settings worldSettings;











private:
    static bool made;


};


#endif //B2PHYSICSSYSTEM
