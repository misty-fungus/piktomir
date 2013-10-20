//
//  PlayerRobot.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/9/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef pictomir_cocos2d_x_PlayerRobot_h
#define pictomir_cocos2d_x_PlayerRobot_h

#include "cocos2d.h"
#include "UniversalRobot.h"


class PlayerRobot : public cocos2d::CCSprite,public UniversalRobot
{
public:
    PlayerRobot(UniversalWorld *parentWorld):
        UniversalRobot(parentWorld),
        curLayer(0),
        oldLayer(0),
        startLayer(0)
    {}
    
    SYNTHESIZE(int, curLayer, CurLayer);
    SYNTHESIZE(int, oldLayer, OldLayer);
    SYNTHESIZE(int, startLayer, StartLayer);
    
    virtual void initTexture() = 0;

};

#endif
