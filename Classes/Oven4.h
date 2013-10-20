//
//  Furnace.h
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 3/14/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__Furnace__
#define __pictomir_cocos2d_x__Furnace__

#include "EnvRobot2D.h"

class Oven4 : public EnvRobot2D
{
public:
    enum BurnState
    {
        Off = 0,
        On = 1
    };
    
    Oven4():
        EnvRobot2D(NULL),
        burnState(Off)
    {}
    
    static Oven4* create()  {
        return new Oven4();
    }
    
    Oven4(UniversalWorld *parent):
        EnvRobot2D(parent),
        burnState(Off)
    {}
    
    virtual ~Oven4()
    {
        //release();
    }
    
    static Oven4* create(UniversalWorld *parent)
    {
        Oven4* oven = new Oven4(parent);
        
        oven->init();
        oven->autorelease();
        
        return oven;
    }
    
    SYNTHESIZE(BurnState, burnState, BurnState);
    
    virtual bool init();
    //virtual void initWithXMLNode(xmlNodePtr robotNode);
    
    void setPosition(cocos2d::CCPoint point, int layer, int direction);
    
    virtual void touch (PlayerRobot* robot);
    
    virtual RobotType parentType()
    {
        return pmEnvRobot;
    }
    
    virtual RobotType type ()
    {
        return pmOven4;
    }
    
    virtual int getNativeMethodCount()
    {
        return 0;
    }
    
    virtual int getConditionsCount()
    {
        return 0;
    }
    
    virtual void restartRobot();
    
    virtual void initTexture();
    
    virtual bool moveOn()
    {
        return false;
    }
};

#endif /* defined(__pictomir_cocos2d_x__Furnace__) */
