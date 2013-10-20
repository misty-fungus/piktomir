//
//  RobotManager.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 29.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__Robot4Manager__
#define __pictomir_cocos2d_x__Robot4Manager__

#include <iostream>
#include <stack>
#include "UniversalRobot.h"
#include "GameParams.h"

class GameLayer;

class RobotManager
{
public:
    
    enum State
    {
        Paused,
        Controlled,
        ControlledByStack,
        Working
    };
protected:    
    SYNTHESIZE(GameLayer *, gameLayer, GameLayer);
    SYNTHESIZE(State, state, State);
    SYNTHESIZE(bool, controlled, Controlled);
private:
    
    SYNTHESIZE(UniversalRobot *,robot, Robot);
    
    _instruction pop()
    {
        _instruction ret = robot->instructionStack.top();
        robot->instructionStack.pop();
        return ret;
    }
public:
    
    RobotManager():
        gameLayer(NULL),
        state(Paused),
        controlled(false),
        robot(NULL)
    {}
        
    static RobotManager* create()  {
        return new RobotManager;
    }
    
    RobotManager(UniversalRobot *robot):
        gameLayer(NULL),
        state(Paused),
        controlled(false),
        robot(robot)
    {}
    
    static RobotManager* create(UniversalRobot *robot)
    {
        return new RobotManager(robot);
    }
    
    void addInstruction(const _instruction& instr);
    void addInstructions(const std::vector<_instruction> &instrList);
    
    void loop();
    
    bool empty()
    {
        return robot->instructionStack.empty();
    }
    
    void clear()
    {
        robot->instructionStack = std::stack<_instruction> ();
    }
protected:
};


#endif /* defined(__pictomir_cocos2d_x__RobotManager__) */
