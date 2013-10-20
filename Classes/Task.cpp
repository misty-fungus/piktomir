//
//  Task.cpp
//  pictomir-cocos2d-x
//
//  Created by Danila Eremin on 2/22/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#include "Task.h"
#include "UniversalRobot.h"
#include "Robot4.h"

bool Task::Complete()
{
    bool retFlag = false;
    
    if (grassToPaint == paintedGrass)
        retFlag = true;
    
    if(furnaceToBurn == burnedFurnace)
        retFlag = true;
    
    if(robot->type() == pmRobot4)
    {
        Robot4 *robot4 = (Robot4 *)robot;
        
        if(endPoint.x == robot4->getCurPosition().x && endPoint.y == robot4->getCurPosition().y && endLayer == robot4->getCurLayer())
            retFlag = true;
        
    }
    
    return retFlag;
}

Task::Task(UniversalRobot *robot):
    robot(robot)
{
    grassToPaint = -1;
    paintedGrass = 0;
    furnaceToBurn = -1;
    burnedFurnace = 0;
    endPoint = ccp(-1,0);
    
}

void Task::clear()
{
    paintedGrass = 0;
    burnedFurnace = 0;
}