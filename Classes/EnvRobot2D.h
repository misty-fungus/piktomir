//
//  EnvRobot2D.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/11/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef pictomir_cocos2d_x_EnvRobot2D_h
#define pictomir_cocos2d_x_EnvRobot2D_h

#include "cocos2d.h"
#include "EnvRobot.h"

class EnvRobot2D : public EnvRobot
{
    SYNTHESIZE(cocos2d::CCPoint, curPosition, CurPosition);
    SYNTHESIZE(int, curLayer, CurLayer);
    SYNTHESIZE(int, curDirection, CurDirection);
    
    SYNTHESIZE(cocos2d::CCPoint, startPosition, StartPosition);
    SYNTHESIZE(int, startLayer, StartLayer);
    SYNTHESIZE(int, startDirection, StartDirection);
    
public:
    EnvRobot2D(UniversalWorld *parentWorld):
    EnvRobot(parentWorld),
        curPosition( cocos2d::CCPointZero ),
        curLayer(0),
        curDirection(0),
        startPosition( cocos2d::CCPointZero ),
        startLayer(0),
        startDirection(0)
    {}

    virtual void restartRobot() = 0;
    
};

#endif
