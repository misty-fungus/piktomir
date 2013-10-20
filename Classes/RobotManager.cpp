//
//  RobotManager.cpp
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 29.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#include "RobotManager.h"
//#include "World4.h"
#include "GameLayer.h"
#include "GameParams.h"

//pthread_mutex_t mutex;

using namespace std;
USING_NS_CC;


void RobotManager::loop()
{
    if(robot->getParentWorld()->robotsWin() && !controlled) {
        robot->win();
        
        if(gameLayer!=NULL)
            gameLayer->robotWin();
        return;
    }
    else if(robot->getParentWorld()->robotsLoose()) {
        robot->destroy();
        
        if(gameLayer != NULL)
            gameLayer->robotLoose();
        return;
    }
    else if(robot->getState() == UniversalRobot::Working)
        return;
    
    if(state == Paused)
        return;
    
    if(robot->instructionStack.empty() && robot->getState() == UniversalRobot::Ready && robot->parentType() == pmPlayerRobot) {
        
        if(gameLayer != NULL)
            gameLayer->robotFailure();
        
        return;
    }
    
    if(robot->instructionStack.empty() )
        return;
    
    _instruction instr = pop();
    
    switch(instr.command)
    {
        case INSTR_EXECUTE:
        {
            RobotMethod method = robot->getMethod(instr.param);
            
            if(method.getMethodType() == RobotMethod::NativeMethod)
            {
                robot->setState(UniversalRobot::Ready);
                
                if(!robot->getParentWorld()->robotsReady(robot))
                {
                    addInstruction(instr);
                    return;
                }
                    
                method.execute();
                
                if(state == Controlled)
                    state = Paused;
                
                if(state == ControlledByStack && robot->instructionStack.empty())
                    state = Paused;
            }
            else
            {
                if(instr.param == CMDMAIN && method.instructionList.empty())
                    gameLayer->robotFailure();
                else
                {
                    addInstructions(method.instructionList);
                    robot->setState(UniversalRobot::Waiting);
                }
            }
            //continue;
            //highlight button
            if(instr.otherParams[0] != -1 && instr.otherParams[1] != -1)
                if(gameLayer!=NULL)
                    gameLayer->highLightButon(instr.otherParams[0], instr.otherParams[1]);
            
            
        } break;
        case INSTR_CHECK_LOOP:
        {
            
            
            if(instr.otherParams[0] == MaxRepeater) {
                gameLayer->robotFailure();
                break;
            }
            
            _instruction cond_instr;
            
            if(instr.otherParams[1])
                cond_instr = pop();
            
            _instruction exec_instr = pop();
            
            if((instr.otherParams[0] < instr.param) &&
               (!instr.otherParams[1] || (instr.otherParams[1] && robot->checkCondition(cond_instr.param))))
            {
                robot->setState(UniversalRobot::Waiting);
                
                ++instr.otherParams[0];
                
                addInstruction(exec_instr);
                
                if(instr.otherParams[1])
                    addInstruction(cond_instr);
                
                addInstruction(instr);
                
                addInstruction(exec_instr);
            }
            
        } break;
        case INSTR_CHECK_CONDITION:
        {
            
            
            if(!robot->checkCondition(instr.param))
                pop();
            else
                robot->setState(UniversalRobot::Waiting);
            
        } break;
    }
    
}


void RobotManager::addInstruction(const _instruction& instr)
{   //add sync for network
    robot->instructionStack.push(instr);
}

void RobotManager::addInstructions(const std::vector<_instruction> &instrList)
{
    for(int i = instrList.size() - 1;i >= 0 ; --i)
        addInstruction(instrList[i]);
}