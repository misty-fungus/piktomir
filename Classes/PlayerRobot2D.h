//
//  PlayerRobot2D.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/9/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef pictomir_cocos2d_x_PlayerRobot2D_h
#define pictomir_cocos2d_x_PlayerRobot2D_h

#include "cocos2d.h"
#include "PlayerRobot.h"


class PlayerRobot2D : public PlayerRobot
{
    SYNTHESIZE(cocos2d::CCPoint, curPosition, CurPosition);
    SYNTHESIZE(int, curDirection, CurDirection);
    
    SYNTHESIZE(cocos2d::CCPoint, oldPosition, OldPosition);
    SYNTHESIZE(int, oldDirection, OldDirection);
    
    SYNTHESIZE(cocos2d::CCPoint, startPosition, StartPosition);
    SYNTHESIZE(int, startDirection, StartDirection);
    
public:
    PlayerRobot2D(UniversalWorld *parentWorld):
        PlayerRobot(parentWorld),
        curPosition( cocos2d::CCPointZero ),
        curDirection(0),
        oldPosition( cocos2d::CCPointZero ),
        oldDirection(0),
        startPosition( cocos2d::CCPointZero ),
        startDirection(0)
    {}

    
    virtual void restartRobot() = 0;
    
};

#endif
