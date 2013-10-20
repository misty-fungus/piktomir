//
//  Robot4.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__Robot4__
#define __pictomir_cocos2d_x__Robot4__

#include "PlayerRobot2D.h"

int sign (int a) ;

class Robot4 : public PlayerRobot2D
{
private:
    cocos2d::CCParticleFireworks *fireWorks;
public:
    
    virtual bool init();
    virtual void initTexture();
    
    Robot4(UniversalWorld *parent):
        PlayerRobot2D(parent)
    {}
    
    virtual ~Robot4()
    {
        //release();
    }
    
    static Robot4* create(UniversalWorld *parent)
    {
        Robot4 *robot = new Robot4(parent);
        
        robot->init();
        robot->autorelease();
        
        return robot;
    }
    
    enum Condition
    {
        GrassIsGreen = 1,
        GrassIsBlue = 2,
        NoWall = 3,
        Wall = 4
    };
    
    enum NativeMethod 
    {
        Move = 3,
        TurnLeft = 1,
        TurnRight = 2,
        Paint = 4,
        MoveReverse = 5,
        UnPaint = 6
    };

    virtual bool checkCondition(int condition);
    bool isNearWall();
    
    virtual void restartRobot();
    void setPosition(cocos2d::CCPoint point, int layer, int state);
    bool isGrassAt(cocos2d::CCPoint point, int layer);
    
//real moving
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    
    void endMove();
    void endPaint();
    void endUnPaint();
    void endTurn();
    void endTouch();
    
    virtual void destroy();
    virtual void win();
    
    virtual RobotType type ()
    {
        return pmRobot4;
    }
    
    void move();
    void moveReverse();
    void turnLeft();
    void turnRight();
    void paint();
    void unPaint();
    
    virtual int getNativeMethodCount()
    {
        return 5;
    }
    
    virtual int getConditionsCount()
    {
        return 4;
    }
    
    virtual RobotType parentType()
    {
        return pmPlayerRobot;
    }
};

#endif /* defined(__pictomir_cocos2d_x__Robot4__) */
