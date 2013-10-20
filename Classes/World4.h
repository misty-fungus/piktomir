//
//  World4.h
//  pictomir-cocos2d-x
//
//  Created by Nikita Besshaposhnikov on 28.11.12.
//  Copyright (c) 2012 NIISI. All rights reserved.
//

#ifndef __pictomir_cocos2d_x__World4__
#define __pictomir_cocos2d_x__World4__

#include <iostream>
#include "UniversalWorld.h"
#include "Map4.h"
#include "Robot4.h"
#include "Oven4.h"
#include "TransportRobot4.h"
#include "RobotManager.h"
#include <libxml/xmlreader.h>

class GameLayer;

class World4 : public UniversalWorld
{
public:
    
    World4():
        UniversalWorld()
    {
    }
    
    World4(int width, int height, int layerNum, int tileset):
        UniversalWorld(Map4::create(this,width,height,layerNum,tileset))
    {
    }
    
    
    static World4* create()
    {
        return new World4();
    }
    
    static World4* create(int width, int height, int layerNum, int tileset)
    {
        return new World4(width,height,layerNum,tileset);
    }
    
    Map4 *getMap4()
    {
        return (Map4 *)map;
    }
         
    virtual WorldType type()
    {
        return pmWorld4;
    }
};

#endif /* defined(__pictomir_cocos2d_x__World4__) */
