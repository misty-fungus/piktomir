//
//  Method.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef pictomir_cocos2d_x_RobotMethod_h
#define pictomir_cocos2d_x_RobotMethod_h

#include "cocos2d.h"
#include "GameParams.h"

class UniversalRobot;

typedef void (UniversalRobot::* ROBOT_NATIVE_METHOD)(/*UniversalRobot **/);

const short INSTR_EXECUTE = 0;
const short INSTR_CHECK_CONDITION= 1;
const short INSTR_CHECK_LOOP = 2;


struct _instruction
{
    short command;
    short param;
    short otherParams[2];
};

class RobotMethod
{
public:
    enum MethodType
    {
        NativeMethod,
        ProgrammedMethod,
        TempMethod
    };
    
    RobotMethod():
        parent(NULL),
        methodType(NativeMethod),
        nativeMethod((ROBOT_NATIVE_METHOD)NULL),
        inverseMethod(0)
    {}
    
    static RobotMethod* create()
    {
        return new RobotMethod;
    }
    
    RobotMethod(const RobotMethod &other):
        parent(other.parent),
        methodType(other.methodType),
        nativeMethod(other.nativeMethod),
        instructionList(other.instructionList),
        inverseMethod(other.inverseMethod)
    {}
        
    RobotMethod(UniversalRobot *parent,ROBOT_NATIVE_METHOD method,int inverseMethod):
        parent(parent),
        methodType(NativeMethod),
        nativeMethod(method),
        inverseMethod(inverseMethod)
    {}
    
    static RobotMethod* create(UniversalRobot *parent,ROBOT_NATIVE_METHOD method,int inverseMethod)
    {
        return new RobotMethod(parent, method, inverseMethod);
    }
    
    RobotMethod(UniversalRobot *parent,const std::vector<_instruction> instructionList, MethodType methodType):
        parent(parent),
        methodType(methodType),
        nativeMethod((ROBOT_NATIVE_METHOD)NULL),
        instructionList(instructionList),
        inverseMethod(0)
    {}
    
    static RobotMethod* create(UniversalRobot *parent,const std::vector<_instruction> instructionList, MethodType methodType)  {
        return new RobotMethod(parent,instructionList,methodType);
    }

    void execute() const;
    static std::vector<_instruction> makeMethod(std::vector<int> methods,int layerIndex);
    static std::vector<_instruction> makeExecuteMethod(int method,bool useRepeater,int repeater,bool useCondition,int condition);

    UniversalRobot *parent;
    SYNTHESIZE(MethodType, methodType, MethodType);
    ROBOT_NATIVE_METHOD nativeMethod;
    SYNTHESIZE(int, inverseMethod, InverseMethod);
    
public:    
    std::vector<_instruction> instructionList;

};

#endif
