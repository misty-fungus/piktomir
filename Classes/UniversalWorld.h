//
//  UniversalWorld.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__UniversalWorld__
#define __pictomir_cocos2d_x__UniversalWorld__

#include <iostream>
#include <vector>
#include <algorithm>
#include "RobotManager.h"
#include "UniversalRobot.h"
#include "Hints.h"
#include "Task.h"
#include "Map.h"

class Map;

enum WorldType
{
    pmUniversalWorld = -1,
    pmWorld4 = 0
};

class UniversalWorld
{
private:
    std::vector<UniversalRobot *> robotList;
    std::vector<RobotManager *> robotManagers;
    int baseRobot;
protected:
    SYNTHESIZE(Map *, map,  Map);
    SYNTHESIZE(Task *, task, Task);
public:
    
    void sortRobotList(bool (*sortPredicate)(const void*,const void*))
    {
        std::sort(robotList.begin(), robotList.end(), sortPredicate);
    }
    
    UniversalWorld() :
        robotList(std::vector<UniversalRobot *>()),
        robotManagers(std::vector<RobotManager *>()),
        baseRobot(-1),
        map(NULL),
        task(NULL)
    {}
    
    UniversalWorld(Map *map) :
        robotList(std::vector<UniversalRobot *>()),
        robotManagers(std::vector<RobotManager *>()),
        baseRobot(-1),
        map(map),
        task(NULL)
    {}
    
    virtual ~UniversalWorld()
    {
//        CC_SAFE_DELETE(map);
        CC_SAFE_DELETE(task);
        
        for(int i = 0; i < robotCount(); ++i)
        {
            //CC_SAFE_DELETE(robotList[i]);
            CC_SAFE_DELETE(robotManagers[i]);
        }
        
    }
    
    static UniversalWorld* create(Map *map)
    {
        return new UniversalWorld(map);
    }
    
    static UniversalWorld* create()
    {
        return new UniversalWorld;
    }
    
    int robotCount() const
    {
        return robotList.size();
    }
    
    UniversalRobot *robotAt(int index)
    {
        return robotList[index];
    }
        
    RobotManager *robotManagerAt(int index)
    {
        return robotManagers[index];
    }
    
    void addRobot(UniversalRobot *robot)
    {
        robotList.push_back(robot);
        robotManagers.push_back(new RobotManager(robot));
    }

    void setBaseRobot(int robot)
    {
        baseRobot = robot;
    }
    
    UniversalRobot *getBaseRobot()
    {
        if(baseRobot < 0 || baseRobot >= robotList.size())
            return NULL;
        
        return robotList[baseRobot];
    }
    
    RobotManager *getBaseRobotManager()
    {
        if(baseRobot < 0 || baseRobot >= robotList.size())
            return NULL;
        
        return robotManagers[baseRobot];
    }
    
    virtual WorldType type()
    {
        return pmUniversalWorld;
    }
    
    bool robotsWin()
    {
        bool flag = true;
        
        for(int  i = 0; i < robotCount(); ++i)
        {
            if(robotAt(i)->getTask() != NULL && robotAt(i)->getState() != UniversalRobot::Win)
                flag = false;
        }
        
        return flag;
    }
    
    bool robotsLoose()
    {
        bool flag = false;
        
        for(int  i = 0; i < robotCount(); ++i)
        {
            if(robotAt(i)->getTask() != NULL && robotAt(i)->getState() == UniversalRobot::Broken)
                flag = true;
        }
        
        return flag;
    }
    
    bool robotsReady(UniversalRobot *exceptRobot)
    {
        bool flag = true;
        
        for(int  i = 0; i < robotCount(); ++i)
        {
            if(robotAt(i) != exceptRobot && robotAt(i)->getState() != UniversalRobot::Ready && robotAt(i)->getState() != UniversalRobot::Working)
                flag = false;
        }
        
        return flag;
    }
};

#endif /* defined(__pictomir_cocos2d_x__UniversalWorld__) */
