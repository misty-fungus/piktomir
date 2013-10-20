//
//  TransportRobot4.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/16/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__TransportRobot4__
#define __pictomir_cocos2d_x__TransportRobot4__

#include <iostream>
#include "EnvRobot2D.h"

class TransportRobot4 : public EnvRobot2D
{
public:

    TransportRobot4():
        EnvRobot2D(NULL)
    {}
    
    static TransportRobot4* create()  {
        return new TransportRobot4();
    }
    
    TransportRobot4(UniversalWorld *parent):
        EnvRobot2D(parent)
    {}
    
    virtual ~TransportRobot4()
    {
        //release();
    }
    
    static TransportRobot4* create(UniversalWorld *parent)
    {
        TransportRobot4* robot = new TransportRobot4(parent);
        
        robot->init();
        robot->autorelease();
        
        return robot;
    }
    
    virtual bool init();
    
    void setPosition(cocos2d::CCPoint point, int layer, int direction);
    
    virtual void touch (PlayerRobot* robot);
    
    virtual RobotType parentType()
    {
        return pmEnvRobot;
    }
    
    virtual RobotType type ()
    {
        return pmTrasportRobot4;
    }
    
    virtual int getNativeMethodCount()
    {
        return 0;
    }
    
    virtual int getConditionsCount()
    {
        return 0;
    }
    
    virtual bool moveOn()
    {
        return true;
    }
    
    virtual void restartRobot();
    
    virtual void initTexture();
};


#endif /* defined(__pictomir_cocos2d_x__TransportRobot4__) */
