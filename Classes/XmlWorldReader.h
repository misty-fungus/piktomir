//
//  XmlWorldReader.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 7/3/13.
//  Copyright (c) 2013 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__XmlWorldReader__
#define __pictomir_cocos2d_x__XmlWorldReader__

#include <iostream>

#include "UniversalWorld.h"
#include "ProgramLayer.h"
#include "GameLayer.h"

class XmlWorldReader
{
public:
    UniversalWorld *readWorld(GameLayer *layer,std::string mapPath);

private:

    
    xmlNodePtr curNode;
    
    WorldType worldType;
    UniversalWorld *world;
    GameLayer *gameLayer;
    
    SYNTHESIZE(std::string, error, Error);
    
    //creating objects
    void createWorldByType();
    void createRobot(RobotType robotType);
    
    //robot Nodes
    bool readRobotNode(RobotType robotType);
    bool readRobot4Node();
    bool readOven4Node();
    bool readTransportRobot4Node();
    
    //Map Nodes
    bool readMapNode();
    bool readMap4Node();
    
    //Functions node
    bool readFunctionsNode(bool isBaseRobot);
    
    //Method Node
    bool readMethodNode(MethodLayer *methodLayer);
    
    //Task node
    bool readTaskNode();
    
    //Hints Node
    bool readHintsNode();
};


#endif /* defined(__pictomir_cocos2d_x__XmlWorldReader__) */
