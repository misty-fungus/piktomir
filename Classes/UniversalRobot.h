//
//  UniversalRobot.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef pictomir_cocos2d_x_UniversalRobot_h
#define pictomir_cocos2d_x_UniversalRobot_h

#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: varType get##funName(void) const { return varName; }\
public: void set##funName(varType var){ varName = var; }

#include "cocos2d.h"
#include "RobotMethod.h"
#include <map>
#include <vector>
#include <stack>
#include <libxml/xmlreader.h>
#include "Task.h"

class UniversalWorld;
class ProgramLayer;
class Task;

const int EMPTY_METHOD = 0;
const int CMDA = -2;
const int CMDB = -3;
const int CMDC = -4;
const int CMDMAIN = -5;
const int CMDA_EXECUTE = -6;
const int CMDB_EXECUTE = -7;
const int CMDC_EXECUTE = -8;
const int CMDMAIN_EXECUTE = -9;

const int EMPTY_CONDITION = 0;

enum RobotType
{
    pmEnvRobot = -3,
    pmPlayerRobot = -2,
    pmUniversalRobot = -1,
    pmRobot4 = 0,
    pmOven4 = 1,
    pmTrasportRobot4,
    pmRobotCount = 3
};

class UniversalRobot
{
    SYNTHESIZE(UniversalWorld *, parentWorld, ParentWorld);
    
    SYNTHESIZE(int, stepCount, StepCount);
    
    SYNTHESIZE(Task *, task, Task);
public:
    enum RobotState
    {
        NoState,
        Ready,
        Working,
        Waiting,
        Win,
        Broken
    };
    
    enum RobotControl
    {
        NoControl,
        Programm,
        Network,
        Auto
    };
    
    
    SYNTHESIZE(RobotState, state, State);
    std::map<int,RobotMethod> methodList;
    std::stack<_instruction> instructionStack;
    
public:
    UniversalRobot():
        parentWorld(NULL),
        state(Ready),
        task(NULL)
    {}
    
    UniversalRobot(UniversalWorld *parentWorld):
        parentWorld(parentWorld),
        state(Ready),
        task(NULL)
    {}
    
    virtual ~UniversalRobot()
    {
        CC_SAFE_DELETE(task);
    }
    
    static UniversalRobot* create()
    {
        return new UniversalRobot();
    }
    
    static UniversalRobot* create(UniversalWorld *parentWorld)
    {
        return new UniversalRobot(parentWorld);
    }
    
    
    void execute(int methodID) const
    {
        //if(methodList.find(methodName).)
        methodList.at(methodID).execute();
    }
    
    RobotMethod getMethod(int methodID) const
    {
        //if(methodList.find(methodName).)
        return methodList.at(methodID);
    }
    
    void addMethod(int key, RobotMethod method)
    {
        methodList.insert(std::pair<int,RobotMethod>(key,method));
    }
    
    std::vector<int> getMethodList()
    {
        std::vector<int> ret;
        
        for(std::map<int,RobotMethod>::iterator i = methodList.begin(); i != methodList.end(); ++i)
        {
            ret.push_back((*i).first);
        }
        
        return ret;
    }    
    
    void cleanTempMethods()
    {
        for(std::map<int,RobotMethod>::iterator i = methodList.begin(); i != methodList.end();) {
            if((*i).second.getMethodType() == RobotMethod::TempMethod) {
                methodList.erase(i++);
            } else {
				++i;
			}
        }

    }
    
    virtual bool checkCondition(int condition)
    {
        return true;
    }

    virtual void destroy() {}
    virtual void win() {}
    
    virtual RobotType type ()
    {
        return pmUniversalRobot;
    }
    
    virtual int getNativeMethodCount()
    {
        return 0;
    }
    
    virtual int getConditionsCount()
    {
        return 0;
    }
    
    virtual RobotType parentType()
    {
        return pmUniversalRobot;
    }
    
};


#endif
