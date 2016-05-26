//
// Created by ashish on 4/13/16.
//

#ifndef MYGAME_GAMECAMERA_HPP
#define MYGAME_GAMECAMERA_HPP



#include "cocos2d.h"

//camera class suited to game needs
class MainCamera :public cocos2d::Camera{
public:
    static MainCamera* create();
    virtual ~MainCamera();
    bool init()override;



    //set position instantly
    void setBack2DPosition();



    //restore original position of camera
    void setBackToDefault(float delta);
    //restore the original camera distance from the screen
    void setZoomBackToDefault(float delta);
    //restore original x,y coordinate
    void setBack2DPosition(float delta);



//TODO add ability to follow objects


    cocos2d::Vec2 getDownVector();
    void move2D(float delta,const cocos2d::Vec2 & displacement);



    void zoomIn(float delta,float depth,const cocos2d::Vec2 &  focusPoint);
    void zoomOut(float delta,float depth,const cocos2d::Vec2 & returnPoint);


    cocos2d::Vec2 getStart2dPosition();


//TODO make setPosition and its overloads private
//and make module container friend of this class
private:
    cocos2d::BillBoard * blackBoard = nullptr;
    cocos2d::Vec3 defaultPos;
    cocos2d::Vec3 maxPos;
    cocos2d::Vec3 minPos;
    cocos2d::Vec3 prevPos;


};


#endif //MYGAME_GAMECAMERA_HPP
