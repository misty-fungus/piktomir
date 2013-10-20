//
//  RobotMethod.c
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//
#include <stdio.h>
#include "RobotMethod.h"
#include "UniversalRobot.h"
#include "GameParams.h"
USING_NS_CC;

using namespace std;

void RobotMethod::execute() const
{
    if(methodType == NativeMethod)
        (parent->*nativeMethod) ();
        //nativeMethod(parent);
}

vector<_instruction> RobotMethod::makeMethod(std::vector<int> methods,int layerIndex)
{
    vector<_instruction> ret;
    _instruction instr;
    
//    if(useRepeater && repeater != 0)
//    {
//        instr.command = INSTR_CHECK_LOOP;
//        
//        if(repeater < 7)
//            instr.param = repeater;
//        else
//            instr.param = MaxRepeater ;
//        
//        instr.otherParams[0] = 0;
//        instr.otherParams[1] = (useCondition && condition != EMPTY_CONDITION) ? 1 : 0;
//        ret.push_back(instr);
//    }
//    
//    if(useCondition && condition != -1)
//    {
//        instr.command = INSTR_CHECK_CONDITION;
//        instr.param = condition;
//        instr.otherParams[0] = 0;
//        instr.otherParams[1] = 0;
//        ret.push_back(instr);
//    }

    
    for(int i= 0; i < methods.size(); ++i)
    {
        
        if(methods[i] != EMPTY_METHOD)
        {
            instr.command = INSTR_EXECUTE;
            instr.param = methods[i];
            instr.otherParams[0] = layerIndex;
            instr.otherParams[1] = i;
            ret.push_back(instr);
        }
        
    }
    
    return ret;
}

vector<_instruction> RobotMethod::makeExecuteMethod(int method,bool useRepeater,int repeater,bool useCondition,int condition)
{
    vector<_instruction> ret;
    _instruction instr;
    
    if(useRepeater && repeater != 0)
    {
        instr.command = INSTR_CHECK_LOOP;
        
        if(repeater < 7)
            instr.param = repeater;
        else
            instr.param = MaxRepeater ;
        
        instr.otherParams[0] = 0;
        instr.otherParams[1] = (useCondition && condition != EMPTY_CONDITION) ? 1 : 0;
        ret.push_back(instr);
    }
    
    if(useCondition && condition != -1)
    {
        instr.command = INSTR_CHECK_CONDITION;
        instr.param = condition;
        instr.otherParams[0] = 0;
        instr.otherParams[1] = 0;
        ret.push_back(instr);
    }
    
    
    if(method != EMPTY_METHOD)
    {
        instr.command = INSTR_EXECUTE;
        instr.param = method;
        instr.otherParams[0] = -1;
        instr.otherParams[1] = -1;
        ret.push_back(instr);
    }
    
    return ret;
}