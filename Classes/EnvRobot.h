//
//  EnvRobot.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/11/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef pictomir_cocos2d_x_EnvRobot_h
#define pictomir_cocos2d_x_EnvRobot_h

#include "cocos2d.h"
#include "UniversalRobot.h"

class PlayerRobot;

class EnvRobot : public UniversalRobot,public cocos2d::CCSprite
{
public:
    EnvRobot(UniversalWorld *parentWorld):
        UniversalRobot(parentWorld)
    {}
        
    virtual void initTexture() = 0;
    virtual void touch(PlayerRobot *) = 0;
    virtual bool moveOn() = 0;
};

#endif
